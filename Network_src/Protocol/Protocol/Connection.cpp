#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include "Connection.hpp"
#include "Client.hpp"

Connection::Connection(Client& cl, boost::asio::io_service& ios, const char* host, short port) :
	client(cl), socket(ios)
{
	socket.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(host), port));
}

Connection::~Connection()
{
	socket.shutdown(tcp::socket::shutdown_send);
	socket.close();
}

/*
**	Writes request (synchronous)
*/
void	Connection::syncRequest(Message *msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);

	socket.send(boost::asio::buffer(msg, msgSize));
}

/*
**	Reads response (synchronous)
**	while response is empty, retry receive
*/
void	Connection::syncResponse(Message *msg, std::size_t msgSize, MsgType expected)
{
	socket.receive(boost::asio::buffer(msg, msgSize));
	if (isInvalidResponse(*msg))
		syncResponse(msg, msgSize, expected);
	else
		responseHeaderCheck(*msg, expected);
}

/*
**	Writes request (asynchronous)
*/
void	Connection::request(MessagePtr msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);
	socket.async_send(boost::asio::buffer(msg.get(), msgSize),
		boost::bind(&Connection::sendHandler, this, msg, boost::asio::placeholders::error)
	);
}

/*
**	Reads response header (asynchronous)
**	Its callback reads response payload (asynchronous)
*/
void		Connection::responseExpect()
{
	MessagePtr	header(new Message());

	socket.async_receive(boost::asio::buffer(header.get(), sizeof(*header)),
		boost::bind(&Connection::response, this, header, boost::asio::placeholders::error)
	);
}

void	Connection::sendHandler(MessagePtr msg, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::REQUEST, static_cast<MsgType>(msg->type));
}

//#include <fstream>

//static std::ofstream	ofs_send("send.txt", std::ofstream::out);
//static std::ofstream	ofs_recv("recv.txt", std::ofstream::out);

void		Connection::response(MessagePtr header, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::RESPONSE);

	/*
	ofs_recv.write(reinterpret_cast<char *>(header.get()), sizeof(*header));
	std::flush(ofs_recv);
	*/

	if (!isInvalidResponse(*header))
	{
		uint8*		buff = new uint8[header->length - 1];

		std::cout	<< "[ASYNC] receved response header: type="
					<< (int)(header->type) << " length=" << header->length << std::endl;

		auto boostFunc = boost::bind(&Connection::recvHandler, this, header, buff, boost::asio::placeholders::error);
		socket.async_receive(boost::asio::buffer(buff, header->length - 1), boostFunc);
	}
	else
	{
		responseExpect(); // response is empty -> retry
	}
}

void	Connection::recvHandler(MessagePtr header, uint8 *buf, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::RESPONSE);
	disconnectCheck(*header);

	/*
	ofs_recv.write(reinterpret_cast<char *>(buf), header->length - 1);
	std::flush(ofs_recv);
	*/

	if (!isIgnoredResponse(*header))
	{
		MessagePtr		msg = client.getMsgFactory().makeResponse(*header);
		if (msg != nullptr) // unimplemented response
		{
			uint8*			msgData = reinterpret_cast<uint8*>(msg.get());

			std::memcpy(msgData, header.get(), sizeof(*header));
			std::memcpy(msgData + sizeof(*header), buf, header->length - 1);

			client.notify(msg);
		}
	}

	delete[] buf;
	responseExpect();
}

inline bool	Connection::isInvalidResponse(const Message& msg) const
{
	if (msg.type > static_cast<uint8>(MsgType::AUTH_KEY))
	{
		std::cout << "[INVALID] receved response header: type="
			<< (int)(msg.type) << " length=" << (int)msg.length << std::endl;
		return true;
	}
	//return (msg.type == 0 && msg.length == 0);
	return msg.type == 0;
}

void	Connection::responseHeaderCheck(const Message& res, MsgType expectedType)
{
	disconnectCheck(res);

	Message	expected;
	expected.type = static_cast<uint8>(expectedType);
	//expected.length = res.length;
	//expected.length -= sizeof(res.length);

	if (res.type != expected.type)
	{
		std::ostringstream os;

		os << "ERROR: unexpected response " << static_cast<int>(res.type)
			<< " (expected: " << static_cast<int>(expected.type) << ").";
		std::cerr << os.str() << std::endl;
	}

	/*
	if (res.length != expected.length)
	{
		std::cerr << "WARNING: unexpected response[type:" << static_cast<int>(res.type)
			<< "] of size " << res.length
			<< " (expected: " << expected.length << ")." << std::endl;
	}
	*/
}

/*
**	Checks if there were network (i.e. asio) errors in async_send/receive
*/
inline void Connection::asyncTaskCheck(const boost::system::error_code& err, TaskType taskType,
										MsgType msgType)
{
	if (err)
	{
		std::ostringstream	os;

		os << "ERROR: ";
		if (taskType == TaskType::RESPONSE)
			os << "async_receive(): ";
		else
			os << "async_send(): ";
		os << "[Message type: ";

		if (msgType != MsgType::NO_MSG)
			os << static_cast<int>(msgType);
		else
			os << "EMPTY";
		os << "]." << std::endl << "\treason: " << err.message() << std::endl;

		throw std::runtime_error(os.str());
	}
}

const std::vector<MsgType>	Connection::ignoredResponses =
{
	MsgType::STATUSBAR,
	MsgType::RECALCULATE,
	MsgType::BALANCE_STATS
}; 

inline bool	Connection::isIgnoredResponse(const Message& header) const
{
	auto ret = std::find(ignoredResponses.begin(), ignoredResponses.end(), static_cast<MsgType>(header.type));
	return ret != ignoredResponses.end();
}

inline void	Connection::disconnectCheck(const Message& msg) const
{
	if (msg.type == static_cast<uint8>(MsgType::FATAL_ERR))
	{
		auto buf = reinterpret_cast<const char *>(&msg);
		throw std::runtime_error("Connection rejected by server: " + std::string(buf + sizeof(msg)));
	}
}


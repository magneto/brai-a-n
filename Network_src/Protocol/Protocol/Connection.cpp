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

#include <fstream>

static std::ofstream	ofs_send("send.txt", std::ofstream::out);
static std::ofstream	ofs_recv("recv.txt", std::ofstream::out);

/*
**	Writes request (synchronous)
*/
void	Connection::syncRequest(Message *msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);

	socket.send(boost::asio::buffer(msg, msgSize));
	/*
	ofs_send.write(reinterpret_cast<char *>(msg), msgSize);
	std::flush(ofs_send);
	*/
}

//#include <boost/array.hpp>

/*
**	Reads response (synchronous)
**	while response is empty, retry receive
*/
void	Connection::syncResponse(Message *msg, std::size_t msgSize, MsgType expected)
{
	/*
	boost::array<char, 1024>	buf;
	auto ret = socket.receive(boost::asio::buffer(buf));
	std::cout << "received " << ret << " bytes" << std::endl;
	responseHeaderCheck(*reinterpret_cast<Message *>(&(buf[0])), expected);

	std::memcpy(msg, reinterpret_cast<Message *>(&(buf[0])), ret);
	return;
	*/

	socket.receive(boost::asio::buffer(msg, msgSize));
	if (isEmptyResponse(*msg))
		syncResponse(msg, msgSize, expected);
	else
	{
		/*
		if (msg->type == 0x07)
		{
			ofs_recv.write(reinterpret_cast<char *>(msg), msgSize);
			std::flush(ofs_recv);
		}
		*/
		responseHeaderCheck(*msg, expected);
	}
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
	std::shared_ptr<Message>	header(new Message());

	socket.async_receive(boost::asio::buffer(header.get(), sizeof(*header)),
		boost::bind(&Connection::response, this, header, boost::asio::placeholders::error)
		);
}

void	Connection::sendHandler(MessagePtr msg, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::REQUEST, static_cast<MsgType>(msg->type));
}

void		Connection::response(std::shared_ptr<Message> header, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::RESPONSE);
	if (!isEmptyResponse(*header))
	{
		auto len = header->length;
		std::cout << "[ASYNC] receved response header: type="
				<< (int)(header->type) << " length=" << len << std::endl;

		std::shared_ptr< std::vector<uint8> > buff(new std::vector<uint8>(len + sizeof(len)));

		std::memcpy(buff->data(), header.get(), sizeof(*header));

		socket.async_receive(boost::asio::buffer(buff->data() + sizeof(header), len),
			boost::bind(&Connection::recvHandler, this, buff, boost::asio::placeholders::error)
			);
	}
	else
	{
		responseExpect(); // response is empty -> retry
	}
}

void		Connection::recvHandler(std::shared_ptr< std::vector<uint8> > buf, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::RESPONSE);

	if (!isIgnoredResponse(*reinterpret_cast<Message*>(buf->data())))
	{
		MessagePtr	msg = client.getMsgFactory().makeResponse(*reinterpret_cast<Message*>(buf->data()));
		uint8*		data = reinterpret_cast<uint8*>(msg.get());
		std::size_t	msgSize = sizeof(*msg);

		std::memcpy(data + msgSize, buf->data() + msgSize, buf->size() - msgSize);

		disconnectCheck(*msg);
		client.notify(msg);
	}

	responseExpect();
}

inline bool	Connection::isEmptyResponse(const Message& msg) const
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
	MsgType::RECALCULATE
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


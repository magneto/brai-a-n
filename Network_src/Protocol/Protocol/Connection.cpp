#include <iostream>
#include <bitset>
#include <boost/bind.hpp>
#include "Connection.hpp"
#include "Client.hpp"
#include <boost/make_shared.hpp>
#include <vector>

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
	socket.read_some(boost::asio::buffer(msg, msgSize));
	if (isEmptyResponse(*msg))
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
	std::shared_ptr<Message>	header = std::make_shared<Message>();

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

		boost::shared_ptr< std::vector<uint8> > buff(new std::vector<uint8>(len + sizeof(len)));

		std::memcpy(&((buff.get())[0]), header.get(), sizeof(*header));

		socket.async_receive(boost::asio::buffer(buff.get() + sizeof(header), len),
			boost::bind(&Connection::recvHandler, this, buff, boost::asio::placeholders::error)
			);
	}
	else
	{
		responseExpect(); // response is empty -> retry
	}
}

void		Connection::recvHandler(boost::shared_ptr< std::vector<uint8> > buf, const boost::system::error_code& err)
{
	asyncTaskCheck(err, TaskType::RESPONSE);

	MessagePtr msg = client.getMsgFactory().makeResponse(*reinterpret_cast<Message*>(buf.get()));
	std::memcpy(msg.get(), &((buf.get())[0]), buf->size());
	disconnectCheck(*msg);

	client.notify(msg);
	responseExpect();
}

inline bool	Connection::isEmptyResponse(const Message& msg) const
{
	return (msg.type == 0 && msg.length == 0);
}

void	Connection::responseHeaderCheck(const Message& res, MsgType expectedType)
{
	disconnectCheck(res);

	Message	expected;
	expected.type = static_cast<uint8>(expectedType);
	expected.length = res.length;
	//expected.length -= sizeof(res.length);

	if (res.type != expected.type)
	{
		std::ostringstream os;

		os << "ERROR: unexpected response " << static_cast<int>(res.type)
			<< " (expected: " << static_cast<int>(expected.type) << ").";
		std::cerr << os.str() << std::endl;
	}

	if (res.length != expected.length)
	{
		std::cerr << "WARNING: unexpected response[type:" << static_cast<int>(res.type)
			<< "] of size " << res.length
			<< " (expected: " << expected.length << ")." << std::endl;
	}
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

inline void	Connection::disconnectCheck(const Message& msg) const
{
	if (msg.type == static_cast<uint8>(MsgType::FATAL_ERR))
	{
		auto buf = reinterpret_cast<const char *>(&msg);
		throw std::runtime_error("Connection rejected: " + std::string(buf + sizeof(msg)));
	}
}

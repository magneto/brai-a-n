#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <boost/bind.hpp>
#include "Client.hpp"

Client::Client(const char* host, short port):
	io_service(), socket(io_service), msgFactory()
{
	socket.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(host), port));
}

Client::~Client()
{
	socket.shutdown(tcp::socket::shutdown_send);
	socket.close();
}

/*
**	Starts asynchronous I/O if asynchronous sends/receives are performed
**
**	Runs server connection process (synchrone)
*/
void	Client::run()
{
	login();
	responseExpect();
	io_service.run();
}

/*
**	Stops asynchronous tasks
*/
void	Client::stop()
{
	io_service.stop();
}

/*
**	LOT TO DO HERE
*/

/*
**	Yields the server responses queue
*/
std::queue<MessagePtr>	Client::getQueue()
{
	return queue;
}

/*****************************************************************************
**	PRIVATE
*****************************************************************************/

/*
**	Launches login to server process (asynchronous)
*/
void	Client::login()
{
	connection();
	playerConfiguration();
	worldInfo();
}

void	Client::connection()
{
	{
		auto		msg = msgFactory.makeRequest(MsgType::CONNECT);
		Connect*	c = static_cast<Connect *>(msg.get());

		std::strncpy(c->version, CLIENT_VERSION, sizeof(c->version));
		syncRequest(c, sizeof(*c));
	}

	Accepted	a;
	syncResponse(&a, sizeof(a));
}

void	playerConfiguration()
{
	// TO DO
}

void	worldInfo()
{
	// TO DO
}

void	Client::syncRequest(Message *msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);
	socket.send(boost::asio::buffer(msg, msgSize));
}

void	Client::syncResponse(Message *msg, std::size_t msgSize)
{
	socket.read_some(boost::asio::buffer(msg, msgSize));
	responseTypeCheck(*msg);
}

void	Client::request(MessagePtr msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);
	socket.async_send(boost::asio::buffer(msg.get(), msgSize),
		boost::bind(&Client::sendHandler, this, msg, boost::asio::placeholders::error)
	);
}

void	Client::sendHandler(MessagePtr msg, const boost::system::error_code& err)
{
	if (err)
	{
		std::fprintf(stderr, "send() error occured. Message type is %x.\n", msg->type);
		std::cerr << "\treason: " <<  err.message() << std::endl;
	}
}

void		Client::responseExpect()
{
	std::shared_ptr<int32>	length = std::make_shared<int32>();

	socket.async_receive(boost::asio::buffer(length.get(), sizeof(*(length.get()))),
		boost::bind(&Client::response, this, length, boost::asio::placeholders::error)
	);
}

void		Client::response(std::shared_ptr<int32> lengthPtr, const boost::system::error_code& err)
{
	recvErrCheck(err);
	int32 len = *(lengthPtr.get());
	if (!responseLenCheck(len))
		return ;

	boost::shared_array<int8>	buff(new int8[len + sizeof(len)]);
	std::memcpy(buff.get(), &len, sizeof(len));

	socket.async_receive(boost::asio::buffer(buff.get() + sizeof(len), len),
		boost::bind(&Client::recvHandler, this, buff, boost::asio::placeholders::error)
	);
}

void		Client::recvHandler(boost::shared_array<int8> buf, const boost::system::error_code& err)
{
	recvErrCheck(err);

	auto msg = MessagePtr(reinterpret_cast<Message*>(buf.get())); // cancer ?
	responseTypeCheck(*msg);

	queue.push(msg);
	responseExpect();
}

inline bool	Client::responseLenCheck(int32 len)
{
	if (len == 0)
	{
		responseExpect();
		return false;
	}
	else if (len < 0)
	{
		std::ostringstream	os;
		os << "Unexpected error occured : invalid len value in server message (" << len << ").";
		throw std::runtime_error(os.str());
	}
	return true;
}

inline void	Client::responseTypeCheck(const Message& msg)
{
	if (msg.type == static_cast<int8>(MsgType::FATAL_ERR))
	{
		auto buf = reinterpret_cast<const char *>(&msg);
		throw std::runtime_error("Connection rejected: " + std::string(buf + sizeof(msg)));
	}
	else
	{
		std::cout
			<< "Receved server response." << std::endl 
			<< "\tmessageType: " << static_cast<int>(msg.type) << std::endl;
	}
}

inline void Client::recvErrCheck(const boost::system::error_code& err)
{
	if (err)
	{
		std::ostringstream os;

		os << "recv() error occured." << std::endl << "\treason: " << err.message() << std::endl;
		throw std::runtime_error(os.str());
	}
}

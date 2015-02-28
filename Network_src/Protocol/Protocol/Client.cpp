#include <iostream>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <boost/array.hpp>
#include "Client.h"

Client::Client(const char* host, short port):
	io_service(), socket(io_service), msgFactory(),
	serverEp(boost::asio::ip::address_v4::from_string(host), port)
{
	socket.open(udp::v4());
}

Client::~Client()
{
	socket.shutdown(udp::socket::shutdown_send);
	socket.close();
}

bool	Client::connection()
{
	{
		auto absWrap = msgFactory.makeRequest(MessageType::CONNECT);
		auto msgWrap = std::static_pointer_cast<Requests::Connect>(absWrap);
		std::strcpy(msgWrap->msg->version, CLIENT_VERSION);
		request(msgWrap);
	}

	auto absWrap = msgFactory.makeResponse(MessageType::ACCEPTED);
	auto msgWrap = std::static_pointer_cast<Responses::Accept>(absWrap);
	response(absWrap);

	bool ret = (static_cast<MessageType>(msgWrap->msg->header.type) == MessageType::ACCEPTED);

	if (ret)
	{
		player.slot = msgWrap->msg->playerSlot;
	}
	return ret;
}

void	Client::request(std::shared_ptr<MsgWrapper> msgWrapper)
{
	std::size_t	size = msgWrapper->getSize();
	//msgWrapper->hostToNet();
	socket.send_to(boost::asio::buffer(msgWrapper->getStream(), size), serverEp);
}

void	Client::response(std::shared_ptr<MsgWrapper> msgWrapper)
{
	udp::endpoint	remoteEp;

	boost::asio::mutable_buffers_1	buff(msgWrapper->getStream(), msgWrapper->getSize());
	socket.receive_from(buff, remoteEp);
	msgWrapper->netToHost();
	// eventually add check on remote endpoint...
}

/*
**	Starts asynchronous I/O if asynchronous sends/receives are performed
**
**	Runs server connection process (synchrone)
*/
void	Client::run()
{
	if (connection())
		std::cout << "connected" << std::endl;
	else
		std::cout << "connection failed" << std::endl;

	io_service.run();
}

void	Client::stop()
{
	io_service.stop();
}

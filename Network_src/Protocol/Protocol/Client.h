#pragma once

# include "boost/asio.hpp"
# include "MessageFactory.h"
# define DFL_HOST		"127.0.0.1"
# define DFL_PORT		7777


using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Client
{
public:
	Client(const char* host = DFL_HOST, short port = DFL_PORT);
	~Client();

	bool	connection();

	void	run();
	void	stop();

	struct Player
	{
		int8	slot;

	}			player;

private:
	boost::asio::io_service	io_service;
	MessageFactory			msgFactory;
	udp::socket				socket;
	udp::endpoint			serverEp;

	void	request(std::shared_ptr<MsgWrapper> msgWrapper);
	void	response(std::shared_ptr<MsgWrapper> msgWrapper);
};
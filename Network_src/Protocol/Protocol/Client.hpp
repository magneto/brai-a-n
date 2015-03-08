#pragma once

# include <utility>
# include <boost/shared_array.hpp>
# include "MsgFactory.hpp"
# include "TerrariaInfo.hpp"
# include "Player.hpp"
# include "Connection.hpp"
# include "MsgQueue.hpp"

# ifdef _WIN32
#  include <Windows.h>
#  define usleep	Sleep
# else
#  include <unistd.h>
# endif //_WIN32

class Client
{
public:
	Client(const char* host = TERR_DFL_HOST, short port = TERR_DFL_PORT);
	~Client() = default;
	Client(const Client&) = delete;
	Client&	operator=(const Client&) = delete;

	void	run();
	void	stop();

	void	notify(MessagePtr msg);
	MsgQueue&	getResponseQueue();
	MsgFactory&	getMsgFactory();

private:
	boost::asio::io_service	io_service;
	Connection				connection;
	MsgFactory				msgFactory;
	Player					player;
	WorldInfo				worldInfo;
	MsgQueue				responseQueue;

	bool	filterResponse(MessagePtr msg);

	void	loginProcess();
	void	spawn(const Spawn& sp);

	void	login();
	void	appearanceSet();
	void	keySend();
	void	playerConfiguration();
	void	inventorySet();
	void	worldInfoFetch();
};
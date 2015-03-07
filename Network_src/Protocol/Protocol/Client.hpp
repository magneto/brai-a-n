#pragma once

# include <queue>
# include <utility>
# include <boost/asio.hpp>
# include <boost/shared_array.hpp>
# include "MessageFactory.hpp"
# include "TerrariaInfo.hpp"
# include "Player.hpp"

using boost::asio::ip::tcp;

class Client
{
public:
	Client(const char* host = DFL_HOST, short port = DFL_PORT);
	~Client();
	Client(const Client&) = delete;
	Client&	operator=(const Client&) = delete;

	void	run();
	void	stop();

	std::queue<MessagePtr>	getQueue();

private:
	boost::asio::io_service	io_service;
	MessageFactory			msgFactory;
	tcp::socket				socket;
	std::queue<MessagePtr>	queue;
	Player					player;
	WorldInfo				worldInfo;

	void	login();
	void	connection();
	void	playerConfiguration();
	void	setAppearance(Appearance& a);
	void	inventorySet();
	void	worldInfoFetch();

	void	syncRequest(Message *msg, std::size_t msgSize);
	void	syncResponse(Message *msg, std::size_t msgSize);


	void	request(MessagePtr msg, std::size_t);
	void	sendHandler(MessagePtr msg, const boost::system::error_code& err);

	void	responseExpect();
	void	response(std::shared_ptr<int32> length, const boost::system::error_code& err);
	void	recvHandler(boost::shared_array<uint8> buf, const boost::system::error_code& err);

	inline bool	responseLenCheck(int32 len);
	inline void	responseTypeCheck(const Message &msg);
	inline void recvErrCheck(const boost::system::error_code& err);

	template<typename T>
	void	syncResponseCheck(const T& res, MsgType type);
};
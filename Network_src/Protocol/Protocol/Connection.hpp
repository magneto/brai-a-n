#pragma once

# include <vector>
# include <boost/asio.hpp>
# include "MsgFactory.hpp"
# include "MsgQueue.hpp"

using boost::asio::ip::tcp;

class	Client;

//#include <boost/shared_ptr.hpp>

class Connection
{
public:
	Connection(Client& cl, boost::asio::io_service& ios, const char* host, short port);
	~Connection();
	Connection(const Connection&) = delete;
	Connection&	operator=(const Connection&) = delete;

	void	syncRequest(Message *msg, std::size_t msgSize);
	void	syncResponse(Message *msg, std::size_t msgSize, MsgType expected);

	void	request(MessagePtr msg, std::size_t);
	void	responseExpect();

private:
	Client&		client;
	tcp::socket	socket;

	static const std::vector<MsgType>	ignoredResponses;

	void	sendHandler(MessagePtr msg, const boost::system::error_code& err);

	void	response(MessagePtr header, const boost::system::error_code& err);
	void	recvHandler(MessagePtr header, uint8 *buf, const boost::system::error_code& err);

	inline void	disconnectCheck(const Message &msg) const;
	inline bool	isInvalidResponse(const Message& msg) const;
	inline bool	isIgnoredResponse(const Message& header) const;
	void		responseHeaderCheck(const Message& res, MsgType expectedType);
	inline void asyncTaskCheck(const boost::system::error_code& err, TaskType taskType, MsgType msgType = MsgType::NO_MSG);
};
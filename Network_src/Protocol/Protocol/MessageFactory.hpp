#pragma once

# include <map>
# include <memory>
# include "MsgWrapper.hpp"

/*
**	A Factory for Terria Messages (client <-> server)
*/
class MessageFactory
{
public:
	std::shared_ptr<MsgWrapper>	makeRequest(MessageType);
	std::shared_ptr<MsgWrapper>	makeRequest(const Message::Header&);

	std::shared_ptr<MsgWrapper>	makeResponse(MessageType);
	std::shared_ptr<MsgWrapper>	makeResponse(const Message::Header&);

private:

	typedef MsgWrapper* (*MessageCreator)(void);
	typedef std::map<MessageType, MessageCreator> CmdRegistry;

	enum class CmdType
	{
		REQUEST,
		RESPONSE
	};

	static const CmdRegistry requestsRegistry;
	static const CmdRegistry responsesRegistry;

	std::shared_ptr<MsgWrapper>	create(MessageType, CmdType);
	std::shared_ptr<MsgWrapper>	create(const Message::Header&, CmdType);
};

#pragma once

# include <map>
# include <memory>
# include "Message.hpp"

/*
**	A Factory for Terria Messages (client <-> server)
*/

typedef std::shared_ptr<Message>	MessagePtr;

class MessageFactory
{
public:
	MessagePtr	makeRequest(MsgType);
	MessagePtr	makeRequest(const Message&);

	MessagePtr	makeResponse(MsgType);
	MessagePtr	makeResponse(const Message&);

private:

	typedef Message* (*MessageCreator)(void);
	typedef std::map<MsgType, MessageCreator> CmdRegistry;

	enum class CmdType
	{
		REQUEST,
		RESPONSE
	};

	static const CmdRegistry requestsRegistry;
	static const CmdRegistry responsesRegistry;

	MessagePtr	create(MsgType, CmdType);
	MessagePtr	create(const Message&, CmdType);
};

#pragma once

# include <map>
# include <memory>
# include "Message.hpp"

/*
**	A Factory for Terria Messages (client <-> server)
*/

enum class TaskType
{
	REQUEST,
	RESPONSE
};

class MsgFactory
{
public:
	MessagePtr	makeRequest(MsgType);
	MessagePtr	makeRequest(const Message&);

	MessagePtr	makeResponse(MsgType);
	MessagePtr	makeResponse(const Message&);

private:

	typedef Message* (*MessageCreator)(void);
	typedef std::map<MsgType, MessageCreator> CmdRegistry;

	static const CmdRegistry requestsRegistry;
	static const CmdRegistry responsesRegistry;

	MessagePtr	create(MsgType, TaskType);
	MessagePtr	create(const Message&, TaskType);
};

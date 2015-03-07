#include <stdexcept>
#include <vector>
#include <string>
#include "MessageFactory.hpp"

/*
**	Makes a new Request with the associated MsgType
*/
MessagePtr	MessageFactory::makeRequest(MsgType msgType)
{
	return create(msgType, CmdType::REQUEST);
}

MessagePtr	MessageFactory::makeRequest(const Message& src)
{
	return create(src, CmdType::REQUEST);
}

MessagePtr	MessageFactory::makeResponse(MsgType msgType)
{
	return create(msgType, CmdType::RESPONSE);
}

/*
**	Makes a new Response with the associated header
**	Header will be assigned in the new Wrapper
*/
MessagePtr	MessageFactory::makeResponse(const Message& src)
{
	return create(src, CmdType::RESPONSE);
}

MessagePtr	MessageFactory::create(MsgType msgType, CmdType cmdType)
{
	const CmdRegistry&	registry = (cmdType == CmdType::REQUEST) ? requestsRegistry : responsesRegistry;
	auto it = registry.find(msgType);

	if (it == registry.end())
	{
		std::string type = (cmdType == CmdType::REQUEST) ? "request" : "response";
		throw(std::logic_error("Invalid " + type + " type."));
	}

	auto* msg = it->second();
	msg->type = static_cast<uint8>(msgType);
	return MessagePtr(msg);
}

MessagePtr	MessageFactory::create(const Message& src, CmdType cmdType)
{
	auto msg = this->create(static_cast<MsgType>(src.type), cmdType);

	*(msg.get()) = src;
	return msg;
}

const MessageFactory::CmdRegistry MessageFactory::requestsRegistry =
{
	{ MsgType::CONNECT, [](void) -> Message * { return new Connect(); } },
	{ MsgType::APPEARANCE, [](void) -> Message * { return new Appearance(); } },
	{ MsgType::PLAYER_LIFE, [](void) -> Message * { return new Life(); } },
	{ MsgType::PLAYER_MANA, [](void) -> Message * { return new Mana(); } },
	{ MsgType::PLAYER_BUFFS, [](void) -> Message * { return new Buffs(); } },
	{ MsgType::INVENTORY, [](void) -> Message * { return new InventoryItem(); } }
};

const MessageFactory::CmdRegistry MessageFactory::responsesRegistry =
{
	{ MsgType::ACCEPTED, [](void) -> Message * { return new Accepted(); } }
};

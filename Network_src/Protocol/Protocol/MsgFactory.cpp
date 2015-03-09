#include <stdexcept>
#include <string>
#include "MsgFactory.hpp"

/*
**	Makes a new Request with the associated MsgType
*/
MessagePtr	MsgFactory::makeRequest(MsgType msgType)
{
	return create(msgType, TaskType::REQUEST);
}

MessagePtr	MsgFactory::makeRequest(const Message& src)
{
	return create(src, TaskType::REQUEST);
}

MessagePtr	MsgFactory::makeResponse(MsgType msgType)
{
	return create(msgType, TaskType::RESPONSE);
}

/*
**	Makes a new Response with the associated header
**	Header will be assigned in the new Wrapper
*/
MessagePtr	MsgFactory::makeResponse(const Message& src)
{
	return create(src, TaskType::RESPONSE);
}

MessagePtr	MsgFactory::create(MsgType msgType, TaskType taskType)
{
	const CmdRegistry&	registry = (taskType == TaskType::REQUEST) ? requestsRegistry : responsesRegistry;
	auto it = registry.find(msgType);

	if (it == registry.end())
	{
		std::string type = (taskType == TaskType::REQUEST) ? "request" : "response";
		throw(std::logic_error("Invalid " + type + " type."));
	}

	auto* msg = it->second();
	msg->type = static_cast<uint8>(msgType);
	return MessagePtr(msg);
}

MessagePtr	MsgFactory::create(const Message& src, TaskType taskType)
{
	auto msg = this->create(static_cast<MsgType>(src.type), taskType);

	*msg = src;
	return msg;
}

const MsgFactory::CmdRegistry MsgFactory::requestsRegistry =
{
	{ MsgType::SPAWN_PLAYER, [](void) -> Message * { return new SpawnPlayer(); } },
};

const MsgFactory::CmdRegistry MsgFactory::responsesRegistry =
{
	{ MsgType::SPAWN, [](void) -> Message * { return new Spawn(); } },
	{ MsgType::TILE_DATA, [](void) -> Message * { return new TileRowData(); } },
	{ MsgType::NPC_UPDATE, [](void) -> Message * { return new NPCUpdate(); } },
	{ MsgType::NPC_NAME, [](void) -> Message * { return new NPCName(); } },
	{ MsgType::ITEM_UPDATE, [](void) -> Message * { return new ItemUpdate(); } },
	{ MsgType::ITEM_OWNER, [](void) -> Message * { return new ItemOwnerSet(); } }
};

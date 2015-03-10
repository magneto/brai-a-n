#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
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

		std::cerr << "Unsupported " << type << " type " << (int)msgType << std::endl;
		//throw(std::logic_error("Invalid " + type + " type."));
		return nullptr;
	}

	auto* msg = it->second();
	msg->type = static_cast<uint8>(msgType);
	return MessagePtr(msg);
}

MessagePtr	MsgFactory::create(const Message& src, TaskType taskType)
{
	auto msg = this->create(static_cast<MsgType>(src.type), taskType);

	if (msg != nullptr) // is implemented
		*msg = src;
	return msg;
}

const MsgFactory::CmdRegistry MsgFactory::requestsRegistry =
{
	{ MsgType::SPAWN_PLAYER, [](void) -> Message * { return new SpawnPlayer(); } },
	{ MsgType::PARTY_CHANGE, [](void) -> Message * { return new ChangeParty(); } },
	{ MsgType::TELEPORT, [](void) -> Message * { return new Teleport(); } },
	{ MsgType::CHAT, [](void) -> Message * { return new Chat(); } },
	{ MsgType::PLAYER_CONTROL, [](void) -> Message * { return new PlayerControl(); } }
};

const MsgFactory::CmdRegistry MsgFactory::responsesRegistry =
{
	{ MsgType::SPAWN, [](void) -> Message * { return new Spawn(); } },
	{ MsgType::CHAT, [](void) -> Message * { return new Chat(); } },
	{ MsgType::TILE_DATA, [](void) -> Message * { return new TileRowData(); } },
	{ MsgType::PLAYER_LIFE, [](void) -> Message * { return new Life(); } },
	{ MsgType::NPC_UPDATE, [](void) -> Message * { return new NPCUpdate(); } },
	{ MsgType::NPC_NAME, [](void) -> Message * { return new NPCName(); } },
	{ MsgType::ITEM_UPDATE, [](void) -> Message * { return new ItemUpdate(); } },
	{ MsgType::ITEM_OWNER, [](void) -> Message * { return new ItemOwnerSet(); } }
};

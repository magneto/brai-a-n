#include <stdexcept>
#include <vector>
#include <string>
#include "MessageFactory.hpp"

const MessageFactory::CmdRegistry MessageFactory::requestsRegistry =
{
	{ MessageType::CONNECT, [](void) -> MsgWrapper * { return new ConcreteMsgWrapper<Connect>(); } }
};

const MessageFactory::CmdRegistry MessageFactory::responsesRegistry =
{
	{ MessageType::ACCEPTED, [](void) -> MsgWrapper * { return new ConcreteMsgWrapper<Accepted>(); } }
};


std::shared_ptr<MsgWrapper>	MessageFactory::create(MessageType msgType, CmdType cmdType)
{
	const CmdRegistry&	registry = (cmdType == CmdType::REQUEST) ? requestsRegistry : responsesRegistry;
	auto it = registry.find(msgType);

	if (it == registry.end())
	{
		std::string type = (cmdType == CmdType::REQUEST) ? "request" : "response";
		throw(std::logic_error("Invalid " + type + " type."));
	}

	auto* msgWrap = it->second();
	msgWrap->genericMsg->header.type = static_cast<byte>(msgType);
	return std::shared_ptr<MsgWrapper>(msgWrap);
}

std::shared_ptr<MsgWrapper>	MessageFactory::create(const Message::Header& header, CmdType cmdType)
{
	auto msgWrap = this->create(static_cast<MessageType>(header.type), cmdType);

	msgWrap->genericMsg->header = header;
	return msgWrap;
}

/*
**	Makes a new Request with the associated MessageType
*/
std::shared_ptr<MsgWrapper>	MessageFactory::makeRequest(MessageType msgType)
{
	return create(msgType, CmdType::REQUEST);
}

std::shared_ptr<MsgWrapper>	MessageFactory::makeRequest(const Message::Header& header)
{
	return create(header, CmdType::REQUEST);
}

std::shared_ptr<MsgWrapper>	MessageFactory::makeResponse(MessageType msgType)
{
	return create(msgType, CmdType::RESPONSE);
}

/*
**	Makes a new Response with the associated header
**	Header will be assigned in the new Wrapper
*/
std::shared_ptr<MsgWrapper>	MessageFactory::makeResponse(const Message::Header& header)
{
	return create(header, CmdType::RESPONSE);
}

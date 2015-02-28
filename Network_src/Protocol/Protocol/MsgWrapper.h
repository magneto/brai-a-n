#pragma once

# include <memory>
# include "Message.h"

class MessageFactory;

class MsgWrapper
{
	friend class MessageFactory;
protected:
	std::shared_ptr<Message>	genericMsg;
public:

	MsgWrapper() : genericMsg(nullptr) {}

	virtual ~MsgWrapper() = default;

	virtual void	netToHost() = 0;
	virtual void	hostToNet() = 0;

	unsigned char*	getStream()
	{
		return reinterpret_cast<unsigned char*>(genericMsg.get());
	}

	std::size_t	getSize() const
	{
		return (genericMsg->header.length + sizeof(genericMsg->header.length));
	}
};

template<typename T>
class ConcreteMsgWrapper : public MsgWrapper
{
public:

	std::shared_ptr<T>	msg;

	ConcreteMsgWrapper() : msg(std::make_shared<T>())
	{
		genericMsg = msg;
		msg->header.length = sizeof(T) - sizeof(msg->header.length);
	}

	virtual void	hostToNet()
	{
		msg->Message::hostToNet();

		if (&T::hostToNet != &Message::hostToNet)
		{
			msg->hostToNet();
		}
	}

	virtual void	netToHost()
	{
		msg->Message::netToHost();

		if (&T::netToHost != &Message::netToHost)
		{
			msg->netToHost();
		}
	}
};

namespace Requests
{
	typedef ConcreteMsgWrapper<Connect>		Connect;
}

namespace Responses
{
	typedef ConcreteMsgWrapper<Accepted>	Accept;
}


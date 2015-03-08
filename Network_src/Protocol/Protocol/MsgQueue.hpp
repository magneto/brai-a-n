#pragma once

# include <queue>
# include "Message.hpp"

class MsgQueue
{
public:
	void		push(MessagePtr msg);
	MessagePtr	pop();

private:
	std::queue<MessagePtr>	queue;
};
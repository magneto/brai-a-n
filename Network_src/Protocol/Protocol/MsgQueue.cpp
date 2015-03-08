#include "MsgQueue.hpp"

void		MsgQueue::push(MessagePtr msg)
{
	queue.push(msg);
}

MessagePtr	MsgQueue::pop()
{
	MessagePtr	ret = queue.front();
	queue.pop();
	return ret;
}
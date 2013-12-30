#include "CommandQueue.h"

void CommandQueue::push(const Command& command)
{
    mQueue.push(command);
}
Command CommandQueue::pop()
{
    Command res = mQueue.front();
    mQueue.pop();
    return res;
}
bool CommandQueue::isEmpty() const
{
    return (mQueue.size() == 0);
}

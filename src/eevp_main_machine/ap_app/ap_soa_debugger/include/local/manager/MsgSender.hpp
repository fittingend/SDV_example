#ifndef __MSG_SENDER_HPP__
#define __MSG_SENDER_HPP__

#include <Common.h>

class MsgSender
{
private:

public:
    virtual void SendMessage(std::string Message) {}
};

#endif
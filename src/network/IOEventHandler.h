#ifndef _IOEVENT_HANDLER_H_
#define _IOEVENT_HANDLER_H_

#include "Message.h"

class IOEventReceiver
{
    public:
        void initSender(IOEventSender* sender){mSender = sender;}

        virtual void onRecv(Message* message) = 0;

        void send(Message* message)
        {
            if(mSender)
            {
                mSender->send(message);
            }
        }
    protected:
        IOEventSender* mSender;
};


class IOEventSender
{
    public:
        virtual void send(Message* message);
};


#endif

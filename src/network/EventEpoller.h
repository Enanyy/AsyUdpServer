#ifndef _EVENT_EPOLLER_H_
#define _EVENT_EPOLLER_H_

#include "IOEvent.h"

#include <sys/epoll.h>

class EventEpoller : public IOEvent
{
    public:
        EventEpoller();
        ~EventEpoller();

        virtual void close();
        
        virtual void update();

        virtual bool registerEndPoint(int fd, EndPoint* endpoint);
        virtual bool unRegisterEndPoint(int fd, EndPoint* endpoint);

   private:
        int mFD;
};

#endif

#ifndef _EPOLL_POLLER_H_
#define _EPOLL_POLLER_H_

#include "EventPoller.h"

#include <sys/epoll.h>

class EpollPoller : public EventPoller
{
    public:
        EpollPoller();
        ~EpollPoller();

        virtual void close();
        
        virtual void update();

        virtual bool registerEndPoint(int fd, EndPoint* endpoint);
        virtual bool unRegisterEndPoint(int fd, EndPoint* endpoint);

   private:
        int mFD;
};

#endif

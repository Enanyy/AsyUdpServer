#ifndef _KQUEUE_POLLER_H_
#define _KQUEUE_POLLER_H_

#include "EventPoller.h"

class KqueuePoller: public EventPoller
{
    public:
        KqueuePoller();
        ~KqueuePoller();

    public:
        virtual void update();
};


#endif

#ifndef _SELECT_POLLER_H_
#define _SELECT_POLLER_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>

#include "EventPoller.h"

class SelectPoller: public EventPoller
{
    public:
        virtual void update();

};

#endif

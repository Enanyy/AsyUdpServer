#ifndef _EVENT_KQUEUE_H_
#define _EVENT_KQUEUE_H_

#include "IOEvent.h"

class EventKqueue: public IOEvent
{
    public:
        EventKqueue();
        ~EventKqueue();

    public:
        virtual void update();
};


#endif

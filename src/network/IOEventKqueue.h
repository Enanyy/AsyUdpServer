#ifndef _IOEVENT_KQUEUE_H_
#define _IOEVENT_KQUEUE_H_

#include "IOEvent.h"

class IOEventKqueue: public IOEvent
{
    public:
        IOEventKqueue();
        ~IOEventKqueue();

    public:
        virtual void update();
};


#endif

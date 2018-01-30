#ifndef _EVENT_SELECTOR_H_
#define _EVENT_SELECTOR_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>

#include "IOEvent.h"

class EventSelector: public IOEvent
{
    public:
        virtual void update();

};

#endif

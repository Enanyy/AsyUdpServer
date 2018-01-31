#ifndef _IOEVENT_SELECTOR_H_
#define _IOEVENT_SELECTOR_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>

#include "IOEvent.h"

class IOEventSelector: public IOEvent
{
    public:

        IOEventSelector();
        ~IOEventSelector();

        virtual void update();

};

#endif

#ifndef _FRAME_MODULE_H_
#define _FRAME_MODULE_H_

#include "Module.h"
#include "MessageReceiver.h"
#include "MessageDispatch.h"

class Module;
class MessageDispatch;
class FrameModule: public Module
{
    public:
        FrameModule();
        virtual ~FrameModule();
        virtual void onRegister();
        virtual void onUnRegister();

    public:
        void onRecvReady(User* u, const MessageID messageId, const char* message, const unsigned int length);


};

#endif

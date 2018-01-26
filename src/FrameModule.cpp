#include "FrameModule.h"
#include "protocpp/scene.pb.h"
#include "protocpp/action.pb.h"

FrameModule:: FrameModule()
{

}

FrameModule:: ~FrameModule()
{
    onUnRegister();
}

void FrameModule::onRegister()
{
    MessageDispatch::getMe()->registerReceiver(this, MessageID::GM_READY_CS, &FrameModule::onRecvReady);
}

void FrameModule::onUnRegister()
{ 
    MessageDispatch::getMe()->unRegisterReceiver(this, MessageID::GM_READY_CS, &FrameModule::onRecvReady);
}

void FrameModule::onRecvReady(User* u, const MessageID messageId, const char* message, const unsigned int length)
{
    if(message== NULL)
    {
        printf("FrameModule::onRecvReady message == NULL.\n");
        return;
    }
    RECV_MSG(msg, GM_Ready, message);

    printf("FrameModule::onRecvReady GM_READY_CS:roleid=%d\n", msg.roleid());

    SEND_TCP(GM_READY_BC, msg, u);

}



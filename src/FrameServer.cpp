#include "FrameServer.h"
#include "MessageDispatch.h"
#include "protocpp/scene.pb.h"
#include "MessageID.h"

FrameServer:: FrameServer()
{
}


FrameServer:: ~FrameServer()
{
}
void FrameServer::update()
{
}

void FrameServer::onStart()
{
    printf("FrameServer::onStart\n");

}

void FrameServer:: onConnect(EndPoint* endpoint)
{
    if(endpoint == NULL)
    {
        return ;
    }

    printf("FrameServer::onConnect client connected:%d.\n",endpoint->getFD());

    static int roleid = 10000;

    User *u = new User(this, endpoint);

    mUserList.push_back(u);


    GM_Connect send;
    send.set_roleid(u->getUserID());
    send.set_clientid(u->getUserID());
    send.set_type(1);

    SEND_TCP(GM_CONNECT_BC, send, u);

    u->online();
}

void FrameServer:: onMessage(Message* message)
{
    if(message)
    {
        MessageID messageId = (MessageID)message->getPacket()->getPacketID();
        User* u = getUser(message->getEndPoint()->getFD());

        MessageDispatch::getMe()->dispatch( u, messageId, message->getPacket()->getPacketBody(), message->getPacket()->getPacketBodyLength() );
    
        printf("FrameServer::onMessage->messageid:%d,\n", (int)messageId);
    }
}

void FrameServer:: onDisconnect(EndPoint* endpoint)
{
    if(endpoint == NULL)
    {
        return ;
    }

    User * u = getUser(endpoint->getFD());

    if(u)
    {
        printf("FrameServer::onDisconnect user %d disconnect.\n",endpoint->getFD());

        u->offline();

        SAFE_DELETE(u);
    }
}

User* FrameServer::getUser(const int userId) 
{
    auto it = mUserList.cbegin();
    while(it!=mUserList.cend())
    {
        User* u = (*it);
        if(u && u->getUserID() == userId)
        {
            return u;
        }
        ++it;
    }
    return NULL;
}


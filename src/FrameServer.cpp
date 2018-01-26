#include "FrameServer.h"
#include "MessageDispatch.h"
#include "protocpp/scene.pb.h"
#include "MessageID.h"

FrameServer:: FrameServer()
{
}

void FrameServer:: start()
{
    printf("Frameserver::start.\n");
    mServer.registerStart(DelegateFactory::create(this,&FrameServer::onStart));
    mServer.registerConnect(DelegateFactory::create(this,&FrameServer::onConnect));
    mServer.registerMessage(DelegateFactory::create(this,&FrameServer::onMessage));
    mServer.registerDisconnect(DelegateFactory::create(this,&FrameServer::onDisconnect));

    mServer.start("127.0.0.1", TCP_PORT, UDP_PORT);
}

void FrameServer::close()
{
    mServer.close();
}

FrameServer:: ~FrameServer()
{
    close();
}

void FrameServer::run()
{
    while(true)
    {
        update();
    }
}

void FrameServer::update()
{
    mServer.update();   
}

void FrameServer::onStart(void* param)
{
    printf("FrameServer::onStart\n");

}

void FrameServer:: onConnect(void* param)
{
    if(param == NULL)return;

    Client * client = static_cast<Client*>(param);

    if(client== NULL)return;

    printf("FrameServer::onConnect client connected:%d.\n",client->getEndPoint()->getFD());

    static int roleid = 10000;

    User *u = new User(roleid++, client);

    mUserList.push_back(u);


    GM_Connect send;
    send.set_roleid(u->getUserID());
    send.set_clientid(u->getClientID());
    send.set_type(1);

    SEND_TCP(GM_CONNECT_BC, send, u);

    u->online();
}

void FrameServer:: onMessage(void* param)
{
    MessageInfo* message = static_cast<MessageInfo*>(param);
    if(message)
    {
        MessageID messageId = (MessageID)message->getPacketID();
        User* u = getUser(message->getClient());

        MessageDispatch::getMe()->dispatch( u, messageId, message->getPacket()->getPacketBody(), message->getPacket()->getPacketBodyLength() );
    
        printf("FrameServer::onMessage->messageid:%d,\n", (int)messageId);
    }
}

void FrameServer:: onDisconnect(void* param)
{
    if(param==NULL)return;

    Client* client = static_cast<Client*>(param);

    if(client)
    {
        printf("FrameServer::onDisconnect client %d disconnect.\n",client->getEndPoint()->getFD());

        User* u = getUser(client);
        if(u)u->offline();

        SAFE_DELETE(u);
    }
}


void FrameServer::sendFrame()
{
    while(true)
    {


        usleep(100);
    }
}

void* FrameServer::sendFrameThread(void* param)
{
    FrameServer* frameserver = static_cast<FrameServer*>(param);
    if(frameserver)
    {
        frameserver->sendFrame();
    }
    return NULL;
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

User* FrameServer:: getUser(Client* client)
{
    if(client == NULL)return NULL;

    auto it = mUserList.cbegin();
    while(it!=mUserList.cend())
    {
        User* u = (*it);
        if(u && u->getClientID() == client->getEndPoint()->getFD())
        {
            return u;
        }
        ++it;
    }
    return NULL;

}

#include "AsycUdpServer.h"
AsycUdpServer:: AsycUdpServer()
{
    mAvaible = false;
}

AsycUdpServer::~AsycUdpServer()
{
    release(); 
}

void AsycUdpServer::release()
{
    
    for(auto it = mClientList.cbegin(); it != mClientList.cend(); ++it)
    {
        Client* c = (*it);
        if(c)
        {
            c->release();
            SAFE_DELETE(c);
        }
    }

    mClientList.clear();

    mTcpServer.close();
    mUdpServer.close();

    mOnStart.clear();
    mOnConnect.clear();
    mOnMessage.clear();
    mOnDisconnect.clear();
    mOnDebug.clear();

}

bool AsycUdpServer:: initTcp(const char* ip, const unsigned int port)
{
    if(mTcpServer.start(this,20, ip, port) == false)
    {
        printf("tcp start fail.\n");
        return false;
    }

    return true;
}

bool AsycUdpServer:: initUdp(const unsigned int port)
{
    int fd = mUdpServer.socket(SOCKET_TYPE::UDP);
    if( fd < 0)
    {
        return false;
    }
    if(mUdpServer.bind(NULL, port) < 0)
    {
        return false;
    }

    ThreadPool::getMe()->addTask<AsycUdpServer>(this, &AsycUdpServer::onRecvfrom, NULL);

    ThreadPool::getMe()->addTask<AsycUdpServer>(this, &AsycUdpServer::onSendto, NULL);

    return true;
}

void AsycUdpServer::start(const char* ip, const int tcpport, const int udpport)
{

    if(initTcp(ip, tcpport)== false)
    {
        printf("initTcp failed.\n");
        exit(0);
    }
    
    printf("initTcp success.\n");

    if(initUdp(udpport) == false)
    {
        printf("initUdp failed.\n");
        exit(0);
    }

    printf("initUdp success.\n");

    mOnStart();

    mAvaible = true;
}

void AsycUdpServer::close()
{
    mTcpServer.close();
    mUdpServer.close();
    mAvaible = false;
}


void AsycUdpServer::update()
{
    //printf("AsycUdpServer::update->size=%d\n",mReceiveMessageQueue.size());
    if(mAvaible == false)
    {
        return ;
    }

    mTcpServer.update();
            
    while(mReceiveMessageQueue.size() > 0)
    {

        MessageInfo *message = mReceiveMessageQueue.front();
        if(message && message->getPacketID() >MessageID::MinValue && message->getPacketID() < MessageID::MaxValue)
        {
             mOnMessage((void*)message);
        }
        mReceiveMessageQueue.pop();
        
        SAFE_DELETE(message);    
    }



    while(mConnectQueue.size() >0)
    {
        Client* client = mConnectQueue.front();
        if(client)client->onConnect();
        mOnConnect((void*)client);
        mConnectQueue.pop();
    }


    while(mDisconnectQueue.size() > 0)
    {
        Client* client = mDisconnectQueue.front();
        if(client)client->onDisconnect();
        mOnDisconnect((void*)client);
        mDisconnectQueue.pop();
        if(client)
        {
            client->release();
            SAFE_DELETE(client);
        }
    }

}

void AsycUdpServer::sendto(MessageInfo* message)
{
    if(message == NULL)
    {
        return;
    } 

    printf("AsycUdpServer::sendUdp client %d cache udp packet id = %d.\n",message->getClient()->getEndPoint()->getFD(), message->getPacket()->getPacketID());

   if(mUdpServer.isValid())
   {
       mSendToMessageQueue.push(message);
   }
}


void AsycUdpServer::onConnect(EndPoint* endpoint )
{
    if(endpoint == NULL)
    {
        return;
    }

    printf("AsycUdpServer::onConnect client connect fd=%d ip=%s port=%d.\n",endpoint->getFD(), endpoint->getHostIP(), endpoint->getHostPort() );
    
    Client* c = new Client(this, endpoint);

    mClientList.push_back(c);

    mConnectQueue.push(c);

}

void AsycUdpServer::onRecv(EndPoint* endpoint, Packet* packet)
{

}

void AsycUdpServer::onDisconnect(EndPoint* endpoint)
{
    if(endpoint==NULL)
    {
        return ;
    }

    Client* c = getClient(endpoint);
    if(c)
    {
        c->onDisconnect();

        SAFE_DELETE(c);
    }
}

void AsycUdpServer::onClose()
{
    auto it = mClientList.begin();
    while(it != mClientList.end())
    {
        Client* c = (*it);
        if(c)
        {
            c->onDisconnect();
        }

        SAFE_DELETE(c);
        c = NULL;
    }
    mClientList.clear();
}

void AsycUdpServer::onError(EndPoint* endpoint)
{
    if(endpoint==NULL)
    {
        return;
    }

    printf("AsycUdpServer::onError %s:%d\n",endpoint->getHostIP(), endpoint->getHostPort());
}

void AsycUdpServer::onRecvfrom(void* arg)
{
   while(true)
   {
        Packet* packet = new Packet();
        sockaddr_in from;
        int length = packet->recvfrom(&mUdpServer, from);
        if(length >= Packet::PACKET_HEAD_LEGNTH)
        {

            printf("AsycUdpServer::onRecvfrom recv from udp ip=%s port=%d id=%d.\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port), packet->getPacketID());
            Client* client = getClient(from.sin_addr.s_addr, from.sin_port);
            if(client)
            {
                mReceiveMessageQueue.push(new MessageInfo(packet, client));
            }
            else
            {
                SAFE_DELETE(client);
            }
        }
        else
        {
            SAFE_DELETE(packet);
        }
   }
}

void AsycUdpServer::onSendto(void* arg)
{
    while(true)
    {
        if(mSendToMessageQueue.size() > 0)
        {
            MessageInfo* message = mSendToMessageQueue.front();

            if(message)
            {

            }

            SAFE_DELETE(message);
        }
    }
}

void AsycUdpServer:: clientDisconnect(Client * client)
{
    if(!client)return;
    

    for(auto it = mClientList.begin(); it != mClientList.end(); ++it)
    {
        if(client == (*it))
        {
            mClientList.erase(it);
            break;
        }
    }

    mDisconnectQueue.push(client);

}


Client* AsycUdpServer::getClient(EndPoint* endpoint)
{
    Client* client = NULL;
   if(endpoint == NULL)
   {
       return client;
   }
    for(auto it = mClientList.cbegin(); it != mClientList.cend(); ++it)
    {
        Client * c = (*it);
        if(c && c->getEndPoint() == endpoint)
        {
            client = c;
            break;
        }
    }

    return client;
}

Client* AsycUdpServer::getClient(const unsigned long ip, const unsigned int port)
{
    Client * client = NULL;

    for(auto it = mClientList.cbegin(); it != mClientList.cend(); ++it)
    {
        Client* c = (*it);
        if(c && c->getEndPoint())
        {
            if(c->getEndPoint()->getIP() == ip && c->getEndPoint()->getPort() == port)
            {
                client = c;
                break;
            }
        }
    }
    return client;
}

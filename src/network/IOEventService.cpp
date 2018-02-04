#include "IOEventService.h"

void IOEventHandler::send(EndPoint* endpoint, Packet* packet)
{
    if(mService)
    {
        mService->send(endpoint, packet);
    }
}

void IOEventHandler::sendto(EndPoint* endpoint, Packet* packet)
{
    if(mService)
    {
        mService->sendto(endpoint,packet);
    }
}

IOEventService:: IOEventService():mEventHandler(NULL),
                                  mInited(false),
                                  mTcpServer(),
                                  mUdpServer(),
                                  mMessageQueue(),
                                  mSendToQueue(),
                                  mConnectQueue(),
                                  mDisconnectQueue()
{
    
}

IOEventService::~IOEventService()
{
    mEventHandler = NULL;
    close();
}

bool IOEventService::init(IOEventHandler* handler)
{
    mEventHandler = handler;
    
    if(mEventHandler == NULL)
    {
        return false;
    }

    mEventHandler->init(this);

    return true;

}

bool IOEventService:: initTcp(const char* ip, const unsigned int port)
{
    if(mTcpServer.start(this,20, ip, port) == false)
    {
        printf("tcp start fail.\n");
        return false;
    }

    return true;
}

bool IOEventService:: initUdp(const unsigned int port)
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

//    ThreadPool::getMe()->addTask<IOEventService>(this, &IOEventService::onRecvfrom, NULL);

//    ThreadPool::getMe()->addTask<IOEventService>(this, &IOEventService::onSendto, NULL);

    return true;
}

void IOEventService::start(const char* ip, const int tcpport, const int udpport)
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

    if(mEventHandler)
    {
        mEventHandler->onStart();
    }

    mInited = true;
}

void IOEventService::close()
{
    mTcpServer.close();
    mUdpServer.close();
}

void IOEventService::run()
{
    if(!mInited)
    {
        return;
    }

    if(mEventHandler == NULL)
    {
        return ;
    }

    while(true)
    {

        if(mTcpServer.isValid())
        {
            mTcpServer.update();
        }

        if(mEventHandler)
        {
            mEventHandler->update();
        }

        while(mConnectQueue.size() > 0)
        {
            
            EndPoint* endpoint = mConnectQueue.front();

            if(mEventHandler)
            {
                mEventHandler->onConnect(endpoint);
            }
            mConnectQueue.pop();

        }

        
        while(mMessageQueue.size() > 0)
        {
            Message* message = mMessageQueue.front();
            if(mEventHandler)
            {
                mEventHandler->onMessage(message);
            }
            mMessageQueue.pop();

            SAFE_DELETE(message);
        }

        while(mDisconnectQueue.size() > 0)
        {
            EndPoint* endpoint = mDisconnectQueue.front();
            if(mEventHandler)
            {
                mEventHandler->onDisconnect(endpoint);
            }
            SAFE_DELETE(endpoint);
        }
    }
}

void IOEventService::sendto(EndPoint* endpoint, Packet* packet)
{
    if(endpoint && packet)
    {
        mSendToQueue.push(new Message(endpoint, packet));
    }
}

void IOEventService::onConnect(EndPoint* endpoint )
{
    if(endpoint == NULL)
    {
        return;
    }

    printf("IOEventService::onConnect fd=%d ip=%s port=%d.\n",endpoint->getFD(), endpoint->getHostIP(), endpoint->getHostPort() );
    
    mConnectQueue.push(endpoint);
}

void IOEventService::onRecv(EndPoint* endpoint, Packet* packet)
{
    printf("IOEventService:: onRecv\n");

    if(endpoint == NULL || packet == NULL)
    {
        return ;
    }

    printf("IOEventService::onRecv recv from tcp ip=%s port=%d id=%d.\n",endpoint->getHostIP(), endpoint->getHostPort(), packet->getPacketID());
    mMessageQueue.push(new Message(endpoint,packet));
}


void IOEventService::onDisconnect(EndPoint* endpoint)
{
    if(endpoint==NULL)
    {
        return ;
    }

    mDisconnectQueue.push(endpoint);

}

void IOEventService::onClose()
{
    
}

void IOEventService::onError(EndPoint* endpoint)
{
    if(endpoint==NULL)
    {
        return;
    }

    printf("IOEventService::onError %s:%d\n",endpoint->getHostIP(), endpoint->getHostPort());
}

void IOEventService::onRecvfrom(void* arg)
{
   while(true)
   {
        Packet* packet = new Packet();
        sockaddr_in from;
        int length = packet->recvfrom(&mUdpServer, from);
        if(length >= Packet::PACKET_HEAD_LENGTH)
        {

            printf("IOEventService::onRecvfrom recv from udp ip=%s port=%d id=%d.\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port), packet->getPacketID());
         
            EndPoint* endpoint = getEndPoint(from.sin_addr.s_addr, from.sin_port);

            mMessageQueue.push(new Message(endpoint, packet));
        }
        else
        {
            SAFE_DELETE(packet);
        }
   }
}

void IOEventService::onSendto(void* arg)
{
}



#include "IOEventService.h"

IOEventService:: IOEventService():mInited(false),mTcpServer(),mUdpServer()
{
    
}

IOEventService::~IOEventService()
{
    close();
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

    //ThreadPool::getMe()->addTask<IOEventService>(this, &IOEventService::onRecvfrom, NULL);

    //ThreadPool::getMe()->addTask<IOEventService>(this, &IOEventService::onSendto, NULL);

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

    if(mTcpServer.isValid())
    {
        mTcpServer.update();
    }


}

void IOEventService::onConnect(EndPoint* endpoint )
{
    if(endpoint == NULL)
    {
        return;
    }

    printf("IOEventService::onConnect fd=%d ip=%s port=%d.\n",endpoint->getFD(), endpoint->getHostIP(), endpoint->getHostPort() );
    


}

int IOEventService::onRecv(EndPoint* endpoint)
{
    printf("IOEventService:: onRecv\n");

    if(endpoint == NULL)
    {
        return 0;
    }

    Packet* packet = new Packet();
    printf("IOEventService::onRecv %d Begin recv.\n", endpoint->getFD());
    int result = packet->recv(endpoint);
    printf("IOEventService::onRecv %d End recv.\n", endpoint->getFD());
    if(result >= Packet:: PACKET_HEAD_LEGNTH)
    {
        printf("IOEventService::onRecv recv from tcp ip=%s port=%d id=%d.\n",endpoint->getHostIP(), endpoint->getHostPort(), packet->getPacketID());

    }
    else
    {
        SAFE_DELETE(packet);
    }
    return result;
}

int IOEventService::onSend(EndPoint* endpoint)
{
}

void IOEventService::onDisconnect(EndPoint* endpoint)
{
    if(endpoint==NULL)
    {
        return ;
    }

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
/*
void IOEventService::onRecvfrom(void* arg)
{
   while(true)
   {
        Packet* packet = new Packet();
        sockaddr_in from;
        int length = packet->recvfrom(&mUdpServer, from);
        if(length >= Packet::PACKET_HEAD_LEGNTH)
        {

            printf("IOEventService::onRecvfrom recv from udp ip=%s port=%d id=%d.\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port), packet->getPacketID());
            
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
*/


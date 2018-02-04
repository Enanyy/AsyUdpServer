#include "IOEvent.h"

void IOEventListener::send(EndPoint* endpoint, Packet* packet)
{
    if(mIOEvent)
    {
        mIOEvent->send(endpoint, packet);    
    }
}

EndPoint* IOEventListener::getEndPoint(int fd)
{
    if(mIOEvent)
    {
        return mIOEvent->getEndPoint(fd);
    }
    return NULL;
}

EndPoint* IOEventListener::getEndPoint(unsigned long ip, unsigned int port)
{
    if(mIOEvent)
    {
        return mIOEvent->getEndPoint(ip, port);
    }
    return  NULL;
}


IOEvent:: IOEvent():mServer(),mListener(NULL),mEndPoints() ,mBackLog(10), mPackets()
{

}

IOEvent::~IOEvent()
{

}


bool IOEvent::start(IOEventListener* listener, const int backlog, const char* ip, const unsigned int port)
{
    mListener   = listener;
    mBackLog    = backlog;

    if(mListener)
    {
        mListener->init(this);
    }

    int fd      = mServer.socket(SOCKET_TYPE::TCP);
    if(fd < 0)
    {
        return false;
    }

    if(mServer.bind(ip, port) < 0)
    {
        return false;
    }

    if(mServer.listen(backlog) < 0)
    {
        return false;
    }

    return registerEndPoint (fd, &mServer);
}

void IOEvent::close()
{
    mServer.close();
    mListener = NULL;
    
    auto it = mEndPoints.begin();
    for(; it != mEndPoints.end(); ++it)
    {
        EndPoint* endpoint = it->second;
        
        if(endpoint)
        {
            delete endpoint;
            endpoint = NULL;
        }
    }
}


bool IOEvent::registerEndPoint(int fd, EndPoint* endpoint)
{
    if(endpoint == NULL)
    {
        return false;
    }

    auto it = mEndPoints.find(fd);
    if(it != mEndPoints.end())
    {
        return false;
    }

    mEndPoints[fd] = endpoint;
    
    endpoint->setnonblocking(true);
    endpoint->setnodelay(true);

    printf("register endpoint :%d.\n",fd);

    return true;

}

bool IOEvent::unRegisterEndPoint(int fd, EndPoint* endpoint)
{
    auto it = mEndPoints.find(fd);
    if(it != mEndPoints.end())
    {
        mEndPoints.erase(fd);
 
        return true;
    }
    return false;
}

void IOEvent::send(EndPoint*  endpoint, Packet* packet)
{
    if(endpoint == NULL || packet == NULL) return;

    int fd = endpoint->getFD();

    auto it = mPackets.find(fd);

    if(it == mPackets.end())
    {
        std::queue<Packet* > packets;
        mPackets[fd] = packets;
    }

    mPackets[fd].push(packet);
}

EndPoint* IOEvent::getEndPoint(const int fd)
{
    auto it = mEndPoints.find(fd);
    if(it != mEndPoints.end())
    {
        return it->second;
    }
    return NULL;
}


EndPoint* IOEvent::getEndPoint(unsigned long ip, unsigned int port)
{
    for(auto it = mEndPoints.begin(); it != mEndPoints.end(); ++it)
    {
        if(it->second->getIP() == ip && it->second->getPort() == port)
        {
            return it->second;
        }
    }

    return NULL;
}


void IOEvent::onAccept(const int fd)
{
    if(fd == mServer.getFD())
    {
        EndPoint* endpoint = mServer.accept();
        if(endpoint != NULL)
        {
            registerEndPoint(endpoint->getFD(), endpoint);
            if(mListener != NULL)
            {
                mListener->onConnect(endpoint);
            }
        }
    }
}

void IOEvent::onRecv(const int fd)
{
    EndPoint* endpoint = getEndPoint(fd);
    if(endpoint != NULL)
    {
        Packet* packet = new Packet();
        printf("IOEvent::onRecv %d Begin recv.\n", endpoint->getFD());
        int result = packet->recv(endpoint);
        printf("IOEvent::onRecv %d End recv.\n", endpoint->getFD());
        if(result >= Packet:: PACKET_HEAD_LENGTH)
        {
            printf("IOEvent::onRecv recv from tcp ip=%s port=%d id=%d.\n",endpoint->getHostIP(), endpoint->getHostPort(), packet->getPacketID());
           
            if(mListener!= NULL && packet !=NULL)
            {
                mListener->onRecv(endpoint, packet);
            }
        }
        else
        {
            delete packet;
            packet = NULL;

            if(result < 0)
            {
               onDisconnect(fd);
            }
        }
    }
}

void IOEvent::onSend(const int fd)
{
    EndPoint* endpoint = getEndPoint(fd);

    if(endpoint == NULL)
    {
        return;
    }

    auto it = mPackets.find(fd);

    if(it == mPackets.end())
    {
        return;
    }

    bool success = true;

    while(it->second.size()  > 0)
    {
        Packet* packet = it->second.front();
        if(packet)
        {
            int result = endpoint->send(packet->data(), packet->length());

            if(result < 0)
            {
                success = false;
            }
        }

        it->second.pop();
        delete packet;
        packet = NULL;
    }

    if(success == false)
    {
        onDisconnect(fd);
    }

}

void IOEvent::onError(const int fd)
{
   EndPoint* endpoint = getEndPoint(fd);
   if(endpoint!=NULL)
   {
       endpoint->close();
       onDisconnect(fd);

       if(mListener!=NULL)
       {
           mListener->onDisconnect(endpoint);
           mListener->onError(endpoint);
       }

       mEndPoints.erase(fd);

       if(endpoint)
       {
            delete endpoint;
            endpoint = NULL;
       }
   }
}

void IOEvent::onDisconnect(const int fd)
{
    EndPoint* endpoint = getEndPoint(fd);

    if(endpoint!=NULL)
    {
        endpoint->close();
        if(mListener!=NULL)
        {
            mListener->onDisconnect(endpoint);
        }

        mEndPoints.erase(fd);

        if(endpoint)
        {
            delete endpoint;
            endpoint = NULL;
        }
    }
}

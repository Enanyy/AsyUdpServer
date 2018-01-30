#include "IOEvent.h"

IOEvent:: IOEvent():mServer(),mListener(NULL),mBackLog(10)
{

}

IOEvent::~IOEvent()
{

}


bool IOEvent::start(EventListener* listener, const int backlog, const char* ip, const unsigned int port)
{
    mListener   = listener;
    mBackLog    = backlog;

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


EndPoint* IOEvent::getEndPoint(const int fd)
{
    auto it = mEndPoints.find(fd);
    if(it != mEndPoints.end())
    {
        return it->second;
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
        if(mListener!= NULL)
        {
           if( mListener->onRecv(endpoint) < 0)
           {
               onDisconnect(fd);
           }
        }
    }
}

void IOEvent::onSend(const int fd)
{
    EndPoint* endpoint = getEndPoint(fd);
    if(endpoint != NULL)
    {
        if(mListener!=NULL)
        {
           if( mListener->onSend(endpoint) < 0)
           {
               onDisconnect(fd);
           }
        }
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
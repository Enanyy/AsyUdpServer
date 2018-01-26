#include "EndPoint.h"

EndPoint::EndPoint():mFD(-1),mIP(0),mPort(0)
{
}

EndPoint::EndPoint(char* ip, unsigned int port):mFD(-1), mIP(inet_addr(ip)), mPort(htons(port))
{
}

EndPoint::~EndPoint()
{

}

char* EndPoint::getHostIP()
{
    struct in_addr addr;
    addr.s_addr = mIP;
    return inet_ntoa(addr);
}

int EndPoint::socket(SOCKET_TYPE type)
{
    mFD = ::socket(type == SOCKET_TYPE::TCP? AF_INET:PF_INET, type == SOCKET_TYPE::TCP? SOCK_STREAM:SOCK_DGRAM, 0 );
    return mFD;
}

void EndPoint::close()
{
    ::close(mFD);
}

int EndPoint::send(const void* data, const int length)
{
    if(data == NULL)
    {
        return 0;
    }

    int size        = length;
    void* buffer    = const_cast<void*>(data);

    while(size > 0)
    {
        int result = ::send(mFD, buffer, size, 0);
        if(result <= 0)
        {
            return result;
        }

        size    -= result;
        buffer  += result;
    }
    return length -size;

}

int EndPoint::recv(void* data ,const int length)
{
    if(data == NULL)
    {
        return 0;
    }

    int size        = length;
    void* buffer    = const_cast<void*>(data);

    while(size > 0)
    {
        int result = ::recv(mFD, buffer, size, 0);
        if(result <= 0)
        {
            return result;
        }

        size    -= result;
        buffer  += result;
    }

    return length - size;
}

void EndPoint::sendto(const void* data, const int length)
{
    if(data == NULL)
    {
        return ;
    }
    struct sockaddr_in sin;
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = mIP;
    sin.sin_port        = mPort;

    int result = ::sendto(mFD, data, length, 0, (const struct sockaddr*)&sin, sizeof(sin));
}

int EndPoint::recvfrom(void* data, const int length, sockaddr_in& from)
{
    memset(&from,0, sizeof(from));
    
    size_t len = sizeof(sockaddr_in);

    int result = ::recvfrom(mFD, data, length, 0, (struct sockaddr*)&from, (socklen_t*)&len);

    return result;
}

EndPoint* EndPoint::accept()
{
    if(mFD < 0)
    {
        return NULL;
    }
    size_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in sin;
    int fd = ::accept(mFD, (struct sockaddr*)&sin, (socklen_t*)&len);

    if(fd < 0)
    {
        return NULL;
    }

    EndPoint* endpoint = new EndPoint(inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    endpoint->setFD(fd);

    return endpoint;
}

int EndPoint::listen(int backlog)
{
    if(mFD < 0)
    {
        return -1;
    }
    return ::listen(mFD, backlog);

}

int EndPoint::bind(const char* ip, const unsigned int port)
{
    if(mFD < 0)
    {
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = ip == NULL? INADDR_ANY:inet_addr(ip);
    sin.sin_port        = htons(port);

    mPort               = sin.sin_port;
    mIP                 = sin.sin_addr.s_addr;

    return ::bind(mFD, (struct sockaddr*) &sin, sizeof(struct sockaddr_in));
}

void EndPoint::setnonblocking(const bool flag)
{
    int opt = flag ? O_NONBLOCK : 0;
    ::fcntl(mFD, F_SETFL, opt);
}

void EndPoint::setnodelay(const bool flag)
{
    int on = flag ? 1 : 0;
    ::setsockopt(mFD, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on));
}

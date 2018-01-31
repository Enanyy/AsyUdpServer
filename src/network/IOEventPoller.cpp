#include "IOEventPoller.h"

IOEventPoller::IOEventPoller():mFD(epoll_create(256))
{
    
}


IOEventPoller::~IOEventPoller()
{
    close();
}

void IOEventPoller::close()
{
    ::close(mFD);
    IOEvent::close();
}

bool IOEventPoller::registerEndPoint(int fd, EndPoint* endpoint)
{
    if(endpoint == NULL)
    {
        return false;
    }
    
    endpoint->setnonblocking(true);
    endpoint->setnodelay(true);

    struct epoll_event ev;

    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    if(epoll_ctl(mFD, EPOLL_CTL_ADD, fd, &ev) < 0 )
    {
        printf("epoll_ctl:%d failed\n", fd);
        return false;
    }

    return IOEvent:: registerEndPoint(fd, endpoint);

}

bool IOEventPoller::unRegisterEndPoint(int fd, EndPoint* endpoint)
{
    struct epoll_event ev;

    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;

    epoll_ctl(mFD, EPOLL_CTL_DEL, fd, &ev);

    return IOEvent:: unRegisterEndPoint(fd, endpoint);
}

void IOEventPoller::update()
{
    struct epoll_event events[mBackLog];
    struct epoll_event ev;

    int nfds = epoll_wait(mFD, events, mBackLog, 500);

    for(int i = 0; i < nfds; ++i)
    {
        if(events[i].events & (EPOLLERR | EPOLLHUP))
        {
            int fd = events[i].data.fd;
            
            events[i].data.fd = -1;

            onError(fd);
        }
        else
        {
            if(events[i].data.fd == mServer.getFD())
            {
                onAccept(events[i].data.fd);
            }
            else{

                if(events[i].events & EPOLLIN)
                {
                    onRecv(events[i].data.fd);
                
                    ev.data.fd  = events[i].data.fd;
                    ev.events   = EPOLLOUT | EPOLLET;
                    epoll_ctl(mFD, EPOLL_CTL_MOD, ev.data.fd, &ev);
                
                }
                else if(events[i].events & EPOLLOUT)
                {
                    onSend(events[i].data.fd);
                    
                    ev.data.fd  = events[i].data.fd;
                    ev.events   = EPOLLIN | EPOLLET;
                    epoll_ctl(mFD, EPOLL_CTL_MOD, ev.data.fd, &ev);
                }
            }
        }
    }
}


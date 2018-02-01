#ifndef _IOEVENT_SERVICE_H_
#define _IOEVENT_SERVICE_H_

#include <vector>
#include <queue>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "Packet.h"
#include "EndPoint.h"
#include "IOEventPoller.h"
#include "../public.h"
#include "../common/ThreadPool.h"
#include "../common/nocopyable.h"

class IOEventService:public nocopyable, 
                     public IOEventListener
{
    public:
        IOEventService();
        virtual ~IOEventService();

        void start(const char * ip, const int tcpport, const int udpport);
        void close();


        void run();
    private:

        bool initTcp(const char *ip, const unsigned int port);
        bool initUdp(const unsigned int port);

        //IOEventListener
        virtual void    onConnect(EndPoint* endpoint); 
        virtual int     onRecv(EndPoint* endpoint);
        virtual int     onSend(EndPoint* endpoint);
        virtual void    onDisconnect(EndPoint* endpoint);
        virtual void    onClose();
        virtual void    onError(EndPoint* endpoint);

    private:

        bool                        mInited;
        IOEventPoller               mTcpServer;
        EndPoint                    mUdpServer;
};


#endif


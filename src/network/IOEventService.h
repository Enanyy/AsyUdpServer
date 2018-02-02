#ifndef _IOEVENT_SERVICE_H_
#define _IOEVENT_SERVICE_H_

#include <vector>
#include <queue>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "Packet.h"
#include "Message.h"
#include "EndPoint.h"
#include "IOEventPoller.h"
#include "../public.h"
#include "../common/ThreadPool.h"
#include "../common/nocopyable.h"

class IOEventService;

class IOEventHandler
{
    friend class IOEventService;
    public:
        virtual void update() = 0;
        virtual void onStart() = 0;
        virtual void onMessage(Message*) = 0;
        virtual void onConnect(EndPoint*) = 0;
        virtual void onDisconnect(EndPoint*) = 0;

        void send(EndPoint* endpoint, Packet* packet);

        void sendto(EndPoint* endpoint, Packet* packet);

    private:

        void init(IOEventService* service)
        {
            mService = service;
        }
    private:
        IOEventService* mService;
};

typedef std::queue<Message*>  MessageQueue;
typedef std::queue<EndPoint*> EndPointQueue;

class IOEventService:public nocopyable, 
                     public IOEventListener
{
    public:
        IOEventService();
        virtual ~IOEventService();

        bool init(IOEventHandler* handler);

        void start(const char * ip, const int tcpport, const int udpport);
        void close();


        void run();
        void sendto(EndPoint* endpoint, Packet* packet);
    private:

        bool initTcp(const char *ip, const unsigned int port);
        bool initUdp(const unsigned int port);

        //IOEventListener
        virtual void    onConnect(EndPoint* endpoint); 
        virtual void    onRecv(EndPoint* endpoint, Packet* packet);
        virtual void    onDisconnect(EndPoint* endpoint);
        virtual void    onClose();
        virtual void    onError(EndPoint* endpoint);

        void onRecvfrom(void* arg);
        void onSendto(void* arg);
    private:
        IOEventHandler*             mEventHandler;
        bool                        mInited;
        IOEventPoller               mTcpServer;
        EndPoint                    mUdpServer;

        MessageQueue                mMessageQueue;
        MessageQueue                mSendToQueue;
        EndPointQueue               mConnectQueue;
        EndPointQueue               mDisconnectQueue;
};


#endif


#ifndef _ASYCUDPSERVER_H_
#define _ASYCUDPSERVER_H_

//#include <vector>
//#include <queue>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "Client.h"
#include "network/Packet.h"
#include "MessageInfo.h"
#include "common/DelegateFactory.h"
#include "common/SafeQueue.h"
#include "common/SafeVector.h"
#include "network/EndPoint.h"
#include "public.h"
#include "common/ThreadPool.h"
#include "network/IOEventPoller.h"

class Client;
class MessageInfo;

class AsycUdpServer: public IOEventListener
{
    public:
        AsycUdpServer();
        virtual ~AsycUdpServer();

        void start(const char * ip, const int tcpport, const int udpport);
        void close();

        void update();

        void sendUdp(MessageInfo* message);

        void clientDisconnect(Client * client);

        Client* getClient(EndPoint* endpoint);
        Client* getClient(const unsigned long ip, const unsigned int port);

        void release();

        void registerStart(IDelegate* delegate) { if(delegate) mOnStart += delegate;}
        void registerConnect(IDelegate* delegate){ if(delegate) mOnConnect+= delegate;}
        void registerMessage(IDelegate* delegate){ if(delegate) mOnMessage+= delegate;}
        void registerDisconnect(IDelegate* delegate) {if(delegate) mOnDisconnect += delegate;}
        void registerDebug(IDelegate* delegate){if(delegate) mOnDebug+= delegate;}
    private:

        bool initTcp(const char *ip, const unsigned int port);
        bool initUdp(const unsigned int port);

        //EpollListener
        virtual void    onConnect(EndPoint* endpoint); 
        virtual int     onRecv(EndPoint* endpoint);
        virtual int     onSend(EndPoint* endpoint);
        virtual void    onDisconnect(EndPoint* endpoint);
        virtual void    onClose();
        virtual void    onError(EndPoint* endpoint);

    
        //udp 
        void onRecvfrom(void*);
        void onSendto(void*);

    private:

        bool                        mAvaible;
        IOEventPoller               mTcpServer;
        EndPoint                    mUdpServer;

        SafeVector<Client*>         mClientList;

        SafeQueue<Client*>          mConnectQueue;
        SafeQueue<Client*>          mDisconnectQueue;

        SafeQueue<MessageInfo*>     mReceiveMessageQueue;
        SafeQueue<MessageInfo*>     mSendToMessageQueue;
        

        Delegate mOnStart;
        Delegate mOnConnect;
        Delegate mOnMessage;
        Delegate mOnDisconnect;
        Delegate mOnDebug;
};


#endif


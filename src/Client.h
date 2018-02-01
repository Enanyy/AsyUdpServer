#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "AsycUdpServer.h"
#include "network/EndPoint.h"
#include "network/Packet.h"
#include <queue>

class AsycUdpServer;
class EndPoint;
class Packet;
class Client
{

    public:
        Client(AsycUdpServer* server, EndPoint* endpoint);
        ~Client();

        EndPoint* getEndPoint(){return mEndPoint;}

        void send(Packet* packet);
        void sendto(Packet* packet);

        void onConnect();
        void onDisconnect();

        void release();
    private:
        
        EndPoint* mEndPoint;
        AsycUdpServer * mServer;
};


#endif


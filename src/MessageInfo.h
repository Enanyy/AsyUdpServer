#ifndef _MESSAGEINFO_H_
#define _MESSAGEINFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Client.h"
#include "network/Packet.h"
#include "MessageID.h"
class Client;

class MessageInfo
{
    public :
        MessageInfo(Packet *packet,Client *c);
        ~MessageInfo();

        Packet* getPacket();
        MessageID getPacketID();
        Client* getClient();


        void release();
    private:
        Client* mClient;
        Packet* mPacket;

};

#endif

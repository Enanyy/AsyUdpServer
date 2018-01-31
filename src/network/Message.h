#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Packet.h"
#include "EndPoint.h"

class Message
{
    public:
        Message(EndPoint* endpoint, Packet* packet):mEndPoint(endpoint),mPacket(packet){}
        ~Message(){}

        EndPoint* getEndPoint(){return mEndPoint;}
        Packet*   getPacket(){return mPacket;}
    private:
        EndPoint* mEndPoint;
        Packet*   mPacket;

};


#endif

#ifndef _PACKET_H_
#define _PACKET_H_

#include "MemoryStream.h"
#include "EndPoint.h"

class Packet: public MemoryStream
{
    public:
        static const size_t PACKET_ID_OFFSET = 0;
        static const size_t PACKET_TIMESTAMP_OFFSET = 4;
        static const size_t PACKET_BODY_LENGTH_OFFSET = 8;
        static const size_t PACKET_BODY_OFFSET = 12;
        static const size_t PACKET_HEAD_LENGTH = 12;

        static const size_t PACKET_SIZE = 64*1024;
    public:
        Packet();
        Packet(size_t size);
        Packet(char* data, unsigned int length);
        Packet(int id, char* body, unsigned int bodylength);
        virtual ~Packet();

        int                 recvfrom(EndPoint* endpoint, sockaddr_in& from);
        int                 recv(EndPoint* endpoint);

        int                 getPacketID();
        char*               getPacketBody();
        unsigned int        getPacketBodyLength();
        unsigned int        getPacketTimeStamp();

};

#endif

#include "Packet.h"

Packet::Packet():MemoryStream(PACKET_HEAD_LENGTH)
{

}

Packet::Packet(size_t size):MemoryStream(size)
{

}

Packet::Packet(char* data, unsigned int length):MemoryStream(length)
{
    clear();
    resize(length);
    append(data, length);
}

Packet::Packet(int id, char* body, unsigned int bodylength)
{
    mlength = (size_t)(PACKET_HEAD_LENGTH + bodylength);
    clear();

    resize(mlength);
    
    append<int>(id);
    append<int>(0);
    append<int>(bodylength);
    append(body, bodylength);
}

Packet::~Packet()
{
    clear();
}

int Packet::getPacketID()
{
    if(mlength <= PACKET_ID_OFFSET)
    {
        return 0;
    }
    return read<int>(PACKET_ID_OFFSET);
}

char* Packet::getPacketBody()
{
    if(mlength <= PACKET_BODY_OFFSET)
    {
        return NULL;
    }
    return read(PACKET_BODY_OFFSET);
}

unsigned int Packet::getPacketBodyLength()
{
    if(mlength <= PACKET_BODY_LENGTH_OFFSET)
    {
        return 0;
    }
    return read<unsigned int>(PACKET_BODY_LENGTH_OFFSET);
}

unsigned int Packet::getPacketTimeStamp()
{
    if(mlength <= PACKET_TIMESTAMP_OFFSET)
    {
        return 0;
    }
    return read<unsigned int>(PACKET_TIMESTAMP_OFFSET);
}

int Packet::recv(EndPoint* endpoint)
{
    if(endpoint == NULL)
    {
        return 0;
    }

    if(size() < PACKET_HEAD_LENGTH)
    {
        resize(PACKET_HEAD_LENGTH);
    }

    mlength = endpoint->recv(data(), PACKET_HEAD_LENGTH);
    
    if(mlength == PACKET_HEAD_LENGTH)
    {
        int bodylength = read<int>(PACKET_BODY_LENGTH_OFFSET);
        if(bodylength > 0)
        {
            int packetlength = PACKET_HEAD_LENGTH + bodylength;
            if(packetlength > size())
            {
                resize(packetlength);
            }

            int recvbodylength = endpoint->recv(data() + PACKET_HEAD_LENGTH, bodylength);
            if(recvbodylength > 0)
            {
                mlength += recvbodylength;
            }
            else
            {
                clear();
                mlength = -1;
            }
        }
    }

    return length();

}

int Packet::recvfrom(EndPoint* endpoint, sockaddr_in& from)
{
    if(endpoint == NULL)
    {
        return 0;
    }

    char buffer[PACKET_SIZE];
    

    int recvlength = endpoint->recvfrom(buffer, PACKET_SIZE, from);
    mlength = recvlength;

    if(size() < recvlength)
    {
        resize(recvlength);
    }

    append(buffer, recvlength);

    return mlength;
}

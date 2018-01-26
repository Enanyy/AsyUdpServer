#include "MessageInfo.h"

MessageInfo:: MessageInfo(Packet* packet,Client * c)
{
    mPacket = packet;
    mClient = c;
}

MessageInfo::~MessageInfo()
{
    mPacket = NULL;
    mClient = NULL;
}

Packet* MessageInfo::getPacket()
{
    return mPacket;
}

Client* MessageInfo::getClient()
{
    return mClient;
}

MessageID MessageInfo::getPacketID()
{
    if(mPacket)
    {
        return (MessageID)mPacket->getPacketID();
    }
    return MessageID::MinValue;
}

void MessageInfo::release()
{
    SAFE_DELETE(mPacket);
}

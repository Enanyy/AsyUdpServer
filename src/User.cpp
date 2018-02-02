#include "User.h"

User::User(IOEventHandler* handler, EndPoint* endpoint)
{
    mEventHandler = handler;
    mEndPoint = endpoint;
    mReady = false;
}

User:: ~User()
{

}

void User::sendto(Packet* packet)
{
    if(mEventHandler)
    {
       mEventHandler->sendto(mEndPoint, packet);
    }
}

void User::send(Packet* packet)
{
    if(mEventHandler)
    {
        mEventHandler->send(mEndPoint, packet);
    }
}

void User::send(MessageID messageId, const char*data, const size_t length)
{
    if(data != NULL)
    {
        printf("User::sendTcp %d cache tcp id = %d.\n",getUserID(), (int)messageId);
        Packet* packet = new Packet((int)messageId, const_cast<char*>(data), length);
        send(packet);
    }
}

void User::sendto(MessageID messageId, const char* data, const size_t length)
{
    if(data != NULL)
    {
        printf("User::sendUdp %d cache udp id = %d.\n",getUserID(), (int)messageId);
        Packet* packet = new Packet((int)messageId, const_cast<char*>(data), length);
        sendto(packet);
    }
}

int User::getUserID()
{
    if(mEndPoint)
    {
        return mEndPoint->getFD();
    }
    return -1;
}

void User::online()
{

}

void User::offline()
{
}

void User::update()
{

}

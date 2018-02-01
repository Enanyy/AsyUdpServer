#include "Client.h"

Client::Client(AsycUdpServer* server,EndPoint* endpoint):mServer(server),mEndPoint(endpoint)
{
}

Client::~Client()
{
    mServer = NULL;
}

void Client::release()
{

}

void Client::onConnect()
{

}

void Client::onDisconnect()
{

}

void Client::send(Packet* packet)
{
    if(packet!=NULL && mServer != NULL)
    {
        mServer->send(mEndPoint, packet);
    }
}

void Client::sendto(Packet* packet)
{
    if(mServer)
    {
        mServer->sendto(new MessageInfo(packet,this));
    }
}

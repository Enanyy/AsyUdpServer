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

void Client::sendTcp(Packet* packet)
{
    if(packet!=NULL)
    {
        printf("Client::sendTcp client %d cache tcp packet id = %d.\n", mEndPoint->getFD(), packet->getPacketID());
        mSendTcpQueue.push(packet);
    }
}

int Client::sendTcp()
{
    printf("Client::sendTcp sendQueue.size = %d.\n",mSendTcpQueue.size());
    
    int size = 0;

    while(mSendTcpQueue.size() > 0 )
    {
        Packet* packet = mSendTcpQueue.front();

        if(packet!=NULL && mEndPoint!=NULL && mEndPoint->isValid())
        {
            
            int result =mEndPoint->send(packet->data(),packet->length());
            if(result < 0)
            {
                return result;
            }

            size += result;

            printf("Client::sendTcp send tcp id=%d length=%d.\n",packet->getPacketID(), packet->length());
        }

        mSendTcpQueue.pop();
        SAFE_DELETE(packet);
    }
    return size;
}

void Client::sendUdp(Packet* packet)
{
    if(mServer)
    {
        mServer->sendUdp(new MessageInfo(packet,this));
    }
}

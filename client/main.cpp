#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include <pthread.h>
//#include <queue>

#include "../src/network/Packet.h"
#include "../src/MessageID.h"
#include "../src/protocpp/scene.pb.h"
#include "../src/protocpp/action.pb.h"
#include "../src/public.h"
#include "../src/common/SafeQueue.h"

void* sendTcp(void*);
void* recvTcp(void*);
void* sendUdp(void*);
void* recvUdp(void*);
void* input(void*);
void onMessage(Packet* message);

SafeQueue<Packet*> sendTcpQueue;
SafeQueue<Packet*> sendUdpQueue;
SafeQueue<Packet*> recvMessageQueue;

pthread_t sendTcpThread;
pthread_t recvTcpThread;
pthread_t sendUdpThread;
pthread_t recvUdpThread;
pthread_t inputThread;


int tcpSock = -1;
int udpSock = -1;

struct sockaddr_in tcpSockAddr;
struct sockaddr_in udpSockAddr;

const unsigned int LOG_TIME = 10000000;
const bool DEBUG_RUNNIG = false;

#define SEND_BY_TCP(messageId, send)\
    std::string buffer;\
    send.SerializeToString(&buffer);\
    printf("buffer length:%d\n", strlen(buffer.c_str()));\
    Packet* message = new Packet((int)messageId, (char*)buffer.c_str(), strlen(buffer.c_str()));\
    printf("id=%d, time=%d,length =%d\n", message->getPacketID(), message->getPacketTimeStamp(), message->getPacketBodyLength());\
    sendTcpQueue.push(message);\

int main(int argc, char** argv)
{
/*
                    GM_Ready send;
                    send.set_roleid(10);
                    GMPoint3D* position = send.mutable_position();
                    position->set_x(1);
                    position->set_y(2);
                    position->set_z(3);

                    GMPoint3D* direction =send.mutable_direction();
                    direction->set_x(4);
                    direction->set_y(5);
                    direction->set_z(6);
                    
                    SEND_BY_TCP(MessageID:: GM_READY_CS, send);

                    return 0;
*/
    if(argc != 4)
    {
        printf("Usage: ./client ip tcpport udpport\n");
        exit(1);
    }

    bzero(&tcpSockAddr, sizeof(struct sockaddr_in));
    bzero(&udpSockAddr, sizeof(struct sockaddr_in));

    tcpSock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(tcpSock < 0)
    {
        printf("Create tcp socket fail.\n");
        exit(1);
    }

    printf("Create tcp socket success.\n");


    int tcpport = atoi(argv[2]);
    char* ip = argv[1];

    tcpSockAddr.sin_family = AF_INET;
    tcpSockAddr.sin_port = htons(tcpport);
    tcpSockAddr.sin_addr.s_addr = inet_addr(ip);

    printf("ip = %s, tcp = %d.\n",ip, tcpport);

    if(::connect(tcpSock, (struct sockaddr*)&tcpSockAddr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("Connect to %s:%d fail.\n",ip, tcpport);
        ::close(tcpSock);
        exit(1);
    }
    printf("tcp connect to %s:%d success.\n", ip, tcpport);
     
    int udpport = atoi(argv[3]);

    udpSockAddr.sin_family = AF_INET;
    udpSockAddr.sin_addr.s_addr = inet_addr(ip);
    udpSockAddr.sin_port = htons(udpport);

    udpSock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(udpSock < 0)
    {
        printf("Create ucp socket fail.\n");
        ::close(tcpSock);
        ::close(udpSock);
        exit(1);
    }

    printf("Create udp socket success.\n");
    
    pthread_create(&sendTcpThread, NULL, sendTcp, NULL);
    pthread_create(&recvTcpThread, NULL, recvTcp, NULL);
    pthread_create(&sendUdpThread, NULL, sendUdp, NULL);
    pthread_create(&recvUdpThread, NULL, recvUdp, NULL);
    pthread_create(&inputThread, NULL, input, NULL);

   // pthread_join(sendTcpThread, NULL);
   // pthread_join(recvTcpThread, NULL);
   // pthread_join(sendUdpThread, NULL);
   // pthread_join(recvTcpThread, NULL);
   // pthread_join(inputThread,NULL);


    while(true)
    {
        //printf("recvMessageQueue.size() = %d\n",recvMessageQueue.size());
        while(recvMessageQueue.size() > 0)
        {
            Packet* message = recvMessageQueue.front();
            if(message)
            {
                printf("recv message:%d\n",message->length());
                onMessage(message);
            }
            recvMessageQueue.pop();

            SAFE_DELETE(message);
        }

        sleep(1); 
    }

    ::close(tcpSock);
    ::close(udpSock);


    return 0;
}

void* sendTcp(void* arg)
{
    printf("begin sendTcp\n");

    unsigned long milliseconds = 0;
    
    while(true)
    {
        if(tcpSock < 0)
        {
            pthread_exit(0);
            break;
        }

        if(milliseconds % LOG_TIME == 0 && DEBUG_RUNNIG)
        {
            printf("sendTcp running...\n");
        }
        milliseconds += 10;

        while(sendTcpQueue.size() > 0)
        {
            Packet *message = sendTcpQueue.front();
            if(message)
            {
                char* buffer = message->data();
                int length = message->length();
                while(length > 0)
                {
                    int ret = ::send(tcpSock, buffer, length, 0);
                    if(ret < 0)
                    {
                        break;
                    }
                    buffer += ret;
                    length -= ret;

                }

                printf("send tcp messageId:%d, messagelength:%d senddlength:%d\n",(int)message->getPacketID(), message->length(), message->length() - length);
            }

            sendTcpQueue.pop();
            SAFE_DELETE(message);
        }


        //usleep(10);
    }

    return NULL;
}

void* recvTcp(void* arg)
{
    printf("begin recvTcp\n");
    char* head = new char[Packet:: PACKET_HEAD_LENGTH];
    unsigned long milliseconds = 0;
    while(true)
    {
        if(tcpSock < 0)
        {
            pthread_exit(0);
            break;
        }

        if(milliseconds % LOG_TIME == 0 && DEBUG_RUNNIG){
            printf("recvTcp running...\n");
        }
        milliseconds += 10;

        bzero(head, Packet:: PACKET_HEAD_LENGTH);
        
        char* buffer= head;
        size_t needRecvSize = Packet:: PACKET_HEAD_LENGTH;

        int receive = 0;

        receive = ::recv(tcpSock, buffer, needRecvSize, 0);
        if(receive < 0)
        {
            ::close(tcpSock);
            break;
        }

        if(receive > 0)
        {
            printf("tcp recv head length:%d.\n",receive);
        }
      
        if(receive >= Packet:: PACKET_HEAD_LENGTH)
        {

            int bodyLength = 0;
            memcpy(&bodyLength, head + Packet:: PACKET_BODY_LENGTH_OFFSET, 4);

            printf("tcp recv bodylength:%d\n", bodyLength);

            if(bodyLength > 0)
            {
                Packet* message = new Packet(bodyLength);
                needRecvSize = bodyLength;

                receive = 0;
                buffer = message->data();

                receive = ::recv(tcpSock, buffer, needRecvSize, 0);
                if(receive < 0)
                {
                    ::close(tcpSock);
                    break;
                }
                printf("receive body length:%d\n",receive);

                if(receive == needRecvSize)
                {
                    recvMessageQueue.push(message);
                }
                else
                {
                    SAFE_DELETE(message);
                }
            }
        }

        //usleep(10);
    }

    SAFE_DELETE(head);

    return NULL;
}

void* sendUdp(void* arg)
{
    printf("begin sendUdp\n");

    unsigned long milliseconds = 0;
    
    while(true)
    {
        if(udpSock < 0)
        {
            pthread_exit(0);
            break;
        }

        if(milliseconds % LOG_TIME == 0 && DEBUG_RUNNIG)
        {
            printf("sendUdp running...\n");
        }
        milliseconds += 10;

        while(sendUdpQueue.size() > 0)
        {
            Packet* message = sendUdpQueue.front();
            if(message)
            {
                int ret = ::sendto(udpSock, message->data(), message->length(), 0, (const struct sockaddr*)&udpSockAddr, sizeof(struct sockaddr_in));
                printf("udp send message length:%d,%d\n",ret, message->length());
            }

            SAFE_DELETE(message);
            sendUdpQueue.pop();
        }
        //usleep(10);
    }
    return NULL;
}

void* recvUdp(void* arg)
{
    printf("begin recvUdp\n");
    const int MAX_BUFFER_SIZE =1024 * 1024 * 64;
    char* tempbuffer = new char[MAX_BUFFER_SIZE];

    socklen_t socklen =(socklen_t) sizeof(udpSockAddr);
    
    unsigned long milliseconds = 0;
    while(true)
    {
        if(udpSock < 0)
        {
            pthread_exit(0);
            break;
        }

        if(milliseconds % LOG_TIME == 0 && DEBUG_RUNNIG)
        {
            printf ("recvUdp running...\n");
        }
        milliseconds += 10;

        bzero(tempbuffer, MAX_BUFFER_SIZE);

        int ret = ::recvfrom(udpSock, tempbuffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&udpSockAddr, &socklen);

        if(ret > 0)
        {
            printf("udp recv length:%d\n",ret);

            int bodyLength = ret - Packet:: PACKET_HEAD_LENGTH;
            if(bodyLength <0)bodyLength = 0;

           Packet* message = new Packet(bodyLength);

            memcpy(message->data(), tempbuffer, ret);
            recvMessageQueue.push(message);
        }

        //usleep(10);
    }
    return NULL;
}

void onMessage(Packet* message)
{
    if(message == NULL)return;
    
    MessageID messageId = (MessageID)message->getPacketID();
    switch(messageId)
    {
        case MessageID::GM_CONNECT_BC:
            {
                RECV_MSG(msg,GM_Connect,message->getPacketBody());

                printf("Recv:GM_CONNECT_BC,clientid=%d roleid = %d\n",msg.clientid(), msg.roleid());

            }break;
    }
}


void* input(void* arg)
{
    while(true)
    {
        printf("enter message id:");
        int messageId = -1;
        
        scanf("%d",&messageId);
        
        switch((MessageID)messageId)
        {
            case MessageID:: GM_READY_CS:
                {
                    GM_Ready send;
                    send.set_roleid(10);
                    GMPoint3D* position = send.mutable_position();
                    position->set_x(1);
                    position->set_y(2);
                    position->set_z(3);

                    GMPoint3D* direction =send.mutable_direction();
                    direction->set_x(4);
                    direction->set_y(5);
                    direction->set_z(6);
                    
                    SEND_BY_TCP(MessageID:: GM_READY_CS, send);

                }break;
            case MessageID:: GM_FRAME_CS:
                {

                }break;
            case MessageID:: GM_POSITION_BC:
                {

                }break;
            case MessageID:: GM_GET_FRAME_CS:
                {

                }break;
        }
    }
        
    return NULL;
}

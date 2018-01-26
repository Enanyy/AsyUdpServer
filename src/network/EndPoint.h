#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

enum SOCKET_TYPE
{
    TCP,
    UDP,
};

class EndPoint
{
    public:
        EndPoint();
        EndPoint(char* ip, unsigned int port);
        virtual ~EndPoint();

        int                 socket(SOCKET_TYPE type);
        void                close();
        int                 send(const void* data, const int length);
        int                 recv(void* data, const int length);
        void                sendto(const void* data, const int length);
        int                 recvfrom(void* data, const int length, sockaddr_in& from);
        EndPoint*           accept();
        int                 listen(int backlog);
        int                 bind(const char* ip, const unsigned int port);

        void                setFD(const int fd){mFD = fd;}
        int                 getFD(){ return mFD; }

        unsigned int        getPort(){ return mPort; }
        unsigned long       getIP(){ return mIP;}
        char*               getHostIP();
        unsigned int        getHostPort(){ return ntohs(mPort);}

        bool                isValid(){ return mFD >=0; }
        void                setnonblocking(const bool flag);
        void                setnodelay(const bool flag);
    private:

        int                 mFD;

        unsigned int        mPort;
        
        unsigned long       mIP;
};

#endif

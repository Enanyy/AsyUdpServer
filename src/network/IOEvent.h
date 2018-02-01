#ifndef _IO_EVENT_H_
#define _IO_EVENT_H_

#include "EndPoint.h"
#include "Packet.h"
#include <unordered_map>
#include <queue>

class EndPoint;
class Packet;
class IOEvent;

class IOEventListener
{
    friend class IOEvent;

    public:
        IOEventListener(){}
        virtual ~IOEventListener() {}

        void            send(EndPoint* endpoint, Packet* packet);
    private:
        void            init(IOEvent* event){mIOEvent = event;}

        virtual void    onConnect(EndPoint*) = 0;
        virtual void     onRecv(EndPoint* endpoint, Packet* packet) = 0;
        virtual void    onDisconnect(EndPoint*) = 0;
        virtual void    onError(EndPoint*) = 0;
        virtual void    onClose() = 0;

    private:
        IOEvent*        mIOEvent;
};


typedef std::unordered_map<int, EndPoint*>              EndPoints;
typedef std::unordered_map<int, std::queue<Packet*> >   Packets;

class IOEvent
{
    public:
        IOEvent();
        virtual ~IOEvent();

        virtual bool start(IOEventListener* listener, const int backlog, const char* ip, const unsigned int port);
       
        virtual void update() = 0;
        virtual void close();

        virtual bool registerEndPoint(int fd,EndPoint* endpoint);
        virtual bool unRegisterEndPoint(int fd, EndPoint* endpoint);
        
        EndPoint*    getEndPoint(const int fd);
        
        inline bool  isValid(){return mServer.isValid();}
        
        void         send(EndPoint* endpoint, Packet* packet);

     protected:
        void onAccept(const int fd);
        void onRecv(const int fd);
        void onSend(const int fd);
        void onError(const int fd);
        void onDisconnect(const int fd);
    
    protected:
        
        EndPoint            mServer;

        EndPoints           mEndPoints;

        IOEventListener*    mListener;

        int                 mBackLog;

        Packets             mPackets;
};

#endif

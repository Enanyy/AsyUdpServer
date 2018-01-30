#ifndef _IO_EVENT_H_
#define _IO_EVENT_H_

#include "EndPoint.h"
#include <unordered_map>


class EventListener
{
    friend class IOEvent;

    public:
        EventListener(){}
        virtual ~EventListener() {}

    private:
        virtual void    onConnect(EndPoint* endpoint) = 0;
        virtual int     onRecv(EndPoint* endpoint) = 0;
        virtual int     onSend(EndPoint* endpoint) = 0;
        virtual void    onDisconnect(EndPoint* endpoint) = 0;
        virtual void    onError(EndPoint* endpoint) = 0;
        virtual void    onClose() = 0;
};

class EndPoint;
typedef std::unordered_map<int, EndPoint*> EndPoints;

class IOEvent
{
    public:
        IOEvent();
        virtual ~IOEvent();

        virtual bool start(EventListener* listener, const int backlog, const char* ip, const unsigned int port);
       
        virtual void update() = 0;
        virtual void close();

        virtual bool registerEndPoint(int fd,EndPoint* endpoint);
        virtual bool unRegisterEndPoint(int fd, EndPoint* endpoint);
        
        EndPoint* getEndPoint(const int fd);
        
        inline bool isValid(){return mServer.isValid();}
     protected:
        void onAccept(const int fd);
        void onRecv(const int fd);
        void onSend(const int fd);
        void onError(const int fd);
        void onDisconnect(const int fd);
    
    protected:
        
        EndPoint            mServer;

        EndPoints           mEndPoints;

        EventListener*      mListener;

        int                 mBackLog;

};

#endif

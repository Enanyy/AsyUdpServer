#ifndef _USER_H_
#define _USER_H_

#include "MessageID.h"
#include "network/EndPoint.h"
#include "network/IOEventService.h"
class EndPoint;
class User
{
    public:
        User(IOEventHandler* handler, EndPoint * endpoint);
        ~User();

        void Ready(){mReady = true;}
        bool isReady(){return mReady;}

        void send(Packet* packet);
        void sendto(Packet* packet);

        void send(MessageID messageId, const char* data, const size_t length);
        void sendto(MessageID messageId, const char* data, const size_t length);

        int getUserID();

        void online();
        void offline();

        void update();
    private:
        IOEventHandler* mEventHandler;
        EndPoint* mEndPoint;
        bool mReady;
};

#endif

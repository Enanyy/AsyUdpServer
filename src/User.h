#ifndef _USER_H_
#define _USER_H_

#include "Client.h"
#include "MessageID.h"

class Client;
class User
{
    public:
        User(int userId, Client * client);
        ~User();

        void Ready(){mReady = true;}
        bool isReady(){return mReady;}

        void sendUdp(Packet* packet);
        void sendTcp(Packet* packet);

        void sendTcp(MessageID messageId, const char* data, const size_t length);
        void sendUdp(MessageID messageId, const char* data, const size_t length);

        int getClientID();
        int getUserID(){return mUserID;}

        void online();
        void offline();

        void update();
    private:
        int mUserID;
        Client* mClient;
        bool mReady;
};

#endif

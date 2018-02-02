#ifndef _FRAMESERVER_H_
#define _FRAMESERVER_H_

#include <map>

#include "User.h"
#include "ModuleManager.h"
#include "network/IOEventService.h"

class User;
class ModuleManager;
class FrameServer: public IOEventHandler
{
    public:
        static const int TCP_PORT = 1255;
        static const int UDP_PORT = 1337;
    public:
        FrameServer();
        ~FrameServer();

        User* getUser(const int userid);

    private:
        virtual void onStart();
        virtual void onConnect(EndPoint* endpoint);
        virtual void onMessage(Message* message);
        virtual void onDisconnect(EndPoint* endpoint);
        virtual void update();


    private:
        

        std::vector<User*> mUserList;

};

#endif

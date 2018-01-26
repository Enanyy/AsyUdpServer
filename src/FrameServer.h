#ifndef _FRAMESERVER_H_
#define _FRAMESERVER_H_

#include <map>

#include "AsycUdpServer.h"
#include "User.h"
#include "ModuleManager.h"

class User;
class ModuleManager;
class FrameServer
{
    public:
        static const int TCP_PORT = 1255;
        static const int UDP_PORT = 1337;
    public:
        FrameServer();
        ~FrameServer();

        void start();

        void close();

        void run();

        void update();

        User* getUser(const int userid);
        User* getUser(Client* client);

    private:
        void onStart(void* param);
        void onConnect(void* param);
        void onMessage(void* parma);
        void onDisconnect(void* param);

        void sendFrame();

        static void* sendFrameThread(void* param);

    private:
        
        unsigned long mGameFrameCount;

        std::vector<User*> mUserList;

        AsycUdpServer mServer;

        pthread_t mFrameThread;
};

#endif

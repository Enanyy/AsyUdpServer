#include <stdio.h>
#include <stdlib.h>

#include "AsycUdpServer.h"
#include "FrameServer.h"
#include "public.h"
#include "ModuleManager.h"
#include "common/ThreadPool.h"

using namespace std;

void *func(void* arg)
{
    printf("AAA\n");
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("server start.\n"); 

    ModuleManager::getMe()->init();
    
    ThreadPool::getMe()->init(5);

    ThreadPool::getMe()->addTask(func,NULL);


    FrameServer frameserver;

    frameserver.start();

    frameserver.run();

    return 0;
}

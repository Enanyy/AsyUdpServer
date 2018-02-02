#include <stdio.h>
#include <stdlib.h>

#include "FrameServer.h"
#include "public.h"
#include "ModuleManager.h"
#include "common/ThreadPool.h"
#include "network/IOEventService.h"

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

     IOEventService service;
     service.init(&frameserver);
    service.start("127.0.0.1",1255, 1337);
    service.run();

    ThreadPool::getMe()->destroy();

    return 0;
}

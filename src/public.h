/*************************************************************************
	> File Name: public.h
	> Author: Liwengui
	> Mail: 742216502@qq.com 
	> Created Time: 2016年07月07日 星期四 14时47分49秒
 ************************************************************************/
#ifndef __PUBLIC_H__
#define __PUBLIC_H__
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define DEBUG

#define SAFE_DELETE(p)\
	if((p))\
	{\
		delete (p);\
		(p) = NULL;\
	}\

#define SEND_TCP(messageId, send, user)\
		std::string buffer;\
		send.SerializeToString(&buffer);\
		if(user)\
		{\
			user->sendTcp(MessageID::messageId, buffer.c_str(), strlen(buffer.c_str()));\
		}\

#define SEND_UDP(messageId, send, user)\
        std::string buffer;\
        send.SerializeToString(&buffer);\
        if(user)\
        {\
            user->sendUdp(MessageID::messageId,buffer.c_str(), strlen(buffer.c_str()));\
        }\

#define RECV_MSG(name, message, data)\
        message name;\
        name.ParseFromString((char*)data);\

#define PTHREAD_LOCK(lock, code)\
        pthread_mutex_lock(&lock);\
        {\
            code;\
        }\
        pthread_mutex_unlock(&lock);\


#endif


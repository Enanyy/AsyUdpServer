#ifndef _SAFE_QUEUE_H_
#define _SAFE_QUEUE_H_

#include <pthread.h>
#include <iostream>
#include <queue>

template <class T>
class SafeQueue
{

    private:
        std::queue<T> mQueue;
        pthread_mutex_t mLock;

    public:
        SafeQueue()
        {
            pthread_mutex_init(&mLock, NULL);
        }
        ~SafeQueue()
        {
            pthread_mutex_destroy(&mLock);
        }


    public:
        bool empty()const
        {
            return mQueue.size() == 0;
        }

        int size() const
        {
            return mQueue.size();
        }

        void push(const T &t)
        {
            pthread_mutex_lock(&mLock);
            mQueue.push(t);
            
            pthread_mutex_unlock(&mLock);
        }

        void pop()
        {
            pthread_mutex_lock(&mLock);
            
            mQueue.pop();

            pthread_mutex_unlock(&mLock);
        }

        T& front()
        {
            pthread_mutex_lock(&mLock);

            T& t= mQueue.front();

            pthread_mutex_unlock(&mLock);

            return t;
        }

        T& back()
        {
            pthread_mutex_lock(&mLock);

            T& t= mQueue.back();

            pthread_mutex_unlock(&mLock);

            return t;
        }

};

#endif

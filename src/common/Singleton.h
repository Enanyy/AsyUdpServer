#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include <pthread.h>

template <class T>
class Singleton
{
    public: 
        static T* getMe()
        {
           if(mInstance == NULL)
           {
               pthread_mutex_lock(&_lock);
               if(mInstance == NULL)
               {
                   mInstance = new T;
               }
               pthread_mutex_unlock(&_lock);
           }
           return mInstance;
        }

        static void destroy()
        {
            if(mInstance!=NULL)
            {
                delete mInstance;
                mInstance = NULL;
            }
            pthread_mutex_destroy(&_lock);
        }
    protected:
        Singleton(){}
        virtual ~Singleton(){}

        Singleton(const Singleton&){}
        Singleton operator=(const Singleton&){}

    private:
        static T* mInstance;
        static pthread_mutex_t _lock;
};

template<class T>
T* Singleton<T>::mInstance = NULL;
template<class T>
pthread_mutex_t Singleton<T>::_lock = PTHREAD_MUTEX_INITIALIZER;

#endif

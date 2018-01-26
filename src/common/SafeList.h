#ifndef _SAFE_LIST_H_
#define _SAFE_LIST_H_
#include <list>
#include <pthread.h>

using namespace std;
template <class T>
class safelist
{
    private:
        std::list<T> mList;
        pthread_mutex_t mLock;

    public:
        safelist(){pthread_mutex_init(&mLock,NULL);}
        ~safeList(){pthread_mutex_destroy(&mLock);}

        typename list<T>::iterator begin()
        {
            pthread_mutex_lock(&mLock);
            typename list<T>::iterator it = mList.begin();
            pthread_mutex_unlock(&mLock);
            return it;
        }
        typename list<T>::const_iterator begin()
        {
            pthread_mutex_lock(&mLock);
            typename list<T>::const_iterator it = mList.cbegin();
            pthread_mutex_unlock(&mLock);
            return it;
        }

        typename list<T>::iterator end()
        {
            pthread_mutex_lock(&mLock);
            typename list<T>::iterator it = mList.end();
            pthread_mutex_unlock(&mLock);
            return it;
        }
        
        typename list<T>::const_iterator cend()
        {
            pthread_mutex_lock(&mLock);
            typename list<T>::const_iterator it = mList.cend();
            pthread_mutex_unlock(&mLock);
            return it;
        }
};


#endif


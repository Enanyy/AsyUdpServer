#ifndef _SAFE_VECTOR_H_
#define _SAFE_VECTOR_H_

#include <pthread.h>
#include <iostream>
#include <vector>

template <class T>
class SafeVector
{
    private:
        std::vector<T> mVec;
        pthread_mutex_t mLock;

    public:
        SafeVector()
        {
            pthread_mutex_init(&mLock, NULL);
        }
        ~SafeVector()
        {
            pthread_mutex_destroy(&mLock);
        }


    public:
        bool empty()const
        {
            return mVec.size() == 0;
        }

        int size() const
        {
            return mVec.size();
        }

        void push_back(const T &t)
        {
            pthread_mutex_lock(&mLock);

            mVec.push_back(t);
            
            pthread_mutex_unlock(&mLock);
        }

        void pop_back()
        {
            pthread_mutex_lock(&mLock);
            
            mVec.pop_back();

            pthread_mutex_unlock(&mLock);
        }

        T& front()
        {
            pthread_mutex_lock(&mLock);

            T& t= mVec.front();

            pthread_mutex_unlock(&mLock);

            return t;
        }

        T& back()
        {
            pthread_mutex_lock(&mLock);

            T& t= mVec.back();

            pthread_mutex_unlock(&mLock);

            return t;
        }

        typename std::vector<T>::iterator begin()
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::iterator it = mVec.begin();
            pthread_mutex_unlock(&mLock);
            return  it;
            
        }

        typename std::vector<T>::iterator end()
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::iterator it = mVec.end();
            pthread_mutex_unlock(&mLock);
            return  it;
        }

        typename std::vector<T>::const_iterator cbegin()
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::const_iterator it = mVec.cbegin();
            pthread_mutex_unlock(&mLock);
            return  it;
        }

        typename std::vector<T>::const_iterator cend()
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::const_iterator it = mVec.cend();
            pthread_mutex_unlock(&mLock);
            return  it;
        }

        typename std::vector<T>::iterator erase(typename std::vector<T>::iterator position)
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::iterator it = mVec.erase(position);
            pthread_mutex_unlock(&mLock);
            return it;
        }

        typename std::vector<T>::iterator erase(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last)
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::iterator it= mVec.erase(first, last);
            pthread_mutex_unlock(&mLock);
            return it;
        }
        
        typename std::vector<T>::iterator insert(typename std::vector<T>::iterator position, const T& value)
        {
            pthread_mutex_lock(&mLock);
            typename std::vector<T>::iterator it= mVec.insert(position, value);
            pthread_mutex_unlock(&mLock);
            return it;
        }

        void clear()
        {
            pthread_mutex_lock(&mLock);
            mVec.clear();
            pthread_mutex_unlock(&mLock);
        }

};

#endif

#ifndef _SAFE_MAP_H_
#define _SAFE_MAP_H_

#include <pthread.h>
#include <iostream>
#include <map>

template <class K, class V>
class SafeMap
{
    private:
        std::map<K, V> mMap;
        pthread_mutex_t mLock;
    public:
        SafeMap()
        {
            pthread_mutex_init(&mLock,NULL);
        }
        ~SafeMap()
        {
            pthread_mutex_destroy(&mLock);
        }

        int size()
        {
            return mMap.size();
        }

        bool empty(){ return mMap.size() == 0;}

        V& at(const K& k){return mMap.at(k);}
       
        V& operator[](const K& k)
        {
            return mMap[k];
        }

        typename std::map<K, V>::iterator begin()
        {
            pthread_mutex_lock(&mLock);
            typename std::map<K,V>::iterator it = mMap.begin();
            pthread_mutex_unlock(&mLock);
            return it;
        }
        typename std::map<K, V>::iterator end()
        {
            pthread_mutex_lock(&mLock);
            typename std::map<K,V>::iterator it = mMap.end();
            pthread_mutex_unlock(&mLock);
            return it;
        }
        typename std::map<K, V>::const_iterator cbegin()
        {
            pthread_mutex_lock(&mLock);
            typename std::map<K,V>::const_iterator it = mMap.cbegin();
            pthread_mutex_unlock(&mLock);
            return it;
        }
        typename std::map<K, V>::const_iterator cend()
        {
            pthread_mutex_lock(&mLock);
            typename std::map<K,V>::const_iterator it = mMap.cend();
            pthread_mutex_unlock(&mLock);
            return it;
        }

        typename std::map<K,V>::iterator find(const K& k)
        {
             typename std::map<K,V>::iterator it;
             pthread_mutex_lock(&mLock);
             it = mMap.find(k);
             pthread_mutex_unlock(&mLock);
             return it;
        }

        void erase( typename std::map<K,V>::iterator position)
        {
            pthread_mutex_lock(&mLock);
            mMap.erase(position);
            pthread_mutex_unlock(&mLock);
        }

        void clear()
        {
            pthread_mutex_lock(&mLock);
            mMap.clear();
            pthread_mutex_unlock(&mLock);
        }

        int count(const K& k)const
        {
            int n = 0;
            pthread_mutex_lock(&mLock);
            n = mMap.count(k);
            pthread_mutex_unlock(&mLock);
            return n;
        }
    

        std::pair<typename std::map<K,V> :: iterator, bool> insert(const V& v)
        {
            std::pair<typename std::map<K,V> :: iterator, bool> ret;
            pthread_mutex_lock(&mLock);
            ret = mMap.insert(v);
            pthread_mutex_unlock(&mLock);
            return ret;
        }

        typename std::map<K, V>::iterator insert(typename std::map<K, V>::iterator position, const V& v)
        {
            typename std::map<K, V>::iterator it;
            pthread_mutex_lock(&mLock);
            it = mMap.insert(position, v);
            pthread_mutex_unlock(&mLock);
            return it;
        }

};

#endif

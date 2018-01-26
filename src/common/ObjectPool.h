#ifndef _OBJECT_POOL_H_
#define _OBJECT_POOL_H_

#include <vector>
#include <map>
#include <typeinfo>
#include "../common/Singleton.h"


class ObjectPool: public Singleton<ObjectPool>
{

    public:
        ~ObjectPool();


    public:
        template<typename T>
        T* pop();

        template<typename T>
        void push(void* object);

        template<typename T>
        void clear();

        void destroy();

    private:
        std::map<std::string, std::vector<void*> > mCacheObjects;
};

ObjectPool::~ObjectPool()
{
    destroy();
}

template<typename T>
T* ObjectPool::pop()
{
    std::string key = typeid(T).name();

    auto it = mCacheObjects.find(key);

    if(it == mCacheObjects.end())
    {
        return NULL;
    }

    if(it->second.empty())
    {
        return NULL;
    }

    auto iter = it->second.begin();

    if(iter == it->second.end())
    {
        return NULL;
    }

    T* t = static_cast<T*>(*iter);
    it->second.erase(iter);

    return t;

}

template<typename T>
void ObjectPool::push(void* object)
{
    if(object == NULL)
    {
        return ;
    }

    std::string key = typeid(T).name();
    auto it = mCacheObjects.find(key);

    if(it == mCacheObjects.end())
    {
        std::vector<void*> vec;
        mCacheObjects[key] = vec;
    }

    mCacheObjects[key].push_back(object);
}

template<typename T>
void ObjectPool:: clear()
{
    std::string key = typeid(T).name();
    auto it = mCacheObjects.find(key);

    if(it!=mCacheObjects.end())
    {
        for(auto i = it->second.begin(); i != it->second.end(); ++i)
        {
            T* t = static_cast<T*>(*i);
            if(t)
            {
                delete t;
                t = NULL;
            }
            *i = NULL;
        }

        it->second.clear();
    }
}

void ObjectPool::destroy()
{
    for(auto it = mCacheObjects.begin(); it != mCacheObjects.end(); ++it)
    {
        for(auto i = it->second.begin(); i!=it->second.end(); ++i)
        {
            void* object = (*i);
            if(object)
            {
                delete object;
                object = NULL;
            }
            (*i) = NULL;
        }
        it->second.clear();
    }

    mCacheObjects.clear();
}

#endif

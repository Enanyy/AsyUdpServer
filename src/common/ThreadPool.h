#ifndef _THREAD_POOL_HH_
#define _THREAD_POOL_HH_

#include <queue>
#include <vector>
#include "Singleton.h"
#include <pthread.h>
#include "../public.h"

struct ThreadTask
{
    virtual ~ThreadTask(){} 
    virtual void run() = 0;
    virtual void release() = 0;
};

struct GThreadTask : public ThreadTask
{
	void* (*process)(void* arg);
	
    void* data;
	
    GThreadTask(void* (*func)(void*), void* arg):process(func),data(arg)
	{
		
	}
	virtual ~GThreadTask()
	{
		release();
	}
	
	virtual	void run()
	{
		if (process)
		{
			(*process)(data);	
		}
	}
	virtual void release()
	{
		process = NULL;
		data	= NULL;
	}
};

template<class T>
struct CThreadTask: public ThreadTask
{
    T* object;
    void (T::*process)(void*);
    void* data;
    
    CThreadTask(T* t, void (T::*func)(void*), void* arg):object(t),process(func),data(arg)
    {

    }

    virtual ~CThreadTask()
    {
        release();   
    }

    virtual void run()
    {
        if(object && process)
        {
            (object->*process)(data);
        }
    }

    virtual void release()
    {
        object  = NULL;
        process = NULL;
        data    = NULL;
    }

};

class ThreadPool:public Singleton<ThreadPool>
{
    
    public:
        virtual ~ThreadPool()
        {
            destroy();
        }	

        void init(int size)
        {
            if (mInited)
            {
                printf("ThreadPool is inited.\n");
                return;
            }

            mSize = size;
            mThreads.clear();

            mLock = PTHREAD_MUTEX_INITIALIZER;
            mCond = PTHREAD_COND_INITIALIZER;

            for(int  i = 0; i < mSize; ++i)
            {
                pthread_t thread;
                pthread_create(&thread, NULL, process, this);
                mThreads.push_back(thread);
            }

            mInited = true;
            mShutdown = false;

            printf("ThreadPool init success.\n");
        }

        void addTask(void* (*func)(void*), void* arg)
        {
            ThreadTask* task = new GThreadTask(func, arg);

            addTask(task);
        }

        template<typename T>
        void addTask(T* object, void (T::*func)(void*), void* data)
        {
            ThreadTask* task = new CThreadTask<T>(object, func, data);
            addTask(task);
        }

        void addTask(ThreadTask* task)
        {
            if(!task)
                return;

            lock();

            mTasks.push(task);

            unlock();

            //有任务加进来，激活一个挂起的线程处理任务
            pthread_cond_signal(&mCond);

        }

        void destroy()
        {
            shutDown();

            pthread_cond_broadcast(&mCond);

            for(int i = mThreads.size() - 1; i >= 0; --i )
            {
                pthread_t tid = mThreads[i];
                pthread_join(tid, NULL);
            }

            mThreads.clear();

            clear();

            pthread_mutex_destroy(&mLock);
            pthread_cond_destroy(&mCond);

            mInited = false;
            mSize = 0;
            printf("destroy ThreadPool \n");

        }

        void shutDown() { mShutdown = true; }

        bool shouldShutDown() { return mShutdown; }

        unsigned int taskSize() { return mTasks.size();} 

        ThreadTask* popTask()
        {
            ThreadTask* task = NULL;
            
            pthread_t tid = pthread_self();
            printf("thread %d pop a task from queue, size=%d.\n", (int)tid, mTasks.size());

            if (mTasks.size() > 0)
            {
                task = mTasks.front();
                mTasks.pop();
            }

            return task;

        }

        void clear()
        {
            lock();

            for(int i = 0, max = mTasks.size(); i < max; ++i )
            {
                ThreadTask* task = popTask();
                removeTask(task);
            }

            unlock();

        }

        void lock()
        {
            pthread_mutex_lock(&mLock);
        }

        void unlock()
        {
           pthread_mutex_unlock(&mLock);
        }

        void wait()
        {
            pthread_cond_wait(&mCond, &mLock);
        }

        void run()
        {
            pthread_t tid = pthread_self();
            printf("thread %d is running.\n", (int)tid);


            while(true)
            {
                lock();

                //线程池已经关闭，激活所有线程逐个退出
                if (shouldShutDown())
                {
                    pthread_cond_broadcast(&mCond);
                    
                    unlock();

                    printf("thread %d will exit\n",(int)pthread_self());

                    pthread_exit(NULL);

                    break;
                }

                //当任务队列没有任务需要处理就挂起当前线程	
                if (taskSize() == 0)
                {
                    printf("thread %d is waiting\n", (int)pthread_self());

                    wait();
                    unlock();
                }
                else
                {
                    ThreadTask* task = popTask();

                    unlock();

                    if(task)
                    {
                        task->run();

                        removeTask(task);

                        printf("thread %d was done\n", (int)pthread_self());

                    }
                }	
            }
        }

    private:
        static	void* process(void* arg)
        { 
            pthread_t tid = pthread_self();
            printf("thread %d is starting.\n", (int)tid);
            
            if(arg == NULL)
            {
                printf("thread %d arg is NULL.\n",tid);
                return NULL;
            }

            ThreadPool* pool = static_cast<ThreadPool*>(arg);

            if(pool !=NULL)
            {
                pool->run();
            }

            pthread_exit(NULL);            

            return NULL;

        }

        void removeTask(ThreadTask* task)
        {
            if (task)
            {
                task->release();
                SAFE_DELETE(task);
            }

        }


	private:
		int						    mSize;
		bool					    mShutdown;
		std::vector<pthread_t>	    mThreads;
		std::queue<ThreadTask *>	mTasks;

		pthread_mutex_t	    mLock;
		pthread_cond_t	    mCond;

		bool					    mInited;

        
};


#endif

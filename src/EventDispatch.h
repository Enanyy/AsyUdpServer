#ifndef _EVENT_DISPATCH_H_
#define _EVENT_DISPATCH_H_

#include "EventID.h"
#include "EventReceiver.h"
#include "common/Singleton.h"

#include <map>
#include <list>

class EventDispatch:public Singleton<EventDispatch>
{
    public:
        ~EventDispatch()
        {
            clear();
        }
   
        template <typename T>
        bool registerReceiver(const T* object, const EventID eventId, void (T::*func)(const EventID eventId, void* data))
        {
            if(object == NULL)return false;

            auto it = mEventReceivers.find(eventId);

            if(it == mEventReceivers.end())
            {
                std::list<IEventReceiver*> receivers;
                mEventReceivers[eventId] = receivers;
                mEventReceivers[eventId].push_front(new EventReceiver<T>(object, eventId, func));
            }
            else
            { 
                bool exsit = false;

                auto iter = mEventReceivers[eventId].begin();
                for(; iter != mEventReceivers[eventId].end(); ++iter)
                {
                    if( (*iter)->isType(typeid(EventReceiver<T>) ) )
                    {
                        EventReceiver<T> * receiver = static_cast<EventReceiver<T>*>( (*iter) );
                        if( receiver && receiver->equals(typeid(EventReceiver<T>), object, eventId, func) )
                        {
                            exsit = true;
                            break;
                        }
                    }
                }

                if(exsit == false)
                {
                    mEventReceivers[eventId].push_front( new EventReceiver<T>(object, eventId, func) );
                }
            }


            return true;

        }

        template <typename T>
        void unRegisterReceiver(const T* object, const EventID eventId, void (T::*func)(const EventID eventId, void* data))
        {
            if(object == NULL)return;

            EventReceiver<T>* receiver = new EventReceiver<T>(object, eventId,func);

            auto it = mEventReceivers.find(eventId);

            if(it != mEventReceivers.end())
            {
                auto iter = it->second.begin();

                for(; iter != it->second.end(); ++iter)
                {
                    if((*iter)->isEquals(receiver))
                    {
                        break;
                    }
                }

                if(iter != it->second.end())
                {
                    IEventReceiver* recv = (*iter);
                    mEventReceivers[eventId].erase(iter);
                    delete recv;
                    recv = NULL;
                }
            }

            delete receiver;
            receiver = NULL;

        }

        void dispatch(const EventID eventId, void* data)
        {
            auto it = mEventReceivers.find(eventId);
            if(it!= mEventReceivers.end())
            {
                auto iter = mEventReceivers[eventId].cbegin();
                while(iter!= mEventReceivers[eventId].cend())
                {
                    IEventReceiver* receiver = (*iter);

                    if(receiver)
                    {
                        receiver->invoke(eventId, data);
                    }

                    iter++;
                }
            }

        }

        void clear()
        {
            auto it = mEventReceivers.begin();
            for(; it != mEventReceivers.end(); ++it)
            {
                auto iter = it->second.begin();
                for(; iter != it->second.end(); ++iter)
                {
                    IEventReceiver* recv= (*iter);
                    delete recv;
                    recv = NULL;
                }

                it->second.clear();
            }
            mEventReceivers.clear();
        }
    private:
        std::map<EventID, std::list<IEventReceiver*> > mEventReceivers;
};

#endif

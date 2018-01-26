#ifndef _EVENT_RECEIVER_H_
#define _EVENT_RECEIVER_H_
#include "EventID.h"
#include <typeinfo>
#include <stdio.h>

class IEventReceiver
{
    public:
        IEventReceiver(){}
        virtual ~IEventReceiver();

        virtual void invoke(const EventID eventId, void*data) const = 0;
        virtual bool equals(IEventReceiver* receiver)const = 0;
        virtual bool isType(const std::type_info& type)const = 0;
};

template<class  T>
class EventReceiver:public IEventReceiver
{
    public:
        typedef void(T::*Function)(const EventID, void*);

        EventReceiver(T* object,EventID eventId, Function func):mObject(object), mEventID(eventId),mReceiver(func){}
        
        ~EventReceiver()
        {
            mObject = NULL;
        }

        void invoke(const EventID eventId, void* data) const
        {
            if(eventId == mEventID && mObject)
            {
                (mObject->*mReceiver)(eventId, data);
            }
        }

        bool isType(const std::type_info& type)const { return typeid(EventReceiver<T>) == type;}

        bool equals(IEventReceiver* receiver)const
        {
            if(receiver==NULL || receiver->isType(typeid(EventReceiver<T>)) == false)
            {
                return false;
            }

            EventReceiver<T>* e = static_cast<EventReceiver<T> *>(receiver);

            return e &&( e-> mEventID == mEventID && e-> mObject == mObject && e->mReceiver == mReceiver);
        }

        bool equals(const std::type_info& type, const void* object, const EventID eventId, Function func) const
        {
            if(object == NULL || isType(type) == false)
            {
                return false;
            }

            return object == mObject && eventId == mEventID && func == mReceiver;
        }


    private:
        EventID mEventID;

        T* mObject;

        Function mReceiver;
};

#endif

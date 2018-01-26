#ifndef _MESSAGE_RECEIVER_H_
#define _MESSAGE_RECEIVER_H_

#include "User.h"
#include "MessageID.h"
#include <typeinfo>

class User;

class IMessageReceiver
{
    public:
        IMessageReceiver(){}
        virtual ~IMessageReceiver(){}

        virtual void invoke(User*, const MessageID, const char*, const unsigned int) const = 0;
        virtual bool isType(const std::type_info&)const = 0;
        virtual bool equals(IMessageReceiver*)const = 0;
};

template<class T>
class MessageReceiver: public IMessageReceiver
{
    public:
        typedef void(T::*Function)(User*, const MessageID, const char*, const unsigned int);


        MessageReceiver(T* object, MessageID id, Function func):mObject(object),mId(id),mReceiver(func) {}
     
        virtual ~MessageReceiver(){
            mObject = NULL;
            mReceiver = NULL;
        }


        void invoke(User* u, const MessageID messageId, const char* message, const unsigned int length) const
        {
            if(messageId == mId && mObject)
            {
                (mObject->*mReceiver)(u, messageId, message, length);
            }
        }
        
        bool isType(const std::type_info& type)const { return typeid(MessageReceiver<T>) == type;}
        
        bool equals(IMessageReceiver* receiver)const
        {
            if(receiver == NULL || receiver->isType(typeid(MessageReceiver<T>)) ==false)
            {
                return false;
            }

            MessageReceiver* m = static_cast<MessageReceiver<T> *>(receiver);
            return m&&(m->mObject == mObject && m->mId == mId && m->mReceiver == mReceiver);
        }
        
        bool equals(const std::type_info& type, const void* object, const MessageID messageId, Function func)const
        {
            if(object == NULL || isType(type) == false)
            {
                return false;
            }

            return mObject == object && messageId == mId && func == mReceiver;
        }

    private:
        T* mObject;

        MessageID mId;

        Function mReceiver;

};

#endif

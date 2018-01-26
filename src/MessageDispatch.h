#ifndef _MESSAGE_DISPATCH_H_
#define _MESSAGE_DISPATCH_H_

#include "public.h"
#include "User.h"
#include "MessageID.h"
#include "MessageReceiver.h"
#include "common/Singleton.h"
#include <map>
#include <list>

class MessageDispatch: public Singleton<MessageDispatch>
{
    public :

        ~MessageDispatch()
        {
            clear();
        };
        
        template<typename T>
        bool registerReceiver(T* object, const MessageID id, void(T::*func)(User*, const MessageID, const char* ,const unsigned int))
        {
            if(object == NULL)return false;
 
            auto it = mMessageReceivers.find(id);
            if(it == mMessageReceivers.end())
            {
                std::list<IMessageReceiver*> receivers;
                mMessageReceivers[id] = receivers;
                mMessageReceivers[id].push_front(new MessageReceiver<T>(object, id, func));
            }
            else
            {
                auto iter = it->second.begin();

                bool exsit = false;

                for(; iter != it->second.end(); ++iter)
                {
                    if((*iter)->isType(typeid(MessageReceiver<T>)))
                    {
                        MessageReceiver<T>* receiver = static_cast<MessageReceiver<T> *>((*iter));
                        if(receiver && receiver->equals(typeid(T), object, id, func))
                        {
                            exsit = true;
                            break;
                        }
                    }
                }

                if(exsit == false)
                {
                    mMessageReceivers[id].push_front(new MessageReceiver<T>(object, id, func));
                }
                
            }

            return true;
        }

        
        template<typename T>
        void unRegisterReceiver(T* object, const MessageID id,  void(T::*func)(User*, const MessageID, const char* ,const unsigned int))
        {
             if(object == NULL){

                return;
             }

             auto it = mMessageReceivers.find(id);
             if(it!=mMessageReceivers.end())
             {
                 auto iter = it->second.begin();
                 for(; iter != it->second.end(); ++iter)
                 {
                     if( (*iter)->isType( typeid(MessageReceiver<T>) ) )
                     {
                         MessageReceiver<T>* receiver = static_cast<MessageReceiver<T>*>( (*iter) );
                         if( receiver && receiver->equals(receiver) )
                         {
                             iter =  it->second.erase(iter);
                             delete receiver;
                             receiver = NULL;
                         }
                     }
                 }
             }
 
        }

    
        void dispatch(User* u, const MessageID messageId, const char* message, const unsigned int length)
        {
            auto it = mMessageReceivers.find(messageId);
            if(it != mMessageReceivers.end())
            {

                auto iter = it->second.cbegin();
                while(iter != it->second.cend())
                {

                    IMessageReceiver* receiver = (*iter);
                    if(receiver)
                    {
                        receiver->invoke( u, messageId, message, length);
                    }

                    ++iter;
                }
            }
        }

        void clear()
        {
            auto it = mMessageReceivers.begin();
            for(; it != mMessageReceivers.end(); ++it)
            {
                auto iter = it->second.begin();
                for(; iter != it->second.end(); ++iter)
                {
                    IMessageReceiver* receiver = (*iter);
                    delete receiver ;
                    receiver = NULL;
                }
                it->second.clear();
            }

            mMessageReceivers.clear();

        }

    private:
        std::map<MessageID, std::list<IMessageReceiver*> > mMessageReceivers;
};

#endif

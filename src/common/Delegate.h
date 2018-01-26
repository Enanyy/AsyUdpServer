#ifndef _DELEGATE_H_
#define _DELEGATE_H_

#include <list>
#include <typeinfo>

class IDelegate
{
    public:
        virtual ~IDelegate(){}
        virtual bool isType(const std::type_info&) = 0;
        virtual void invoke(void* param = NULL) = 0;
        virtual bool equals(IDelegate*) const = 0;
};

class Delegate
{
    public:
        typedef std::list<IDelegate*> ListDelegate;
        
        Delegate(){}
        ~Delegate(){}

        bool empty() const
        {
            for(auto it = mListDelegate.begin(); it != mListDelegate.end(); ++ it)
            {
                if(*it)
                {
                    return false;
                }
            }
            return true;
        }

        void clear()
        {
            for(auto it = mListDelegate.begin(); it != mListDelegate.end(); ++it)
            {
                IDelegate * d = (*it);
                if(d)
                {
                    delete d;
                    d = NULL;
                }
            }
        }

        Delegate& operator+=(IDelegate* delegate)
        {
            for(auto it = mListDelegate.begin(); it != mListDelegate.end(); ++it)
            {
                IDelegate* d = (*it);
                if(d && d->equals(delegate))
                {
                    delete delegate;
                    return *this;
                }
            }
            mListDelegate.push_back(delegate);
            return *this;
        }

        Delegate& operator-= (IDelegate* delegate)
        {
            for(auto it = mListDelegate.begin(); it != mListDelegate.end(); ++it)
            {
                IDelegate *d = (*it);
                if(d && d->equals(delegate))
                {
                    if(d == delegate)
                    {
                        delete d;
                        d = NULL;
                        break;
                    }
                }
            }
            delete delegate;
            delegate = NULL;

            return *this;
        }

        void operator()(void* param = NULL)
        {
            auto it = mListDelegate.begin();

            while(it != mListDelegate.end())
            {
                IDelegate* delegate = (*it);
                if(delegate == NULL)
                {
                    it = mListDelegate.erase(it);
                }
                else
                {
                    delegate->invoke(param);
                    ++it;
                }
            }
        }

    private:
        Delegate(const Delegate&){}
        Delegate& operator= (const Delegate&){ return *this;}

    private:
        ListDelegate mListDelegate;


};

template<class T>
class MemberDelegate:public IDelegate
{
     public:
         typedef void (T:: *VoidDelegate)(void* param);
         
         MemberDelegate(T *object, VoidDelegate func):mObject(object), mFunc(func){ }

         virtual bool isType(const std::type_info& type){return typeid(MemberDelegate<T>) == type;}

         virtual void invoke(void* param = NULL)
         {
             if(mObject)
             {
                 (mObject->*mFunc)(param);
             }
         }

         virtual bool equals(IDelegate* delegate) const 
         {
             if(delegate == NULL || delegate->isType(typeid(MemberDelegate<T>)) ==false) return false;

             MemberDelegate * cast = static_cast<MemberDelegate<T> *>(delegate);

             return cast && (cast ->mObject ==mObject && cast->mFunc == mFunc);
         }
    private:
             T* mObject;
             VoidDelegate mFunc;
};

class FunctionDelegate :public IDelegate
{
    public:
        typedef void (* VoidDelegate)(void* param);
        
        FunctionDelegate (VoidDelegate func):mFunc(func){}
        ~FunctionDelegate(){}

        virtual bool isType(const std::type_info& type)
        {
            return typeid(FunctionDelegate) == type;
        }

        virtual void invoke(void* param){ mFunc(param); }

        virtual bool equals(IDelegate* delegate) const
        {
            if(delegate == NULL || delegate->isType(typeid(FunctionDelegate)) ==false)
            {
                return false;
            }
            FunctionDelegate* cast = static_cast<FunctionDelegate*>(delegate);

            return cast && cast->mFunc == mFunc;

        }

    private:
        VoidDelegate mFunc;
};


#endif

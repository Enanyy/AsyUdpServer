#ifndef _DELEGATE_FACTORY_H_
#define _DELEGATE_FACTORY_H_

#include "Delegate.h"

class DelegateFactory
{
    public:
        inline static IDelegate* create(void (*func)(void* param))
        {
            return new FunctionDelegate(func);
        }

        template <typename T>
        inline static IDelegate* create(T* object, void (T:: *func)(void* param))
        {
            return new MemberDelegate<T>(object, func);
        }

};


#endif

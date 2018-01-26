#ifndef _MODULE_H_
#define _MODULE_H_

class Module
{
    public :
        Module(){}
        virtual ~Module(){}

        virtual void onRegister() = 0;
        virtual void onUnRegister() = 0;

};

#endif 

#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_
#include <map>
#include "Module.h"
#include "ModuleID.h"
#include "public.h"
#include "common/Singleton.h"

class Module;

class ModuleManager:public Singleton<ModuleManager>
{
    public:
        virtual ~ModuleManager();

        void init();
       
        void registerModule(const ModuleID id, Module* module);
    
        void unRegisterModule(const ModuleID id);

        const Module* getModule(const ModuleID id);

        void clear();
    private:
        std::map<ModuleID,Module*> mMapModule;
};

#endif

#include "ModuleManager.h"

#include "FrameModule.h"

ModuleManager:: ~ModuleManager()
{

}

void ModuleManager::init()
{
    registerModule(ModuleID::Frame, new FrameModule());
    
    printf("ModuleManager::init finish.\n");
}

void ModuleManager::clear()
{
    auto it = mMapModule.cbegin();

    while(it!= mMapModule.cend())
    {
        Module* module = it->second;
        SAFE_DELETE(module);
    }

    mMapModule.clear();
}

void ModuleManager::registerModule(const ModuleID id, Module* module)
{
    if(module == NULL)
    {
        return;
    }

    const Module* p = getModule(id);
    if(p)
    {
        SAFE_DELETE(p);
        return;
    }

    module->onRegister();

    mMapModule.insert(std::pair<ModuleID, Module*>(id, module));

    printf("ModuleManager::registerModle id = %d.\n",(int)id);
}

void ModuleManager:: unRegisterModule(const ModuleID id)
{
    auto it = mMapModule.find(id);
    if(it!= mMapModule.end())
    {
        it->second->onUnRegister();

        SAFE_DELETE(it->second);
        mMapModule.erase(it);
    }
}


const Module* ModuleManager::getModule(const ModuleID id)
{
    auto it = mMapModule.find(id);
  
    if(it!= mMapModule.end())
    {
        return it->second;
    }

    return NULL;
}

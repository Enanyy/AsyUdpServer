#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "../common/Singleton.h"

class SceneManager: public Singleton<SceneManager>
{

    public:
        SceneManager();
        ~SceneManager();

};

#endif

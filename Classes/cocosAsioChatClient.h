#ifndef __COCOSASIOCHATCLIENT_H__
#define __COCOSASIOCHATCLIENT_H__

#include "cocos2d.h"

class cocosAsioChatClient : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(cocosAsioChatClient);
};

#endif // __HELLOWORLD_SCENE_H__

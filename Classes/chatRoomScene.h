#pragma once

#include "cocos2d.h"

class chatRoomScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(chatRoomScene);
};
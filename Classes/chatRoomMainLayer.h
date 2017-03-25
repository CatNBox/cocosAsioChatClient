#pragma once

#include "cocos2d.h"

class chatRoomMainLayer : public cocos2d::Layer
{
public:
	//chatRoomMainLayer();
	//~chatRoomMainLayer();
	virtual bool init();

	CREATE_FUNC(chatRoomMainLayer);

private:
	void initInputForm();
	void initMemberForm();
	void initChatOutputForm();
	void menuCloseCallback(cocos2d::Ref * pSender);
};
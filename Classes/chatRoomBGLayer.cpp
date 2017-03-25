#include "chatRoomBGLayer.h"

bool chatRoomBGLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto bgSpr = cocos2d::Sprite::create("roomBack.jpg");
	bgSpr->setAnchorPoint(cocos2d::Vec2(0, 0));

	this->addChild(bgSpr);

	return true;
}

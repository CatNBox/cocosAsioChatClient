#include "mainScBGlayer.h"


USING_NS_CC;

bool mainScBGLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto backgroundSpr = Sprite::create("moemoe-kyunBACK.png");
	backgroundSpr->setAnchorPoint(Vec2(0, 0));

	addChild(backgroundSpr);

	return true;
}


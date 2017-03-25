#include "chatRoomScene.h"

//Layers
#include "chatRoomBGLayer.h"
#include "chatRoomMainLayer.h"


cocos2d::Scene * chatRoomScene::createScene()
{
	auto scene = chatRoomScene::create();

	auto bgLayer = chatRoomBGLayer::create();
	auto mainLayer = chatRoomMainLayer::create();
	mainLayer->setName("mainLayer");

	scene->addChild(bgLayer, 0);
	scene->addChild(mainLayer, 1);

	return scene;
}

bool chatRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	return true;
}

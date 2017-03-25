#include "chatRoomMainLayer.h"
#include "networkManager.h"
#include "popupLayer.h"

USING_NS_CC;

bool chatRoomMainLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(chatRoomMainLayer::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	//closeItem->setPosition(Vec2(100, 300));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	initInputForm();
	initMemberForm();
	initChatOutputForm();

	return true;
}

void chatRoomMainLayer::initInputForm()
{
}

void chatRoomMainLayer::initMemberForm()
{
}

void chatRoomMainLayer::initChatOutputForm()
{
}

void chatRoomMainLayer::menuCloseCallback(cocos2d::Ref * pSender)
{
	networkManager::getInstance()->disConnect();
	Sleep(1000);
	//Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}
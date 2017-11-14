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

	auto inputbarSpr = Sprite::create("inputBar.png");
	inputbarSpr->setAnchorPoint(Vec2::ZERO);
	inputbarSpr->setPosition(Vec2::ZERO);

	auto inputbarTF = ui::TextField::create("", "fonts/Marker Felt.tff", 20);
	inputbarTF->setSize(Size::Size(600, 40));
	inputbarTF->setTextAreaSize(Size::Size(600, 40));
	inputbarTF->setTouchSize(Size::Size(600, 40));
	inputbarTF->setTouchAreaEnabled(true);
	inputbarTF->setTouchEnabled(true);
	inputbarTF->setCursorEnabled(true);
	inputbarTF->setCursorChar('@');
	inputbarTF->setPlaceHolderColor(Color4B::BLACK);
	inputbarTF->setUnifySizeEnabled(true);
	inputbarTF->setDeleteBackward(true);
	inputbarTF->setTextColor(Color4B::BLACK);
	inputbarTF->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	inputbarTF->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
		//TextFieldTTF::createWithTTF("1",
		//"fonts/Marker Felt.tff",
		//30);//,	Size::ZERO, Size::Size(500, 40), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
	inputbarTF->setAnchorPoint(Vec2::ZERO);
	inputbarTF->setPosition(Vec2(10,20));

	inputbarSpr->addChild(inputbarTF);

	this->addChild(inputbarSpr, 1);
	//this->addChild(inputbarTF, 1);

	//initInputForm();
	initMemberForm();
	initChatOutputForm();

	return true;
}

void chatRoomMainLayer::initInputForm()
{
	auto inputbarSpr = Sprite::create("inputBar.png");
	inputbarSpr->setAnchorPoint(Vec2::ZERO);
	inputbarSpr->setPosition(Vec2::ZERO);

	auto inputbarTF = TextFieldTTF::createWithTTF("", "fonts/Marker Felt.tff", 30, Size::Size(500, 40), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
	
	this->addChild(inputbarSpr, 1);
	this->addChild(inputbarTF, 1);
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
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}
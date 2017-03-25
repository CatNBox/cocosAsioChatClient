#include "mainScMenuLayer.h"
#include "networkManager.h"
#include "commonDataDef.h"
#include "popupLayer.h"
#include <string>

USING_NS_CC;

bool mainScMenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(mainScMenuLayer::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	//버튼
	auto btnConnect = MenuItemImage::create("btnConnect.png", "inputPannel1.png", 
		CC_CALLBACK_1(mainScMenuLayer::menuBtnConnectCallback, this));
	btnConnect->setAnchorPoint(Vec2::ZERO);
	btnConnect->setPosition(362, 100);

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->addChild(btnConnect);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF("Chatting for Cocos2d-x Example", "fonts/Marker Felt.ttf", 24);

	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	this->addChild(label, 1);

	//에디트박스
	mServerAddr = cocos2d::ui::EditBox::create(Size::Size(300, 60), cocos2d::ui::Scale9Sprite::create("inputPannel2.png"));
	mServerAddr->setFontColor(Color3B::BLACK);
	mServerAddr->setFontSize(30);
	mServerAddr->setPlaceHolder("Server");
	mServerAddr->setPlaceholderFontColor(cocos2d::Color3B::BLACK);
	mServerAddr->setPlaceholderFont("fonts/Marker Felt.ttf", 30);
	mServerAddr->setMaxLength(15);
	mServerAddr->setAnchorPoint(Vec2::ZERO);
	mServerAddr->setPosition(Vec2(362, 400));
	mServerAddr->setDelegate(this);

	this->addChild(mServerAddr);

	mEditName = cocos2d::ui::EditBox::create(Size::Size(300,60), cocos2d::ui::Scale9Sprite::create("inputPannel1.png"));
	mEditName->setFontColor(Color3B::BLACK);
	mEditName->setFontSize(30);
	mEditName->setMaxLength(8);
	mEditName->setPlaceHolder("ChatUserName");
	mEditName->setPlaceholderFontColor(cocos2d::Color3B::BLACK);
	mEditName->setPlaceholderFont("fonts/Marker Felt.ttf", 30);
	mEditName->setAnchorPoint(Vec2::ZERO);
	mEditName->setPosition(Vec2(362, 270));
	mEditName->setInputFlag(ui::EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS);
	mEditName->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mEditName->setDelegate(this);

	this->addChild(mEditName);
	

	return true;
}


void mainScMenuLayer::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}

void mainScMenuLayer::menuBtnConnectCallback(cocos2d::Ref * pSender)
{
	if (strlen(mServerAddr->getText()) <= 6)
	{
		auto popup = popupLayer::create();
		popup->showPopup(this, popupCode::PLZ_INSERT_SERV_ADDR);
	}
	else if (strlen(mEditName->getText()) > 8)
	{
		auto popup = popupLayer::create();
		popup->showPopup(this, popupCode::PLZ_INSERT_USER_NAME);
	}

	//서버연결
	networkManager::getInstance()->connectServer(mEditName->getText(), mServerAddr->getText());
}

void mainScMenuLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox * edixBox)
{
}

void mainScMenuLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox * editBox)
{
}

void mainScMenuLayer::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
}

void mainScMenuLayer::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
}

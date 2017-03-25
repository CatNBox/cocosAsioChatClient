#include "popupLayer.h"
#include "commonDataDef.h"

popupLayer::popupLayer()
{
}

bool popupLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	mParentLayer = NULL;

	mBgSpr = cocos2d::Sprite::create("popupRect.png");
	mBgSpr->setAnchorPoint(cocos2d::Vec2::ZERO);
	mBgSpr->setPosition(312, 100);

	this->addChild(mBgSpr);

	return true;
}

void popupLayer::showPopup(cocos2d::Layer * pParentLayer, const int popCode)
{
	//팝업분기
	if (popCode == popupCode::PLZ_INSERT_SERV_ADDR)
	{
		mPopContent = cocos2d::Label::createWithTTF("Server Address is incorrect", "fonts/Marker Felt.ttf", 24);
	}
	else if(popCode == popupCode::PLZ_INSERT_USER_NAME)
	{
		mPopContent = cocos2d::Label::createWithTTF("UserName is Incorrect", "fonts/Marker Felt.ttf", 24);
	}
	else if (popCode == popupCode::FAIL_TO_CONNECT)
	{
		mPopContent = cocos2d::Label::createWithTTF("Connect Failure", "fonts/Marker Felt.ttf", 24);
	}

	mPopContent->setPosition(200, 300);
	mPopContent->setTextColor(cocos2d::Color4B::BLACK);
	mBgSpr->addChild(mPopContent);

	//버튼세팅
	mBtnOK = cocos2d::MenuItemImage::create("inputPannel1.png", "inputPannel2.png",
		CC_CALLBACK_1(popupLayer::menuBtnOKCallback, this));
	mBtnOK->setPosition(200, 100);

	auto menu = cocos2d::Menu::create(mBtnOK, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	mBgSpr->addChild(menu,3);

	//show up
	mParentLayer = pParentLayer;

	if (mParentLayer) 
	{
		mParentLayer->setTouchEnabled(false);
		mParentLayer->setKeyboardEnabled(false);
		mParentLayer->addChild(this, layerZOrder::POPUP);
	}
	else
	{
		cocos2d::Director::getInstance()->getRunningScene()->addChild(this, layerZOrder::POPUP);
	}

	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);
}

void popupLayer::closePopup()
{
	if (mParentLayer)
	{
		mParentLayer->setTouchEnabled(true);
		mParentLayer->setKeyboardEnabled(true);
	}

	this->removeFromParent();
}

void popupLayer::menuBtnOKCallback(cocos2d::Ref * pSender)
{
	closePopup();
}

#pragma once

#include "cocos2d.h"

class popupLayer : public cocos2d::Layer
{
public:
	popupLayer();
	virtual bool init();

	void showPopup(cocos2d::Layer *pParentLayer, const int popCode);
	void closePopup();

	CREATE_FUNC(popupLayer);

private:
	void menuBtnOKCallback(cocos2d::Ref *pSender);

	cocos2d::Sprite *mBgSpr;
	cocos2d::Label *mPopContent;
	cocos2d::Layer *mParentLayer;
	cocos2d::MenuItemImage *mBtnOK;
	//short contentCode;

};
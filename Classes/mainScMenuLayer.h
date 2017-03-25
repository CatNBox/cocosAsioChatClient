#pragma once

#include "cocos2d.h"
#include "ui\UIEditBox\UIEditBox.h"
#include "ui\UIButton.h"
#include "ui\UITextField.h"
#include "ui\UIScrollView.h"

class mainScMenuLayer : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	cocos2d::ui::EditBox *mEditName;
	cocos2d::ui::EditBox *mServerAddr;
	//ui::Button *mServerSeleter;

	CREATE_FUNC(mainScMenuLayer);

private:
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuBtnConnectCallback(cocos2d::Ref* pSender);

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox *edixBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox *editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);
}; 
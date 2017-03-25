#include "networkManager.h"
#include "chatRoomScene.h"
#include "chatProtocol.h"
#include "cocos2d.h"
#include "popupLayer.h"
#include "commonDataDef.h"

networkManager* networkManager::mInstance = NULL;

networkManager * networkManager::getInstance()
{
	if (!mInstance)
	{
		mInstance = new networkManager();
		mInstance->init();
	}

	return mInstance;
}

bool networkManager::init()
{
	return true;
}

void networkManager::connectServer(const char *pName, const char *pEndpoint)
{
	mUserName = pName;

	auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(pEndpoint), PORT_NUMBER);
	mEndPoint = endpoint;


	mAsioChatClient = new asioChatClient(*mIoServ);
	mAsioChatClient->connect(mEndPoint);

	//스케줄 io_service.run()
	//auto _service_thread = std::move(std::thread([this]() {
	//	mIoServ->run();
	//}));

	//boost::thread thread(boost::bind(&boost::asio::io_service::run, &mIoServ));

	auto schedule = cocos2d::Director::getInstance()->getScheduler();
	schedule->performFunctionInCocosThread(CC_CALLBACK_0(networkManager::runIoServ,this));
	
}

networkManager::networkManager()
{
	mIoServ = new boost::asio::io_service;
}

void networkManager::runIoServ()
{
	cocos2d::log("into runIoServ");
	//mAsioChatClient = new asioChatClient(*mIoServ);
	//mAsioChatClient->connect(mEndPoint);

	//cocos2d::log("after connect");
	//boost::thread thre(boost::bind(&boost::asio::io_service::run, mIoServ));
	mIoServ->run();
	cocos2d::log("run");
}

void networkManager::failConnect()
{
	auto popup = popupLayer::create();
	auto mainLayer = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("mainLayer");
	popup->showPopup(mainLayer,
		popupCode::FAIL_TO_CONNECT
	);
	
}

void networkManager::succeedConnect()
{
	//로그인
	PKT_REQ_IN sendPkt;
	sendPkt.init();
	strncpy_s(sendPkt.userName, MAX_NAME_LEN, mUserName, MAX_NAME_LEN - 1);
	mAsioChatClient->postSend(false, sendPkt.packetSize, (char*)&sendPkt);
	//scene전환
	auto roomScene = chatRoomScene::createScene();

	cocos2d::Director::getInstance()->replaceScene(roomScene);
	
	//포스트리시브
}

void networkManager::disConnect()
{
	mAsioChatClient->closeConnect();
}

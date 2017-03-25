#pragma once

#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include "asioChatClient.h"

class networkManager
{
public:
	static networkManager* getInstance();

	bool init();
	void connectServer(const char *pName, const char *pEndpoint);
	void failConnect();
	void succeedConnect();
	void disConnect();

private:
	networkManager();
	//~networkManager();
	void runIoServ();

	asioChatClient *mAsioChatClient;
	boost::asio::io_service *mIoServ;
	boost::asio::ip::tcp::endpoint mEndPoint;
	const char *mUserName;

	static networkManager* mInstance;
};
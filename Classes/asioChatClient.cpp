#include "asioChatClient.h"
#include "networkManager.h"
#include "cocos2d.h"

asioChatClient::asioChatClient(boost::asio::io_service & ioServ)
	://mIoServ(ioServ),
	mSocket(ioServ),
	mIsLogin(false)
{
	InitializeCriticalSectionAndSpinCount(&mLock, 4000);
}

asioChatClient::~asioChatClient()
{
	//임계구간
	EnterCriticalSection(&mLock);

	while (mSendDataQue.empty() == false)
	{
		delete[] mSendDataQue.front();
		mSendDataQue.pop_front();
	}

	LeaveCriticalSection(&mLock);
	DeleteCriticalSection(&mLock);
	//임계구간종료
}

bool asioChatClient::isConnecting()
{
	return mSocket.is_open();
}

void asioChatClient::loginOk()
{
	mIsLogin = true;
}

bool asioChatClient::isLogin()
{
	return mIsLogin;
}

void asioChatClient::connect(boost::asio::ip::tcp::endpoint endpoint)
{
	mPktBufMaker = 0;
	mSocket.async_connect(endpoint,
		boost::bind(&asioChatClient::handle_connect, this, boost::asio::placeholders::error)
	);
}

void asioChatClient::closeConnect()
{
	if (mSocket.is_open())
	{
		mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		//Sleep(500);
		//mSocket.close();
	}
}

void asioChatClient::postSend(const bool isConstantly, const int nSize, char * pData)
{
	char *pSendData = nullptr;

	//임계구간
	EnterCriticalSection(&mLock);

	if (isConstantly == false)
	{
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);

		mSendDataQue.push_back(pSendData);
	}
	else
	{
		pSendData = pData;
	}

	if (isConstantly || mSendDataQue.size() < 2)
	{
		boost::asio::async_write(mSocket, boost::asio::buffer(pSendData, nSize),
			boost::bind(&asioChatClient::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
		);
	}

	LeaveCriticalSection(&mLock);
	//임계구간 종료
	
}

void asioChatClient::postReceive()
{
	memset(&mRecvBuf, '\0', sizeof(mRecvBuf));

	mSocket.async_read_some(
		boost::asio::buffer(mRecvBuf),
		boost::bind(&asioChatClient::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void asioChatClient::handle_connect(const boost::system::error_code & errCode)
{
	cocos2d::log("handle_connect in");
	if (!errCode)
	{
		//접속성공 메세지

		networkManager::getInstance()->succeedConnect();
		auto schedule = cocos2d::Director::getInstance()->getScheduler();
		schedule->performFunctionInCocosThread(CC_CALLBACK_0(asioChatClient::postReceive, this));
		//postReceive();
	}
	else
	{
		networkManager::getInstance()->failConnect();
		//접속실패 메세지
	}
	cocos2d::log("handle_connect out");
}

void asioChatClient::handle_write(const boost::system::error_code & errCode, size_t bytes_transferred)
{
	//임계구간 시작
	EnterCriticalSection(&mLock);

	delete[] mSendDataQue.front();
	mSendDataQue.pop_front();

	char *pData = nullptr;

	if (mSendDataQue.empty() == false)
	{
		pData = mSendDataQue.front();
	}

	LeaveCriticalSection(&mLock);
	//임계구간 종료

	if (pData != nullptr)
	{
		PACKET_HEADER *pHeader = (PACKET_HEADER*)pData;
		postSend(true, pHeader->packetSize, pData);
	}
}

void asioChatClient::handle_receive(const boost::system::error_code & errCode, size_t bytes_transfered)
{
	if (errCode)//에러코드 발생
	{
		if (errCode == boost::asio::error::eof)
		{
			//서버 연결이 끊어짐 메세지
		}
		else
		{
			//에러메세지
		}

		closeConnect();
	}
	else        //에러코드 발생하지않음
	{
		memcpy(&mPktBuf[mPktBufMaker], mRecvBuf.data(), bytes_transfered);

		int nPktData = mPktBufMaker + bytes_transfered;
		int nReadData = 0;

		while (nPktData > 0)
		{
			if (nPktData < sizeof(PACKET_HEADER))
			{
				break;
			}

			PACKET_HEADER *pHeader = (PACKET_HEADER*)&mPktBuf[nReadData];

			if (pHeader->packetSize <= nPktData)
			{
				processPacket(&mPktBuf[nReadData]);

				nPktData -= pHeader->packetSize;
				nReadData += pHeader->packetSize;
			}
			else
			{
				break;
			}
		}

		if (nPktData > 0)
		{
			char tempBuf[MAX_RECEIVE_BUFFER_LEN] = { 0, };
			memcpy(&tempBuf[0], &mPktBuf[nReadData], nPktData);
			memcpy(&mPktBuf[0], &tempBuf[0], nPktData);
		}

		mPktBufMaker = nPktData;

		postReceive();
	}

}

void asioChatClient::processPacket(const char * pData)
{
	PACKET_HEADER* pHeader = (PACKET_HEADER*)pData;

	switch (pHeader->packetID)
	{
	case RES_IN:
		{
			PKT_RES_IN* pPkt = (PKT_RES_IN*)pData;

			loginOk();

			//접속 성공여부 출력
		}
		break;
	case NOTICE_CHAT:
		{
			PKT_NOTICE_CHAT* pPkt = (PKT_NOTICE_CHAT*)pData;

			//메세지 출력
		}
		break;
	default:
		break;
	}
}

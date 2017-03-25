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
	//�Ӱ豸��
	EnterCriticalSection(&mLock);

	while (mSendDataQue.empty() == false)
	{
		delete[] mSendDataQue.front();
		mSendDataQue.pop_front();
	}

	LeaveCriticalSection(&mLock);
	DeleteCriticalSection(&mLock);
	//�Ӱ豸������
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

	//�Ӱ豸��
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
	//�Ӱ豸�� ����
	
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
		//���Ӽ��� �޼���

		networkManager::getInstance()->succeedConnect();
		auto schedule = cocos2d::Director::getInstance()->getScheduler();
		schedule->performFunctionInCocosThread(CC_CALLBACK_0(asioChatClient::postReceive, this));
		//postReceive();
	}
	else
	{
		networkManager::getInstance()->failConnect();
		//���ӽ��� �޼���
	}
	cocos2d::log("handle_connect out");
}

void asioChatClient::handle_write(const boost::system::error_code & errCode, size_t bytes_transferred)
{
	//�Ӱ豸�� ����
	EnterCriticalSection(&mLock);

	delete[] mSendDataQue.front();
	mSendDataQue.pop_front();

	char *pData = nullptr;

	if (mSendDataQue.empty() == false)
	{
		pData = mSendDataQue.front();
	}

	LeaveCriticalSection(&mLock);
	//�Ӱ豸�� ����

	if (pData != nullptr)
	{
		PACKET_HEADER *pHeader = (PACKET_HEADER*)pData;
		postSend(true, pHeader->packetSize, pData);
	}
}

void asioChatClient::handle_receive(const boost::system::error_code & errCode, size_t bytes_transfered)
{
	if (errCode)//�����ڵ� �߻�
	{
		if (errCode == boost::asio::error::eof)
		{
			//���� ������ ������ �޼���
		}
		else
		{
			//�����޼���
		}

		closeConnect();
	}
	else        //�����ڵ� �߻���������
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

			//���� �������� ���
		}
		break;
	case NOTICE_CHAT:
		{
			PKT_NOTICE_CHAT* pPkt = (PKT_NOTICE_CHAT*)pData;

			//�޼��� ���
		}
		break;
	default:
		break;
	}
}

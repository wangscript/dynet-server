
#include "NetClient.h"
#include "NetSocket.h"

/*
*
*	Detail: no
*   
*  Created by hzd 2014-8-4
*
*/

NetClient::NetClient(int32 nMaxRecivedSize/* = 4 * 1024*/,int32 nMaxSendoutSize /*= 16 * 1024*/,int32 nMaxRecivedCacheSize /*= 64 * 1024*/,int32 nMaxSendoutCacheSize /*= 64 * 1024*/):m_rGameSocket(*this,nMaxRecivedSize,nMaxSendoutSize,nMaxRecivedCacheSize,nMaxSendoutCacheSize)
{
	
}

NetClient::~NetClient(void)
{
}


void NetClient::Start()
{

	thread t(boost::bind(&NetClient::HandleStart,this));
	this_thread::yield();
	t.swap(m_cServiceThread);

}

void NetClient::HandleStart()
{
	// Use thread group can add code to connect more servers 
	// here can SetConnect more socket connect
	SetConnect(m_rGameSocket);

	thread_group tg;
	tg.create_thread(boost::bind( &NetClient::Run , this ) );
	this_thread::yield();
	tg.join_all(); // Waitting threads to complete

}

void NetClient::SetAddress( const char* ip , uint16 port )
{
	boost::system::error_code ec;
	m_cServerAddr = tcp::endpoint( address::from_string( ip , ec ) , port );
	assert(!ec);
}

void NetClient::SetConnect(NetSocket& rSocket)
{
	rSocket.async_connect(m_cServerAddr,boost::bind(&NetClient::HandleConnect,this,boost::asio::placeholders::error,&rSocket));
}


void NetClient::HandleConnect(const boost::system::error_code& error,NetSocket* pSocket)
{
	if(error)
	{ 
		SetConnect(*pSocket);
	}else{
		cout << "Connected Success.................ok" << endl;
		pSocket->Run();
		cout << "Socket io event start.................ok" << endl;
		(m_pOnMsgConnected)(*pSocket);
	}
}


void NetClient::SetHandler( PNetServerEnterHandler enter ,  PNetServerOnMsgHandler msg , PNetServerExistHandler exit )
{
	m_pOnMsgConnected	= enter;
	m_pOnMsgRecevied	= msg;
	m_pOnMsgDisconnect	= exit;
}


void NetClient::Run()
{
	while (true)
	{
		boost::system::error_code ec;
		//try
		//{
			run(ec);
			break;
		//}catch( std::exception& e )
		//{
		//}
	}
}

void NetClient::Update(double nDelay)
{
	int32 nBodyLen = 0;
	NetMsgHead* pHead = NULL;
	switch (m_rGameSocket.ReadMsg(&pHead,nBodyLen))
	{
	case MSG_READ_INVALID:
		{
			(m_pOnMsgDisconnect)(m_rGameSocket);
			m_rGameSocket.Disconnect();
			m_rGameSocket.Clear();
		}
		break;
	case MSG_READ_OK:
		{
			(m_pOnMsgRecevied)(m_rGameSocket,pHead,nBodyLen);
			m_rGameSocket.RemoveMsg(PACKAGE_HEADER_SIZE + nBodyLen );
		}
		break;
	case MSG_READ_WAITTING:
	case MSG_READ_REVCING:
		break;
	}

}



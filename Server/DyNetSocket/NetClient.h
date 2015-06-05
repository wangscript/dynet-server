#ifndef SWA_BASE_NETCLIENT_H_
#define SWA_BASE_NETCLIENT_H_

#include "NetSocket.h"

/*
 *
 *	Detail: SocketÕ®–≈Client¿‡
 *   
 *  Created by hzd 2014-8-4
 *
 */

class DLLAPI NetClient : public io_service
{
public:

	friend class NetSocket;

	NetClient(int32 nMaxRecivedSize = 4 * 1024,int32 nMaxSendoutSize = 16 * 1024,int32 nMaxRecivedCacheSize = 64 * 1024,int32 nMaxSendoutCacheSize = 64 * 1024);
	virtual ~NetClient(void);
	void SetAddress( const char* pIp , uint16 nPort );
	void SetHandler( PNetServerEnterHandler pEnter ,  PNetServerOnMsgHandler pMsg , PNetServerExistHandler pExit );
	void Start();
	void Update(double nDelay);
	NetSocket& GetSocket() {
		return m_rGameSocket;
	}

protected:

	void HandleStart();
	void HandleConnect(const boost::system::error_code& error,NetSocket* rSocket);
	void Run();
	void SetConnect(NetSocket& rSocket);
	void HandleConnect(const boost::system::error_code& error);

private:
		
	PNetServerEnterHandler		m_pOnMsgConnected;
	PNetServerOnMsgHandler		m_pOnMsgRecevied;
	PNetServerExistHandler		m_pOnMsgDisconnect;

	tcp::endpoint			m_cServerAddr;
	NetSocket				m_rGameSocket;
	thread					m_cServiceThread;

};

#endif


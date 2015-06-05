#ifndef CLIENT_PLAYER_H_
#define CLIENT_PLAYER_H_

#include "Client_PCH.h"
#include "../DyNetSocket/NetClient.h"

enum 
{
	MAX_CLIENT_COUNT = 1,
};


class ClientPlayer
{
public:
	ClientPlayer(void);
	~ClientPlayer(void);

	void SendMsg( NetMsgHead* msg ,int32 nSize);
	void SetSocket( NetSocket& socket );
	NetSocket* GetSocket();
	uint64 GetUid()
	{
		return 0;
	}
	const char*	GetEncryptKey()
	{
		return m_arrEncryptKey;
	}

	void SetEncryptKey(const char arrNewKey[MAX_ENCRYPT_LENTH])
	{
		memcpy(m_arrEncryptKey,arrNewKey,MAX_ENCRYPT_LENTH);
	}

protected:

	char m_arrEncryptKey[MAX_ENCRYPT_LENTH];//通信密钥
	NetSocket*  mSocket;
	
};


class ClientPlayerMgr : public BaseSingle<ClientPlayerMgr>
{
public:

	ClientPlayerMgr()
	{
		for (int32 i = 0; i < MAX_CLIENT_COUNT ;++i)
		{
			m_arrClientPlayer[i] = NULL;
			m_arrNetClient[i] = NULL;
		}
	}
	
	void AddSocket(NetClient* pNetClient, ClientPlayer* pPlayer)
	{
		int32 nIndex = pNetClient->GetSocket().SID() - 1; // socketSID从1开始 
		m_arrNetClient[nIndex] = pNetClient;
		m_arrClientPlayer[nIndex] = pPlayer;
	}

	ClientPlayer* GetClientPlayer(NetSocket& rSocket)
	{
		return m_arrClientPlayer[rSocket.SID() - 1];
	}

	void Update()
	{
		for (int32 i = 0; i < MAX_CLIENT_COUNT; ++i)
		{
			if(m_arrNetClient[i])
			{
				m_arrNetClient[i]->Update(0.0);
			}		
		}
	}

private:

	NetClient*		m_arrNetClient[MAX_CLIENT_COUNT];
	ClientPlayer*	m_arrClientPlayer[MAX_CLIENT_COUNT];

};


#endif


#include "Encrypt.h"
#include "ClientSession.h"
#include "../DyNetSocket/NetSocket.h"
#include "LogManager.h"
#include "ServerSession.h"

ClientSession::ClientSession(int32 nClientSessionID):m_nSessionID(nClientSessionID)
{
	memcpy(m_arrEncryptKey,EncryptDefualt,MAX_ENCRYPT_LENTH);
}

ClientSession::~ClientSession()
{
}

void ClientSession::SetForFep0(NetSocket& rSocket,ServerSession* pWsSession)
{
	m_pSocket = &rSocket;
	m_pWs = pWsSession;
}

void ClientSession::SetForFep1(ServerSession* pLsSession)
{
	m_pLs = pLsSession;
}

void ClientSession::SetForFep2(ServerSession* pSsSession)
{
	m_pSs = pSsSession;
}

void ClientSession::SetForLsSs(ServerSession* pWsSession,ServerSession* pFepSession,ServerSession* pDpSession)
{
	m_pWs = pWsSession;
	m_pFep = pFepSession;
	m_pDp = pDpSession;
}

void ClientSession::SetForDp1(ServerSession* pWsSession, ServerSession* pLsSession)
{
	m_pWs = pWsSession;
	m_pLs = pLsSession;
}

void ClientSession::SetForDp2(ServerSession* pSsSession)
{
	m_pSs = pSsSession;
}

void ClientSession::SetForWs1(ServerSession* pFepSession, ServerSession* pLsSession, ServerSession* pDpSession)
{
	m_pFep = pFepSession;
	m_pLs = pLsSession;
	m_pDp = pDpSession;
}
void ClientSession::SetForWs2(ServerSession* pSsSession)
{
	m_pSs = pSsSession;
}

int32 ClientSession::SessionID()
{
	return m_nSessionID;
}

void ClientSession::SendMsgToClient(NetMsgHead* pMsg,int32 nSize)
{
	static char arrBuffer[65536];
	memcpy(&arrBuffer[0], pMsg, nSize);
	Encrypt::XorCode(nSize,m_arrEncryptKey,arrBuffer, nSize);
	pMsg = (NetMsgHead*)&arrBuffer[0];
	SendMsg(m_pSocket, pMsg, nSize);
}

void ClientSession::SendMsgToWs(NetMsgHead* pMsg, int32 nSize)
{
	ASSERT(m_pWs);
	SendMsg(m_pWs->GetNetSocket(), pMsg, nSize);
}

void ClientSession::SendMsgToSs(NetMsgHead* pMsg, int32 nSize)
{
	ASSERT(m_pSs); // 由ws判断，如果ss为空，则不会调用该语句 
	SendMsg(m_pSs->GetNetSocket(), pMsg, nSize);
}

void ClientSession::SendMsgToLs(NetMsgHead* pMsg, int32 nSize)
{
	ASSERT(m_pLs);
	SendMsg(m_pLs->GetNetSocket(), pMsg, nSize);
}

void ClientSession::SendMsgToDp(NetMsgHead* pMsg, int32 nSize)
{
	ASSERT(m_pDp);
	SendMsg(m_pDp->GetNetSocket(), pMsg, nSize);
}

void ClientSession::SendMsgToFep(NetMsgHead* pMsg, int32 nSize)
{
	ASSERT(m_pFep);
	SendMsg(m_pFep->GetNetSocket(), pMsg, nSize);
}

void ClientSession::SendMsg(NetSocket* pSocket,NetMsgHead* pMsg,int32 nSize)
{
	ASSERT(pSocket);
	pSocket->ParkMsg(pMsg, nSize);
	pSocket->SendMsg();
}

void ClientSession::Exist()
{
	m_pSocket->SetWillColse();
}

void ClientSession::SetStatus(int32 nStatus)
{
	m_nStatus = nStatus;
}

int32 ClientSession::Status()
{
	return m_nStatus;
}

void ClientSession::Update(int32 nSrvTime)
{

}

void ClientSession::SetOnServerType(int32 nServerType)
{
	m_nOnServerType = nServerType;
}

int32 ClientSession::OnServerType()
{
	return m_nOnServerType;
}

void ClientSession::SetEncryptKey(char arrNewKey[MAX_ENCRYPT_LENTH])
{
	memcpy(m_arrEncryptKey, arrNewKey, MAX_ENCRYPT_LENTH);
}

void ClientSession::GetEncryptKey(char o_arrEncryptKey[MAX_ACCOUNT_LENG])
{
	memcpy(o_arrEncryptKey, m_arrEncryptKey, MAX_ENCRYPT_LENTH);
}


ObjPool<ClientSession> ClientSessionMgr::s_cClientSessionFactory;

ClientSessionMgr::ClientSessionMgr()
{
}

ClientSessionMgr::~ClientSessionMgr()
{

}

ClientSession* ClientSessionMgr::AddSession(int32 nClientSessionID)
{
	
	ClientSessionMapType::iterator it = m_mapClientSession.find(nClientSessionID);
	if(it == m_mapClientSession.end())
	{
		ClientSession* pSession = s_cClientSessionFactory.CreateObj(nClientSessionID);
		m_mapClientSession.insert(std::make_pair(nClientSessionID,pSession));
		return pSession;
	}else
	{
		return NULL;
	}

}


void ClientSessionMgr::RemoveSession(int32 nFepSessionID)
{
	ClientSessionMapType::iterator it = m_mapClientSession.find(nFepSessionID);
	if(it != m_mapClientSession.end())
	{
		ClientSession* pSession = it->second;
		m_mapClientSession.erase(it);
		s_cClientSessionFactory.DestroyObj(pSession);
		pSession = NULL;
	}
	else
	{
		ASSERT(0);
	}
}


ClientSession*	ClientSessionMgr::GetSession(int32 nFepSessionID)
{
	ClientSessionMapType::iterator it = m_mapClientSession.find(nFepSessionID);
	if(it == m_mapClientSession.end())
	{
		return NULL;
	}
	return it->second;
}


void ClientSessionMgr::SendToAll(NetMsgHead* pMsg,int32 nSize)
{
	ClientSessionMapType::iterator it = m_mapClientSession.begin();
	ClientSessionMapType::iterator itEnd = m_mapClientSession.end();
	for(; it != itEnd; ++it)
	{
		//it->second->SendMsg(pMsg,nSize);//TODO
	}
}

void ClientSessionMgr::Update(uint32 nDelay)
{
	ClientSessionMapType::iterator it = m_mapClientSession.begin();
	ClientSessionMapType::iterator itEnd = m_mapClientSession.end();
	ClientSession* pSession = NULL;
	for(; it != itEnd; ++it)
	{
		pSession = it->second;
		pSession->Update(nDelay);
	}
}

int32 ClientSessionMgr::ConnectedSessions()
{
	return m_mapClientSession.size();
}


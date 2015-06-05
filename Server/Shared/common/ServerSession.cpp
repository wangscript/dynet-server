#include "ServerSession.h"
#include "../DyNetSocket/NetServer.h"
#include "Encrypt.h"
#include "CommonInternal.h"
#include "CommonServerOpt.h"
#include "ServerInfoMgr.h"

ServerSession::ServerSession(NetSocket* pNetSocket):m_nServerID(0),m_pSocket(pNetSocket)
{

}

ServerSession::~ServerSession()
{
}

void ServerSession::SendMsg(NetMsgHead* pMsg,int32 nSize)
{
	m_pSocket->ParkMsg(pMsg, nSize);
	m_pSocket->SendMsg();
}

void ServerSession::Exist()
{
	m_pSocket->SetWillColse();
}

void ServerSession::SetStatus(int32 nStatus)
{
	m_nStatus = nStatus;
}

int32 ServerSession::Status()
{
	return m_nStatus;
}

void ServerSession::Update(int32 nSrvTime)
{

}

NetSocket* ServerSession::GetNetSocket()
{
	return m_pSocket;
}


void ServerSession::SetRemoteServerType(EServerType eServerType)
{
	m_eRemoteServerType = eServerType;
}

EServerType ServerSession::ServerTypeLocal()
{
	return m_eLocalServerType;
}

void ServerSession::SetLocalServerType(EServerType eServerType)
{
	m_eLocalServerType = eServerType;
}

EServerType ServerSession::ServerTypeRemote()
{
	return m_eRemoteServerType;
}

void ServerSession::SetServerID(int32 nServerID)
{
	m_nServerID = nServerID;
}

int32 ServerSession::ServerID()
{
	return m_nServerID;
}


void ServerSession::SendMsgToLs(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() == ESERVER_TYPE_LS)
	{
		SendMsg(pMsg,nSize);
	}
}

void ServerSession::SendMsgToWs(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() == ESERVER_TYPE_WS)
	{
		SendMsg(pMsg,nSize);
	}
}

void ServerSession::SendMsgToSs(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() == ESERVER_TYPE_SS)
	{
		SendMsg(pMsg,nSize);
	}
}

void ServerSession::SendMsgToDp(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() == ESERVER_TYPE_DP)
	{
		SendMsg(pMsg,nSize);
	}
}

void ServerSession::SendMsgToFep(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() == ESERVER_TYPE_FEP)
	{
		SendMsg(pMsg,nSize);
	}
}

void ServerSession::SendMsgToClient(NetMsgHead* pMsg, int32 nSize)
{
	if(ServerTypeRemote() != ESERVER_TYPE_DP)
	{
		SendMsg(pMsg,nSize);
	}
}


void ServerSession::LoginToWs()
{
	if(ServerTypeRemote() == ESERVER_TYPE_WS)
	{
		ServerInfoOpt& rLocalInfo = NetServerOpt::Instance()->GetLocalServerInfo();
		SSReqLoginWs sMsg;
		strcpy_s(sMsg.arrServerAddr,rLocalInfo.strHost.c_str());
		sMsg.nServerPort = rLocalInfo.nPort;
		sMsg.nServerID = rLocalInfo.nID;
		sMsg.nServerType = rLocalInfo.nType;
		SendMsg(&sMsg,sMsg.GetPackLength());
	}
}

void ServerSession::LoginResult(bool bIsSuccess)
{
	SSRepLogined sMsg;
	sMsg.nResult = bIsSuccess ? SSRepLogined::SUCCESS:SSRepLogined::FAIL;
	SendMsg(&sMsg,sMsg.GetPackLength());
}

void ServerSession::LoginToS()
{
	ServerInfoOpt& rLocalInfo = NetServerOpt::Instance()->GetLocalServerInfo(); 
	SSReqLoginS sMsg;
	sMsg.nServerID = rLocalInfo.nID; // 告诉对方的自己的信息，而本session的serverID与serverType则代表对方的 
	sMsg.nServerType = rLocalInfo.nType;
	SendMsg(&sMsg,sMsg.GetPackLength());
}

ObjPool<ServerSession>	ServerSessionMgr::s_cServerSessionFactory;
ServerSessionMgr::ServerSessionMgr()
{
}

ServerSessionMgr::~ServerSessionMgr()
{

}

ServerSession* ServerSessionMgr::CreateSession(int32 nSessionID,NetSocket& rSocket)
{
	ServerSession* pSession = s_cServerSessionFactory.CreateObj(&rSocket);
	if(!m_mapSocketIDSession.insert(std::make_pair(nSessionID,pSession)).second)
	{
		s_cServerSessionFactory.DestroyObj(pSession);
		return NULL;
	}
	return pSession;
}

bool ServerSessionMgr::InitSession(int32 nServerID,ServerSession* pSession)
{
	if(!m_mapServerIdSession.insert(std::make_pair(nServerID,pSession)).second)
	{
		return false;
	}
	pSession->SetServerID(nServerID);
	return true;
}


void ServerSessionMgr::RemoveSession(uint32 nServerID)
{
	ServerSession* pSession = NULL;
	ServerIdSessionMapType::iterator it = m_mapServerIdSession.find(nServerID);
	if(it != m_mapServerIdSession.end())
	{
		pSession = it->second;
		m_mapServerIdSession.erase(it);
	}else
	{
		ASSERT(0);
	}
	ASSERT(pSession);
	SocketIDSessionMapType::iterator it2 = m_mapSocketIDSession.find(pSession->GetNetSocket()->SID());
	if(it2 != m_mapSocketIDSession.end())
	{
		m_mapSocketIDSession.erase(it2);
	}
	else
	{
		ASSERT(0);
	}
	s_cServerSessionFactory.DestroyObj(pSession);
}

void ServerSessionMgr::RemoveSession(NetSocket& rSokcet)
{
	ServerSession* pSession = NULL;
	SocketIDSessionMapType::iterator it = m_mapSocketIDSession.find(rSokcet.SID());
	if(it != m_mapSocketIDSession.end())
	{
		pSession = it->second;
		m_mapSocketIDSession.erase(it);
	}
	else
	{
		ASSERT(0);
	}
	ASSERT(pSession);
	ServerIdSessionMapType::iterator it2 = m_mapServerIdSession.find(pSession->ServerID());
	if(it2 != m_mapServerIdSession.end())
	{
		m_mapServerIdSession.erase(it2);
	}else
	{
		ASSERT(0);
	}
	s_cServerSessionFactory.DestroyObj(pSession);
}


ServerSession*	ServerSessionMgr::GetSession(uint32 nServerID)
{
	ServerIdSessionMapType::iterator iter = m_mapServerIdSession.find(nServerID);
	if(iter == m_mapServerIdSession.end())
	{
		return NULL;
	}
	return iter->second;
}

ServerSession* ServerSessionMgr::GetSession(NetSocket& rSokcet)
{
	SocketIDSessionMapType::iterator it = m_mapSocketIDSession.find(rSokcet.SID());
	if(it == m_mapSocketIDSession.end())
	{
		return NULL;
	}
	return it->second;
}

void ServerSessionMgr::SendToAll(NetMsgHead* pMsg,int32 nSize)
{
	ServerIdSessionMapType::iterator iter = m_mapServerIdSession.begin();
	ServerIdSessionMapType::iterator iterend = m_mapServerIdSession.end();
	for(; iter != iterend; ++iter)
	{
		iter->second->SendMsg(pMsg,nSize);
	}
}

void ServerSessionMgr::Update(uint32 nDelay)
{
	ServerIdSessionMapType::iterator it = m_mapServerIdSession.begin();
	ServerIdSessionMapType::iterator itEnd = m_mapServerIdSession.end();
	ServerSession* pPlayer = NULL;
	for(; it != itEnd; ++it)
	{
		pPlayer = it->second;
		pPlayer->Update(nDelay);
	}
}

int32 ServerSessionMgr::CountConnected()
{
	return m_mapServerIdSession.size();
}

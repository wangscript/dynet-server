#include "ServerConnectMgr.h"


ServerConnectMgr::ServerConnectMgr()
{
}


ServerConnectMgr::~ServerConnectMgr()
{
}

bool ServerConnectMgr::ConnectToServer(int32 nServerID,int32 nServerType,const char arrHost[32],int32 nPort,NetMsgEnter fEnter,NetMsgOn fMsg,NetMsgExit fExit)
{

	NetClientInfoVecterType::iterator it = m_vecNetClientInfo.begin();
	NetClientInfoVecterType::iterator itEnd = m_vecNetClientInfo.end();
	for (; it != itEnd;++it)
	{
		if((*it)->nServerID == nServerID)
			return false;
	}

	NetClient* pNetClient = new NetClient();
	pNetClient->SetAddress(arrHost, nPort);
	pNetClient->SetHandler(fEnter,fMsg,fExit);
	pNetClient->Start();

	NetClientInfo* pConn = new NetClientInfo();
	pConn->pNetClient = pNetClient;
	pConn->nServerID = nServerID;
	pConn->nServerType = nServerType;
	m_vecNetClientInfo.push_back(pConn);

	return true;

}

void ServerConnectMgr::Update(int32 nSrvTime)
{
	// 检查要断开的NetClient
	DisconnectServerIDVectorType::iterator itServerID = m_vecNetClient.begin();
	DisconnectServerIDVectorType::iterator itEndServerID = m_vecNetClient.end();
	for (;itServerID != itEndServerID;++itServerID)
	{
		uint32 nServerID = *itServerID;
		for(NetClientInfoVecterType::iterator it = m_vecNetClientInfo.begin();it != m_vecNetClientInfo.end();)
		{
			if((*it)->nServerID == nServerID)
			{
				delete (*it)->pNetClient;
				(*it)->pNetClient = NULL;
				it = m_vecNetClientInfo.erase(it);
			}else
			{
				++it;
			}
		}
	}
	m_vecNetClient.clear();

	NetClientInfoVecterType::iterator it = m_vecNetClientInfo.begin();
	NetClientInfoVecterType::iterator itEnd = m_vecNetClientInfo.end();
	for(; it != itEnd; ++it)
	{
		NetClient* pNetClient = (*it)->pNetClient;
		if(pNetClient)
		{
			pNetClient->Update(nSrvTime);//1
		}
	}

}

// 删除一个Server
void ServerConnectMgr::RemoveServer(int32 nServerID) // 2 由1中进入2
{
	m_vecNetClient.push_back(nServerID);
}

NetClientInfo* ServerConnectMgr::GetNetClient(NetSocket& rSocket)
{
	NetClientInfoVecterType::iterator it = m_vecNetClientInfo.begin();
	NetClientInfoVecterType::iterator itEnd = m_vecNetClientInfo.end();
	for(; it != itEnd; ++it)
	{
		if((*it)->pNetClient->GetSocket().SID() == rSocket.SID())
		{
			return (*it);
		}
	}
	return NULL;
}

int32 ServerConnectMgr::CountConnected()
{
	return m_vecNetClientInfo.size();
}
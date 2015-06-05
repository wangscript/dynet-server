#include "ServerInfoMgr.h"
#include "LogManager.h"
#include "CommonDefine.h"
#include "CommonW2L.h"
#include "CommonInternal.h"
#include "ServerSession.h"

ServerInfoMgr::ServerInfoMgr()
{
}


ServerInfoMgr::~ServerInfoMgr()
{
}

// 获得某服务器信息
const ServerInfo* ServerInfoMgr::GetServerInfo(uint32 nServerID)
{
	ServerInfoMapType::const_iterator it = m_mapServerInfo.find(nServerID);
	if(it == m_mapServerInfo.end())
		return NULL;
	return &(it->second);
}

// 登录一新服务器
bool ServerInfoMgr::OnServerLogin(ServerSession* pSession,const char arrHost[32],int32 nPort,int32 nServerID,uint32 nServerType)
{
	ServerInfo sServerInfo;
	sServerInfo.pServerSession = pSession;
	memcpy(sServerInfo.arrHosrt, arrHost, 32);
	sServerInfo.nPort = nPort;
	sServerInfo.nServerID = nServerID;
	sServerInfo.nServerType = nServerType;
	sServerInfo.nLoadCount = 0;

	ServerInfoMapType::iterator it = m_mapServerInfo.find(nServerID);
	if(it == m_mapServerInfo.end())
	{
		m_mapServerInfo.insert(std::make_pair(nServerID,sServerInfo));
		return true;
	}else
	{
		FLOG_INFO("Repeat server_id login %d",nServerID);
		ASSERT(0);
		return false;
	}
}

// 某服务器退出
void ServerInfoMgr::OnServerExit(uint32 nServerID)
{
	ServerInfoMapType::iterator it = m_mapServerInfo.find(nServerID);
	if(it != m_mapServerInfo.end())
	{
		m_mapServerInfo.erase(it);
	}
	else
	{
		FLOG_INFO("Not find server_id OnServerExit %d", nServerID);
	}
}

// 负载+1
void ServerInfoMgr::OnLoadAddOne(uint32 nServerID)
{
	ServerInfoMapType::iterator it = m_mapServerInfo.find(nServerID);
	if(it != m_mapServerInfo.end())
	{
		it->second.nLoadCount += 1;
	}
}

// 负载-1
void ServerInfoMgr::OnLoadSubOne(uint32 nServerID)
{
	ServerInfoMapType::iterator it = m_mapServerInfo.find(nServerID);
	if(it != m_mapServerInfo.end())
	{
		it->second.nLoadCount += 1;
	}
}

// 获得服务器列表
void ServerInfoMgr::GetServerInfoList(int32 nServerType,ServerInfoVecType& o_rServerList)
{
	for (ServerInfoMapType::iterator it = m_mapServerInfo.begin(); it != m_mapServerInfo.end();++it)
	{
		if(it->second.nServerType == nServerType)
		{
			o_rServerList.push_back(it->second);
		}
	}
}

ServerSession* ServerInfoMgr::GetLoadLestSession(int32 nServerType)
{
	ServerInfo* pServerInfo = NULL;
	ServerInfoVecType o_vecServerList;
	GetServerInfoList(nServerType, o_vecServerList);
	for(ServerInfoVecType::iterator it = o_vecServerList.begin(); it != o_vecServerList.end(); ++it)
	{
		if(pServerInfo == NULL)
		{
			pServerInfo = &(*it);
			if(pServerInfo->nLoadCount == 0)
				break;
		}
		else
		{
			if((*it).nLoadCount < pServerInfo->nLoadCount)
				pServerInfo = &(*it);
		}
	}
	ASSERT(pServerInfo);
	return pServerInfo->pServerSession;
}

// 广播服务器信息
void ServerInfoMgr::BoardcastDpAndFepServerInfo2LsAndSs(uint32 nLoginServerID)
{

	ServerInfoMapType::iterator itNew = m_mapServerInfo.find(nLoginServerID);
	if(itNew == m_mapServerInfo.end())
	{
		return;
	}

	// 如果登录的是dp或fep，则向ls与ss发送该服务器信息
	// 如果登录的是ls或ss，则向它发送dp与fep的服务器列表
	ServerInfo& rLoginServerInfo = itNew->second;
	if(rLoginServerInfo.nServerType == ESERVER_TYPE_DP || rLoginServerInfo.nServerType == ESERVER_TYPE_FEP)
	{
		ServerInfoData sServerInfoData;
		memcpy(sServerInfoData.arrHosrt, rLoginServerInfo.arrHosrt, MAX_SERVERINFO_LIST);
		sServerInfoData.nPort = rLoginServerInfo.nPort;
		sServerInfoData.nServerID = rLoginServerInfo.nServerID;
		sServerInfoData.nServerType = rLoginServerInfo.nServerType;

		ServerInfoMapType::iterator it = m_mapServerInfo.begin();
		ServerInfoMapType::iterator itEnd = m_mapServerInfo.end();
		for(; it != itEnd; ++it)
		{
			ServerInfo& sInfo = it->second;
			switch(sInfo.nServerType)
			{
			case ESERVER_TYPE_LS:
			case ESERVER_TYPE_SS:
			{
				SSRepServerInfoList sServerList;
				sServerList.nCount = 1;
				memcpy(&sServerList.sServerInfoData[0], &sServerInfoData, sizeof(sServerInfoData));
				sInfo.pServerSession->SendMsg(&sServerList, sServerList.GetPackLength());
			}
			break;
			default:
			break;
			}
		}
	}else if (rLoginServerInfo.nServerType == ESERVER_TYPE_LS || rLoginServerInfo.nServerType == ESERVER_TYPE_SS)
	{
		int32 nCount = 0;
		ServerInfoData sServerInfoData[MAX_SERVERINFO_LIST];
		ServerInfoMapType::iterator it = m_mapServerInfo.begin();
		ServerInfoMapType::iterator itEnd = m_mapServerInfo.end();
		for(; it != itEnd; ++it)
		{
			ServerInfo& sInfo = it->second;
			switch(sInfo.nServerType)
			{
			case ESERVER_TYPE_FEP:
			case ESERVER_TYPE_DP:
			{
				strcpy_s(sServerInfoData[nCount].arrHosrt,sInfo.arrHosrt);
				sServerInfoData[nCount].nPort = sInfo.nPort;
				sServerInfoData[nCount].nServerID = sInfo.nServerID;
				sServerInfoData[nCount].nServerType = sInfo.nServerType;
				nCount++;
			}
			break;
			default:
			break;
			}
		}
		
		if(rLoginServerInfo.nServerType == ESERVER_TYPE_LS || rLoginServerInfo.nServerType == ESERVER_TYPE_SS)
		{
			SSRepServerInfoList sServerList;
			sServerList.nCount = nCount;
			memcpy(sServerList.sServerInfoData, sServerInfoData, sizeof(sServerInfoData));
			rLoginServerInfo.pServerSession->SendMsgToClient(&sServerList, sServerList.GetPackLength());
		}
	}

}

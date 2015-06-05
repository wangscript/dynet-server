#ifndef __BASE_MSG_HANDLER_H_
#define __BASE_MSG_HANDLER_H_

#include "BaseDefine.h"
#include "CommonDefine.h"


class NetSocket;
struct NetMsgHead;
class BaseSession;

typedef boost::function<void(BaseSession*, const NetMsgHead*,int32 nSize)> HandlePackType;

// 协议与对应的调用函数 
struct MsgHandlerInfo
{
	MsgHandlerInfo(int32 _nPacketSize, const HandlePackType& _rHandler) :nPacketSize(_nPacketSize), rHandler(_rHandler)
	{
	}
	int32 nPacketSize;   // 数据大小
	HandlePackType rHandler;	// 处理函数
};

typedef	map< uint32, MsgHandlerInfo > NetMsgHandlerMap;

// 用 
class BaseMsgHandler 
{
public:

	virtual void OnNetMsgEnter(NetSocket& rSocket) = 0;

	virtual void OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize) = 0;

	virtual void OnNetMsgExit(NetSocket& rSocket) = 0;

	const MsgHandlerInfo* GetMsgHandler(uint32 nProtocol)
	{
		NetMsgHandlerMap::iterator it = m_mapClientMsgHandler.find(nProtocol);
		if(it == m_mapClientMsgHandler.end())
		{
			return NULL;
		}
		return &(it->second);
	}
protected:
	virtual bool RegisterMessage(uint16 nMsgIdx, int32 nPacketSize, const HandlePackType& rHandler)
	{
		if(!m_mapClientMsgHandler.insert(std::make_pair(nMsgIdx, MsgHandlerInfo(nPacketSize, rHandler))).second)
		{
			ASSERT(0);
		}
		return true;
	}

private:
	NetMsgHandlerMap m_mapClientMsgHandler;
};


#define FOR_WS_ON_MSG_ENTER(rSocket,nLocalServerType) ServerSession* pSession = ServerSessionMgr::Instance()->CreateSession(rSocket.SID(),rSocket); \
ServerInfoOpt& rRemoteInfo = NetServerOpt::Instance()->GetRemoteServerInfo();\
if(!ServerSessionMgr::Instance()->InitSession(rRemoteInfo.nID, pSession))\
{\
	FLOG_ERROR("Repeat ServerID:%d",rRemoteInfo.nID); \
	pSession->Exist(); \
	return; \
}\
	pSession->SetRemoteServerType(ESERVER_TYPE_WS); \
	pSession->SetLocalServerType(nLocalServerType); \
	pSession->SetStatus(ESERVER_STATUS_CONNETED); \
	pSession->LoginToWs();

#define FOR_WS_ON_MSG(rSocket,pHead) const MsgHandlerInfo* pMsgHandlerInfo = GetMsgHandler(pHead->nType);\
if(pMsgHandlerInfo == NULL)\
{\
	FLOG_INFO("Not Find Protocol:%d", pHead->nType); \
	rSocket.SetWillColse(); \
	return; \
}\
else\
{\
	FLOG_INFO("OnNetMsg Protocol: %d", pHead->nType); \
	(pMsgHandlerInfo->rHandler)((BaseSession*)(ServerSessionMgr::Instance()->GetSession(rSocket)),pHead,nSize); \
}

#define FOR_WS_ON_MSG_EXIT(rSocket) if(ServerSession* pSession = ServerSessionMgr::Instance()->GetSession(rSocket))\
{\
	ServerSessionMgr::Instance()->RemoveSession(pSession->ServerID()); \
	FLOG_INFO("OnNetMsgExit:%d", pSession->ServerID()); \
}\
else\
{\
	FLOG_INFO("Can not find session by socket!"); \
	ASSERT(0); \
}

#define FOR_WS_REP_LOGINED(pBaseSession,pMsg)	if(pBaseSession->Status() != ESERVER_STATUS_CONNETED)\
{\
	FLOG_INFO("pBaseSession->Status() != EWORLD_STATUS_CONNECTED");\
	return;\
}\
const SSRepLogined* pPacket = static_cast<const SSRepLogined*>(pMsg);\
if(pPacket->nResult == SSRepLogined::SUCCESS)\
{\
	pBaseSession->SetStatus(ESERVER_STATUS_LOGINED);\
}\
else\
{\
	FLOG_ERROR("Login TO ws Faild!");\
	return;\
}

#define FOR_WS_REP_SERVER_LIST(pSession,pMsg) if(pSession->Status() != ESERVER_STATUS_LOGINED)\
{\
	FLOG_INFO("pSession->Status() != ESERVER_STATUS_LOGINED");\
	return;\
}\
const SSRepServerInfoList* pPacket = static_cast<const SSRepServerInfoList*>(pMsg);\
for(int32 i = 0; i < pPacket->nCount; ++i)\
{\
	const ServerInfoData& rInfo = pPacket->sServerInfoData[i];\
	if(rInfo.nServerType == ESERVER_TYPE_DP || rInfo.nServerType == ESERVER_TYPE_FEP)\
	{\
		ServerConnectMgr* pConnMgr = ServerConnectMgr::Instance();\
		bool bResult = pConnMgr->ConnectToServer(rInfo.nServerID, rInfo.nServerType, rInfo.arrHosrt, rInfo.nPort,\
			boost::bind(&ForServerMsgHandler::OnNetMsgEnter, ForServerMsgHandler::Instance(), _1),\
			boost::bind(&ForServerMsgHandler::OnNetMsg, ForServerMsgHandler::Instance(), _1, _2,_3),\
			boost::bind(&ForServerMsgHandler::OnNetMsgExit, ForServerMsgHandler::Instance(), _1));\
		if(!bResult)\
		{\
			FLOG_ERROR("Connect fail!");\
		}\
	}\
}

#define FOR_S_ON_MSG(rSocket,pMsg)	if(ServerSession* pSession = ServerSessionMgr::Instance()->GetSession(rSocket))\
{\
	const MsgHandlerInfo* pMsgHandlerInfo = GetMsgHandler(pMsg->nType);\
	if(pMsgHandlerInfo == NULL)\
	{\
		FLOG_ERROR("Not Find Protocol:%d", pMsg->nType);\
		rSocket.SetWillColse();\
		return;\
	}\
	else\
	{\
		FLOG_INFO("OnNetMsg Protocol: %d", pMsg->nType);\
		(pMsgHandlerInfo->rHandler)((BaseSession*)(pSession),pMsg,nSize);\
	}\
}\
else\
{\
	FLOG_INFO("Can not find session");\
	ASSERT(0);\
}

#define FOR_S_S_ON_MSG_ENTER(rSocket,nServerType) 	ServerSession* pSession = ServerSessionMgr::Instance()->CreateSession(rSocket.SID(),rSocket);\
pSession->SetStatus(ESERVER_STATUS_CONNETED);\
pSession->SetLocalServerType(nServerType);

#define FOR_S_C_ON_MSG_ENTER(rSocket,nLocalServerType) 	ServerSession* pSession = ServerSessionMgr::Instance()->CreateSession(rSocket.SID(), rSocket); \
ASSERT(pSession); \
NetClientInfo* pInfo = ServerConnectMgr::Instance()->GetNetClient(rSocket); \
ASSERT(pInfo); \
uint32 nServerID = pInfo->nServerID; \
if(!ServerSessionMgr::Instance()->InitSession(nServerID, pSession))\
{\
FLOG_INFO("Reap serverID"); \
pSession->Exist(); \
return; \
}\
pSession->SetStatus(ESERVER_STATUS_CONNETED); \
pSession->SetRemoteServerType((EServerType)(pInfo->nServerType)); \
pSession->SetLocalServerType((EServerType)nLocalServerType); \
pSession->LoginToS();

#define FOR_S_C_ON_MSG_EXIT(rSocket) if(ServerSession* pSession = ServerSessionMgr::Instance()->GetSession(rSocket))\
	{\
	ServerSessionMgr::Instance()->RemoveSession(pSession->ServerID());\
	ServerConnectMgr::Instance()->RemoveServer(pSession->ServerID());\
	FLOG_INFO("OnNetMsgExit:%d", pSession->ServerID());\
}\
else\
{\
	FLOG_INFO("Can not find session by socket!");\
	ASSERT(0);\
}

#define FOR_S_S_ON_MSG_EXIT(rSocket) if(ServerSession* pSession = ServerSessionMgr::Instance()->GetSession(rSocket))\
	{\
		ServerSessionMgr::Instance()->RemoveSession(pSession->ServerID());\
		FLOG_INFO("OnNetMsgExit:%d", pSession->ServerID());\
	}\
else\
{\
	FLOG_INFO("Can not find session by socket!");\
	ASSERT(0);\
}

#define FOR_S_S_REQLOGIN(pSession,pHead) ServerSession* pServerSession = static_cast<ServerSession*>(pSession);\
const SSReqLoginS* pPacket = static_cast<const SSReqLoginS*>(pHead);\
bool bResult = true;\
if(pSession->Status() != ESERVER_STATUS_CONNETED)\
{\
	FLOG_INFO("Reapt serverid serversion");\
	bResult = false;\
}\
else\
{\
	uint32 nServerID = pPacket->nServerID;\
	if(!ServerSessionMgr::Instance()->InitSession(nServerID, pServerSession))\
	{\
		bResult = false;\
		FLOG_INFO("Reapt serverid serversion");\
	}\
	else\
	{\
		pServerSession->SetServerID(pPacket->nServerID);\
		pServerSession->SetRemoteServerType((EServerType)(pPacket->nServerType));\
	}\
}\
pServerSession->LoginResult(bResult);\
if(!bResult)\
pServerSession->Exist();

#define FOR_LS_SS_NOFITY_SERVER_INFO(pSession,pMsg) const SSSessionNofitySInfo* pPacket = static_cast<const SSSessionNofitySInfo*>(pMsg);\
ServerSession* pFepServerSession = ServerSessionMgr::Instance()->GetSession(pPacket->nFepServerID);\
ServerSession* pDpServerSession = ServerSessionMgr::Instance()->GetSession(pPacket->nDpServerID);\
ASSERT(pFepServerSession && pDpServerSession);\
ClientSession* pClientSession = ClientSessionMgr::Instance()->AddSession(pPacket->nClientSessionID);\
ASSERT(pClientSession);\
pClientSession->SetForLsSs((ServerSession*)pSession, pFepServerSession, pDpServerSession);\
pClientSession->SetStatus(ECLIENT_STATUS_NOTIFY_INITED);

#endif

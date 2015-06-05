#include "ForServerMsgHandler.h"

#include "LogManager.h"
#include "ServerInfoMgr.h"
#include "ClientSession.h"
#include "ServerSession.h"

#include "BaseProcHandler.h"
#include "ClientProcHandler.h"
#include "DpServerProcHandler.h"
#include "FepServerProcHandler.h"
#include "SceneServerProcHandler.h"
#include "LoginServerProcHandler.h"

#include "CommonF2W.h"
#include "CommonL2W.h"
#include "CommonD2W.h"
#include "CommonC2W.h"
#include "CommonInternal.h"

ForServerMsgHandler::ForServerMsgHandler()
{
	// common 需要用到内部协议
	// 内部协议，即不分不同servertype，都去做统一的事情，如登录，返回列表，ping等等操作 
#define REGISTER_INTERNAL_MESSAGE(msg_idx,cls,handler) \
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForServerMsgHandler::handler, this, _1, _2, _3)); \
	}

	REGISTER_INTERNAL_MESSAGE(PRO_SS_REQ_LOGIN_WS,SSReqLoginWs,ReqLogin);
	REGISTER_INTERNAL_MESSAGE(PRO_SS_REQ_LOAD_LEST,SSReqLoadLest,ReqRoadLest);
	REGISTER_INTERNAL_MESSAGE(PRO_SS_REQ_CLIENT_EXIT,SSNofityClientExit,NofityClientExit);

#undef REGISTER_INTERNAL_MESSAGE

	// dp
#define REGISTER_DP_MESSAGE(msg_idx,cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&DpServerProcHandler::handler, DpServerProcHandler::Instance(), _1, _2, _3)); \
	}

#undef REGISTER_DP_MESSAGE

	// ls
#define REGISTER_LS_MESSAGE(msg_idx,cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&LoginServerProcHandler::handler, LoginServerProcHandler::Instance(), _1, _2, _3)); \
	}
	
#undef REGISTER_LS_MESSAGE

// fep
#define REGISTER_FEP_MESSAGE(msg_idx,cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&FepServerProcHandler::handler, FepServerProcHandler::Instance(), _1, _2, _3)); \
	}

#undef REGISTER_FEP_MESSAGE

	// ss
#define REGISTER_SS_MESSAGE(msg_idx,cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&SceneServerProcHandler::handler, SceneServerProcHandler::Instance(), _1, _2, _3)); \
	}

#undef REGISTER_SS_MESSAGE

	// client
#define REGISTER_CLIENT_MESSAGE(msg_idx,cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ClientProcHandler::handler, ClientProcHandler::Instance(), _1, _2, _3)); \
	}

	REGISTER_CLIENT_MESSAGE(PRO_C2W_SWITCH_SCENE,C2WSwitchScene,ReqSwitchScene); // 切换场景 

#undef REGISTER_CLIENT_MESSAGE


}


ForServerMsgHandler::~ForServerMsgHandler()
{ 
}

// 有客户端连接回调
void ForServerMsgHandler::OnNetMsgEnter(NetSocket& rSocket)
{
	FOR_S_S_ON_MSG_ENTER(rSocket,ESERVER_TYPE_WS);
}

// 有客户端消息收到
void ForServerMsgHandler::OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize)
{
	FOR_S_ON_MSG(rSocket,pMsg);
}

// 有客户端断开回调(此时socket已经回收) 
void ForServerMsgHandler::OnNetMsgExit(NetSocket& rSocket)
{     
	//---------------------------------服务组代码begin-------------------------------
	ServerSession* pSession = ServerSessionMgr::Instance()->GetSession(rSocket);
	int32 nServerID = pSession->ServerID();
	FOR_S_S_ON_MSG_EXIT(rSocket);
	ServerInfoMgr::Instance()->OnServerExit(nServerID);
	//---------------------------------服务组代码end-------------------------------
}

void ForServerMsgHandler::ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	ServerSession* pServerSession = static_cast<ServerSession*>(pSession);
	if(pServerSession->Status() != ESERVER_STATUS_CONNETED)
		return;

	const SSReqLoginWs* pPacket = static_cast<const SSReqLoginWs*>(pMsg);

	int32 nServerID = pPacket->nServerID;
	int32 nServerType = pPacket->nServerType;

	// 状态
	pServerSession->SetServerID(nServerID);
	pServerSession->SetRemoteServerType((EServerType)nServerType);
	pServerSession->SetStatus(ECLIENT_STATUS_LOGINED);

	// 记录新登入进来的server
	bool bResult = ServerInfoMgr::Instance()->OnServerLogin(pServerSession,pPacket->arrServerAddr,pPacket->nServerPort,nServerID,nServerType);
	if(!bResult)
	{
		FLOG_INFO("OnServerLogin fail serverid:%d",nServerID);
	}

	// 记录serverID与对应的session
	bResult = ServerSessionMgr::Instance()->InitSession(nServerID,pServerSession);
	if(!bResult)
	{
		FLOG_INFO("InitSession fail serverid:%d",nServerID);
	}

	// 返回登录成功
	pServerSession->LoginResult(bResult);

	// fep与dp的服务器信息需要广播给ls与ss
	if(bResult)
	{
		ServerInfoMgr::Instance()->BoardcastDpAndFepServerInfo2LsAndSs(pPacket->nServerID);
	}
	//---------------------------------服务组代码end-------------------------------

}

void ForServerMsgHandler::ReqRoadLest(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	const SSReqLoadLest* pPacket = static_cast<const SSReqLoadLest*>(pMsg);
	int32 nClientSessionID = pPacket->nClientSessionID;
	ServerSession* pFepSession = static_cast<ServerSession*>(pSession);

	// 找出分配最小的(或上一次分配的，以后再优化)
	ServerInfoMgr* pServerInfoMgr = ServerInfoMgr::Instance();
	ServerSession* pLsSession = pServerInfoMgr->GetLoadLestSession(ESERVER_TYPE_LS);
	ServerSession* pDpSession = pServerInfoMgr->GetLoadLestSession(ESERVER_TYPE_DP);
	ASSERT(pLsSession && pDpSession);

	// 第一次创建ClientSesson数据
	ClientSession* pClientSession = ClientSessionMgr::Instance()->AddSession(nClientSessionID);
	ASSERT(pClientSession);
	pClientSession->SetForWs1(pFepSession,pLsSession,pDpSession);

	// 同步到ls,dp,fep
	SSSessionNofitySInfo sNofityInfo;
	sNofityInfo.nClientSessionID = nClientSessionID;
	sNofityInfo.nLsServerID = pLsSession->ServerID();
	sNofityInfo.nDpServerID = pDpSession->ServerID();
	sNofityInfo.nFepServerID = pFepSession->ServerID();

	// 通知分配给Client的ServerID
	pClientSession->SendMsgToLs(&sNofityInfo,sNofityInfo.GetPackLength());
	pClientSession->SendMsgToDp(&sNofityInfo,sNofityInfo.GetPackLength());
	pClientSession->SendMsgToFep(&sNofityInfo,sNofityInfo.GetPackLength());

	// 负载+1
	pServerInfoMgr->OnLoadAddOne(pLsSession->ServerID());
	pServerInfoMgr->OnLoadAddOne(pDpSession->ServerID());
	pServerInfoMgr->OnLoadAddOne(pFepSession->ServerID());
	//---------------------------------服务组代码end-------------------------------

}

void ForServerMsgHandler::NofityClientExit(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	const SSNofityClientExit* pPacket = static_cast<const SSNofityClientExit*>(pMsg);
	int32 nClientSessionID = pPacket->nClientSessionID;

	ClientSession* pClientSession = ClientSessionMgr::Instance()->GetSession(nClientSessionID);
	ASSERT(pClientSession);

	// 同步或保存重要数据到dp

	// 通知ls,ss
	SSNofityClientExit sMsgExit;
	sMsgExit.nClientSessionID = nClientSessionID;

	pClientSession->SendMsgToLs(&sMsgExit,sMsgExit.GetPackLength());
	if(pClientSession->Status() == ECLIENT_STATUS_IN_SCENE)
	{
		pClientSession->SendMsgToSs(&sMsgExit,sMsgExit.GetPackLength());
	}else
	{
		// 由dp去保存退出操作
		pClientSession->SendMsgToDp(&sMsgExit,sMsgExit.GetPackLength());
	}

	// 删除
	ClientSessionMgr::Instance()->RemoveSession(nClientSessionID);

}




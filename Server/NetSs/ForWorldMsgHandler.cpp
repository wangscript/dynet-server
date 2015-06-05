#include "ForWorldMsgHandler.h"
#include "../DyNetSocket/NetSocket.h"
#include "../DyNetSocket/NetClient.h"
#include "CommonW2L.h"
#include "CommonL2W.h"

#include "LogManager.h"

#include "ProcWorldHandler.h"
#include "CommonServerOpt.h"
#include "ServerSession.h"
#include "CommonInternal.h"
#include "ServerConnectMgr.h"
#include "ForServerMsgHandler.h"
#include "ClientSession.h"


ForWorldMsgHandler::ForWorldMsgHandler()
{

	// 内部协议
#define REGISTER_INTERNAL_MESSAGE(msg_idx,cls,handler)\
	{\
	RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForWorldMsgHandler::handler, this, _1, _2, _3)); \
}

	REGISTER_INTERNAL_MESSAGE(PRO_SS_REP_LOGINED,SSRepLogined,RepLogined);	// 登录结果 
	REGISTER_INTERNAL_MESSAGE(PRO_SS_REP_SERVERINFO_LIST,SSRepServerInfoList,RepServerInfoList);	// 服务器管理信息协议 
	REGISTER_INTERNAL_MESSAGE(PRO_SS_NOFTTY_SERVERID,SSSessionNofitySInfo,NofityClientSessionInfo); // 同步分配服务器ID 

#undef REGISTER_INTERNAL_MESSAGE

}


ForWorldMsgHandler::~ForWorldMsgHandler()
{
}

// 有客户端连接回调
void ForWorldMsgHandler::OnNetMsgEnter(NetSocket& rSocket)
{
	FOR_WS_ON_MSG_ENTER(rSocket,ESERVER_TYPE_SS);
}

// 有客户端消息收到
void ForWorldMsgHandler::OnNetMsg(NetSocket& rSocket, NetMsgHead* pHead,int32 nSize)
{
	FOR_WS_ON_MSG(rSocket,pHead);
}

// 有客户端断开回调(此时socket已经回收) 
void ForWorldMsgHandler::OnNetMsgExit(NetSocket& rSocket)
{
	FOR_WS_ON_MSG_EXIT(rSocket);
}


void ForWorldMsgHandler::RepLogined(BaseSession* pBaseSession, const NetMsgHead* pMsg,int32 nSize)
{
	FOR_WS_REP_LOGINED(pBaseSession,pMsg);
}

void  ForWorldMsgHandler::RepServerInfoList(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	FOR_WS_REP_SERVER_LIST(pSession,pMsg);
}

void ForWorldMsgHandler::NofityClientSessionInfo(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	FOR_LS_SS_NOFITY_SERVER_INFO(pSession, pMsg);
}
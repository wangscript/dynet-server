#include "ForServerMsgHandler.h"
#include "../DyNetSocket/NetSocket.h"
#include "../DyNetSocket/NetClient.h"

#include "LogManager.h"
#include "BaseDefine.h"
#include "CommonServerOpt.h"
#include "ServerSession.h"
#include "CommonInternal.h"


ForServerMsgHandler::ForServerMsgHandler()
{

	// 内部协议
#define REGISTER_INTERNAL_MESSAGE(msg_idx,cls,handler)\
	{\
	RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForServerMsgHandler::handler, this, _1, _2, _3)); \
}

	REGISTER_INTERNAL_MESSAGE(PRO_SS_REQ_LOGIN_S, SSReqLoginS, ReqLogined);	// 请求登录

#undef REGISTER_INTERNAL_MESSAGE



}


ForServerMsgHandler::~ForServerMsgHandler()
{
}

// 有客户端连接回调
void ForServerMsgHandler::OnNetMsgEnter(NetSocket& rSocket)
{
	//---------------------------------服务组代码begin-------------------------------
	FOR_S_S_ON_MSG_ENTER(rSocket,ESERVER_TYPE_FEP);
	//---------------------------------服务组代码end-------------------------------
}

// 有客户端消息收到
void ForServerMsgHandler::OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	FOR_S_ON_MSG(rSocket,pMsg);
	//---------------------------------服务组代码end-------------------------------
}

// 有客户端断开回调(此时socket已经回收) 
void ForServerMsgHandler::OnNetMsgExit(NetSocket& rSocket)
{
	//---------------------------------服务组代码begin-------------------------------
	FOR_S_S_ON_MSG_EXIT(rSocket);
	//---------------------------------服务组代码end-------------------------------
}

void ForServerMsgHandler::ReqLogined(BaseSession* pSession,const NetMsgHead* pHead,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	FOR_S_S_REQLOGIN(pSession,pHead);
	//---------------------------------服务组代码end-------------------------------
}

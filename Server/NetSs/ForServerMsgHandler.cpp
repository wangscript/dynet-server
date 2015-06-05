#include "ForServerMsgHandler.h"
#include "../DyNetSocket/NetSocket.h"
#include "../DyNetSocket/NetClient.h"
#include "CommonW2F.h"
#include "CommonF2W.h"
#include "ServerConnectMgr.h"
#include "ServerSession.h"
#include "ServerInfoMgr.h"

#include "LogManager.h"
#include "CommonInternal.h"


ForServerMsgHandler::ForServerMsgHandler()
{

#define REGISTER_INTERNAL_MESSAGE(msg_idx,cls,handler)\
	{\
	RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForServerMsgHandler::handler, this, _1, _2, _3)); \
}

	REGISTER_INTERNAL_MESSAGE(PRO_SS_REP_LOGINED,SSRepLogined,RepLogined);	// 登录结果 

#undef REGISTER_INTERNAL_MESSAGE

}


ForServerMsgHandler::~ForServerMsgHandler()
{
}

// 有客户端连接回调
void ForServerMsgHandler::OnNetMsgEnter(NetSocket& rSocket)
{
	//---------------------------------服务组代码begin-------------------------------
	FOR_S_C_ON_MSG_ENTER(rSocket,ESERVER_TYPE_SS);
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
	FOR_S_C_ON_MSG_EXIT(rSocket);
	//---------------------------------服务组代码end-------------------------------
}



void ForServerMsgHandler::RepLogined(BaseSession* pBaseSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	if(pBaseSession->Status() != ESERVER_STATUS_CONNETED)
	{
		FLOG_INFO("pBaseSession->Status() != EWORLD_STATUS_CONNECTED");
		return;
	}
	const SSRepLogined* pPacket = static_cast<const SSRepLogined*>(pMsg);
	if(pPacket->nResult == SSRepLogined::SUCCESS)
	{
		pBaseSession->SetStatus(ESERVER_STATUS_LOGINED);
	}
	else
	{
		//登录失败，可以再进行第二次及第三次登录试试看
		return;
	}
	//---------------------------------服务组代码end-------------------------------

}

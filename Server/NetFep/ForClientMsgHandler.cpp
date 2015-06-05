#include "ForClientMsgHandler.h"
#include "../DyNetSocket/NetSocket.h"

#include "Encrypt.h"
#include "LogManager.h"

#include "CommonC2S.h"
#include "CommonS2C.h"
#include "CommonC2F.h"
#include "CommonF2C.h"
#include "CommonC2W.h"

#include "ClientSession.h"
#include "ServerSession.h"

#include "CommonInternal.h"
#include "CommonServerOpt.h"

// 在fep中，client的sessionID为socketID

ForClientMsgHandler::ForClientMsgHandler()
{

	// fep
#define REGISTER_FEP_MESSAGE(msg_idx, cls,handler)\
	{\
		RegisterMessage(msg_idx, sizeof(cls),boost::bind(&ForClientMsgHandler::handler, this, _1, _2,_3)); \
	}

	REGISTER_FEP_MESSAGE(PRO_C2F_ENCRYPT,C2FepEncryptInfo,ReqFepEncyptHandler);
	REGISTER_FEP_MESSAGE(PRO_C2F_CLOSE,C2FepClose,ReqFepCloseHandler);

#undef REGISTER_FEP_MESSAGE

	// ls
#define REGISTER_LS_MESSAGE(msg_idx,cls)\
	{\
	RegisterMessage(msg_idx,sizeof(cls),boost::bind(&ForClientMsgHandler::ReqSendToLoginServer,this, _1, _2,_3));\
	}
	

#undef REGISTER_LS_MESSAGE

	// ws
#define REGISTER_WS_MESSAGE(msg_idx,cls)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForClientMsgHandler::ReqSendToWorldServer, this, _1, _2,_3)); \
	}

	REGISTER_WS_MESSAGE(PRO_C2W_SWITCH_SCENE,C2WSwitchScene); // 切换新场景 

#undef REGISTER_WS_MESSAGE

	// ss
#define REGISTER_SS_MESSAGE(msg_idx,cls)\
	{\
		RegisterMessage(msg_idx, sizeof(cls), boost::bind(&ForClientMsgHandler::ReqSendToSceneServer, this, _1, _2,_3)); \
	}

#undef REGISTER_SS_MESSAGE

}


ForClientMsgHandler::~ForClientMsgHandler()
{
}

void ForClientMsgHandler::OnNetMsgEnter(NetSocket& rSocket)
{
	if(ClientSession* pSession = ClientSessionMgr::Instance()->AddSession(rSocket.SID()))
	{
		ServerInfoOpt& rRemoteInfo = NetServerOpt::Instance()->GetRemoteServerInfo();
		ServerSession* pWsServerSession = ServerSessionMgr::Instance()->GetSession(rRemoteInfo.nID);
		if(pWsServerSession == NULL)
		{
			// ws还未连接上或断开了
			pSession->Exist();
			return ;
		}
		pSession->SetForFep0(rSocket,pWsServerSession);
		pSession->SetStatus(ECLIENT_STATUS_CONNECTED);
		FLOG_INFO("OnNetMsgEnter SID: %d", rSocket.SID());
	}else
	{
		FLOG_INFO("Create clientsession fail!");
	}
}


void ForClientMsgHandler::OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	if(ClientSession* pSession = ClientSessionMgr::Instance()->GetSession(rSocket.SID()))
	{
		// 解密处理
		static char msgBuffer[65536];
		memcpy(&msgBuffer[0], pMsg,nSize);
		static char arrEncrypt[MAX_ENCRYPT_LENTH];
		pSession->GetEncryptKey(arrEncrypt);
		Encrypt::XorCode(nSize, arrEncrypt, msgBuffer, nSize);
		pMsg = (NetMsgHead*)&msgBuffer[0];

		const MsgHandlerInfo* pMsgHandlerInfo = GetMsgHandler(pMsg->nType);
		if(pMsgHandlerInfo == NULL)
		{
			FLOG_INFO("Not Find Protocol:%d", pMsg->nType);
			rSocket.SetWillColse();
			return;
		}
		else
		{
			FLOG_INFO("OnNetMsg Protocol: %d", pMsg->nType);
			(pMsgHandlerInfo->rHandler)((BaseSession*)pSession, pMsg,nSize);
		}
	}else
	{
		FLOG_INFO("Cannot found clientsession:%d",rSocket.SID());
		rSocket.SetWillColse();
	}
	//---------------------------------服务组代码end-------------------------------
}

void ForClientMsgHandler::OnNetMsgExit(NetSocket& rSocket)
{
	//---------------------------------服务组代码begin-------------------------------
	// 服务器主动断开或被客户断开，都是从这里最先开始操作
	// socket这时不能再给Client发数据,socket可能已经失效
	int32 nClientSessionID = rSocket.SID();
	FLOG_INFO("OnNetMsgExit nClientSession: %d", nClientSessionID);
	
	// 通知Ws,Ws再通知ls,ss,ss通知dp，最后是dp收到ss的信息，保存数据进行移除client
	ClientSession* pClientSession = ClientSessionMgr::Instance()->GetSession(nClientSessionID);
	ASSERT(pClientSession);

	// 发出客户端退出消息
	SSNofityClientExit sMsgExit;
	sMsgExit.nClientSessionID = nClientSessionID;
	pClientSession->SendMsgToWs(&sMsgExit, sMsgExit.GetPackLength());

	ClientSessionMgr::Instance()->RemoveSession(nClientSessionID);
	//---------------------------------服务组代码end-------------------------------
}

// 发送到ls
void ForClientMsgHandler::ReqSendToLoginServer(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	MSG_CLIENT_TO_LS(pSession, pMsg, nSize);
	//---------------------------------服务组代码end-------------------------------
}

// 发送到ss
void ForClientMsgHandler::ReqSendToSceneServer(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	MSG_CLIENT_TO_SS(pSession,pMsg,nSize);
	//---------------------------------服务组代码end-------------------------------
}

// 发送到ws
void ForClientMsgHandler::ReqSendToWorldServer(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	MSG_CLIENT_TO_WS(pSession,pMsg,nSize);
	//---------------------------------服务组代码end-------------------------------
}

void ForClientMsgHandler::ReqFepEncyptHandler(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	ClientSession* pClientSession = static_cast<ClientSession*>(pSession);
	if(pClientSession->Status() == ECLIENT_STATUS_CONNECTED )
	{
		char arrRandKey[MAX_ENCRYPT_LENTH];
		Encrypt::RandKey(arrRandKey);
		
		F2CEncryptInfo sMsgEncrypt;
		memcpy(sMsgEncrypt.arrEncryptKey, arrRandKey, MAX_ENCRYPT_LENTH);
		pClientSession->SendMsgToClient(&sMsgEncrypt,sMsgEncrypt.GetPackLength());

		// 要先发数据后才能改
		pClientSession->SetEncryptKey(arrRandKey);
		pClientSession->SetStatus(ECLIENT_STATUS_ENCRYPTED);

		// 请求分配服务器ID
		SSReqLoadLest sMsgLoadLest;
		sMsgLoadLest.nClientSessionID = pClientSession->SessionID();
		pClientSession->SendMsgToWs(&sMsgLoadLest,sMsgLoadLest.GetPackLength());

	}else
	{
		FLOG_INFO("ClientSession Status Not Eq EPLAYER_STATUS_CONNECTED");
	}
	//---------------------------------服务组代码end-------------------------------
}

void ForClientMsgHandler::ReqFepCloseHandler(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize)
{
	//---------------------------------服务组代码begin-------------------------------
	ClientSession* pClientSession = static_cast<ClientSession*>(pSession);
	pClientSession->Exist();
	//---------------------------------服务组代码begin-------------------------------
}

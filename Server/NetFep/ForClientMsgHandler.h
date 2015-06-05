#ifndef CLIENT_MSG_HANDLER_H_
#define CLIENT_MSG_HANDLER_H_

#include "BaseDefine.h"
#include "BaseSingle.h"

#include "BaseMsgHandler.h"


/*
 *
 *	Detail: 接收处理客户端（用户）连接
 *   
 * Copyright (c) Created by hzd 2015-4-27.All rights reserved
 *
 */

class ClientSession;
class NetSocket;
struct NetMsgHead;
class ForClientMsgHandler : public BaseMsgHandler, public BaseSingle<ForClientMsgHandler>
{
public:
	ForClientMsgHandler();
	~ForClientMsgHandler();

	// 有客户端连接回调
	virtual void OnNetMsgEnter(NetSocket& rSocket);

	// 有客户端消息收到
	virtual void OnNetMsg(NetSocket& rSocket, NetMsgHead* pHead,int32 nSize);

	// 有客户端断开回调(此时socket已经回收) 
	virtual void OnNetMsgExit(NetSocket& rSocket);

	// 发送到ls
	void ReqSendToLoginServer(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize);

	// 发送到ss
	void ReqSendToSceneServer(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize);

	// 发送到ws
	void ReqSendToWorldServer(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize);

	
	//-----------------------------本地处理协议---------------------------------
	// fep
	void ReqFepEncyptHandler(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize);
	void ReqFepCloseHandler(BaseSession* pSession,const NetMsgHead* pMsg,int32 nSize);
	// 预备消息(用于处理fep新clientsession获得分配ls,ls新clientsession获得dp)

private:
	

};

#define MSG_CLIENT_COMMON_CLIENTSESSIONID(_nClientSessinID,pMsg,nSize) static vector<char> vecMsgBuffer;\
{\
	vecMsgBuffer.resize(nSize); \
	memcpy(&vecMsgBuffer[0], pMsg, nSize); \
	reinterpret_cast<NetMsgHead*>(&vecMsgBuffer[0])->nClientSessionID = _nClientSessinID; \
}\

#define MSG_CLIENT_TO_WS(pSession,pMsg,nSize) ClientSession* pClientSession = static_cast<ClientSession*>(pSession); \
	MSG_CLIENT_COMMON_CLIENTSESSIONID(pClientSession->SessionID(),pMsg,nSize) ;\
	pClientSession->SendMsgToWs((NetMsgHead*)(&vecMsgBuffer[0]), vecMsgBuffer.size());

#define MSG_CLIENT_TO_SS(pSession,pMsg,nSize) ClientSession* pClientSession = static_cast<ClientSession*>(pSession); \
	MSG_CLIENT_COMMON_CLIENTSESSIONID(pClientSession->SessionID(),pMsg,nSize);\
	pClientSession->SendMsgToSs((NetMsgHead*)(&vecMsgBuffer[0]), vecMsgBuffer.size());

#define MSG_CLIENT_TO_LS(pSession,pMsg,nSize) ClientSession* pClientSession = static_cast<ClientSession*>(pSession); \
	MSG_CLIENT_COMMON_CLIENTSESSIONID(pClientSession->SessionID(),pMsg,nSize); \
	pClientSession->SendMsgToLs((NetMsgHead*)(&vecMsgBuffer[0]), vecMsgBuffer.size());


#endif




#ifndef _SERVER_HANDLER_H_
#define _SERVER_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseMsgHandler.h"

/*
 *
 *	Detail: 处理与ws，ss,ls通信消息
 *   
 * Copyright (c) Created by hzd 2015-4-27.All rights reserved
 *
 */

class NetSocket;
struct NetMsgHead;

class ForWorldMsgHandler : public BaseMsgHandler, public BaseSingle<ForWorldMsgHandler>
{

public:
	ForWorldMsgHandler();
	~ForWorldMsgHandler();

	// 有客户端连接回调
	virtual void OnNetMsgEnter(NetSocket& rSocket);

	// 有客户端消息收到
	virtual void OnNetMsg(NetSocket& rSocket, NetMsgHead* pHead,int32 nSize);

	// 有客户端断开回调(此时socket已经回收) 
	virtual void OnNetMsgExit(NetSocket& rSocket);

	// 登录结果
	void RepLogined(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

	// 服务器列表 
	void RepServerInfoList(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

	// 同步client连接的server信息
	void NofityClientSessionInfo(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

private:


};

#endif



#ifndef _SERVER_DP_H_
#define _SERVER_DP_H_

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

class ForServerMsgHandler : public BaseMsgHandler, public BaseSingle<ForServerMsgHandler>
{

public:
	ForServerMsgHandler();
	~ForServerMsgHandler();

	// 有客户端连接回调
	virtual void OnNetMsgEnter(NetSocket& rSocket);

	// 有客户端消息收到
	virtual void OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize);

	// 有客户端断开回调(此时socket已经回收) 
	virtual void OnNetMsgExit(NetSocket& rSocket);

	// 登录结果(fep或ls回调结果)
	void RepLogined(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

private:


};

#endif



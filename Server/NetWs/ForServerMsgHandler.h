#ifndef _SERVER_MSG_HANDLER_H_
#define _SERVER_MSG_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseMsgHandler.h"

/*
 *
 *	Detail: 处理dp,ss,ls(无),fep请求过来的协议
 *   
 * Copyright (c) Created by hzd 2015-4-28.All rights reserved
 *
 */

class NetSocket;
struct NetMsgHead;

// 所有的服务器列表


class ForServerMsgHandler : public BaseMsgHandler , public BaseSingle<ForServerMsgHandler>
{
public:
	ForServerMsgHandler();
	~ForServerMsgHandler();


	// 有客户端连接回调
	void OnNetMsgEnter(NetSocket& rSocket);

	// 有客户端消息收到
	void OnNetMsg(NetSocket& rSocket, NetMsgHead* pMsg,int32 nSize);

	// 有客户端断开回调(此时socket已经回收) 
	void OnNetMsgExit(NetSocket& rSocket);

	// 登录ls,fep,dp,ss登录ws 
	void ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

	// 请求获得分配的负载最低的服务器ID
	void ReqRoadLest(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

	// 客户端退出通知
	void NofityClientExit(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);


};

#endif



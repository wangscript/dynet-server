#ifndef _SEVER_SESSION_H_
#define _SEVER_SESSION_H_

#include "BaseSingle.h"
#include "CommonDefine.h"
#include "BaseSession.h"
#include "ObjPool.h"

class NetSocket;
class NetServer;
struct NetMsgHead;

enum EServerStatus
{
	ESERVER_STATUS_INIT = 0,
	ESERVER_STATUS_CONNETED,
	ESERVER_STATUS_LOGINED,
	ESERVER_STATUS_OK,
};


/*
 *
 *	Detail: 服务器与服务器之间的session
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class ServerSession : public BaseSession
{
public:

	ServerSession(NetSocket* pNetSocket);
	virtual ~ServerSession();

	/*
	 *
	 *	Detail: 唯一ID
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	int32 ServerID();

	/*
	 *
	 *	Detail: 发送消息
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	virtual void SendMsg( NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 退出
	 *   
	 *  Created by hzd 2015-1-21
	 *
	 */
	virtual void Exist();

	/*
	 *
	 *	Detail: 设置状态
	 *   
	 *  Created by hzd 2015-5-27
	 *
	 */
	virtual void SetStatus(int32 nStatus);

	/*
	 *
	 *	Detail: 获得状态
	 *   
	 *  Created by hzd 2015-5-27
	 *
	 */
	virtual int32 Status();

	/*
	 *
	 *	Detail: 定时循环，参数为延时毫秒
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void Update(int32 nSrvTime);

	/*
	 *
	 *	Detail: 获得服务器类型
	 *   
	 * Copyright (c) Created by hzd 2015-4-29.All rights reserved
	 *
	 */
	EServerType ServerTypeRemote();

	/*
	 *
	 *	Detail: 设置服务器类型
	 *   
	 * Copyright (c) Created by hzd 2015-4-29.All rights reserved
	 *
	 */
	void SetRemoteServerType(EServerType eServerType);

	/*
	 *
	 *	Detail: 本地session类型
	 *   
	 *  Created by hzd 2015-5-25
	 *
	 */
	EServerType ServerTypeLocal();
	
	/*
	 *
	 *	Detail: 设置本地session类型
	 *   
	 *  Created by hzd 2015-5-25
	 *
	 */
	void SetLocalServerType(EServerType eServerType);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015-5-20
	 *
	 */
	void SetServerID(int32 nServerID);

		/*
	 *
	 *	Detail: no
	 *   
	 * Copyright (c) Created by hzd 2015-4-28.All rights reserved
	 *
	 */
	void SendMsgToLs(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: no
	 *   
	 * Copyright (c) Created by hzd 2015-4-28.All rights reserved
	 *
	 */
	void SendMsgToWs(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: no
	 *   
	 * Copyright (c) Created by hzd 2015-4-28.All rights reserved
	 *
	 */
	void SendMsgToSs(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 发送信息到Dp
	 *   
	 *  Created by hzd 2015-5-21
	 *
	 */
	void SendMsgToDp(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 发送信息到Fep
	 *   
	 *  Created by hzd 2015-5-21
	 *
	 */
	void SendMsgToFep(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 发送信息到Client
	 *   
	 *  Created by hzd 2015-5-21
	 *
	 */
	void SendMsgToClient(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 登录到WS
	 *   
	 *  Created by hzd 2015-5-22
	 *
	 */
	void LoginToWs();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015-5-22
	 *
	 */
	void LoginResult(bool bIsSuccess);

	/*
	 *
	 *	Detail: 登录到某服务器
	 *   
	 *  Created by hzd 2015-5-22
	 *
	 */
	void LoginToS();

	/*
	 *
	 *	Detail: 获得NetSocket
	 *   
	 * Copyright (c) Created by hzd 2015-5-27.All rights reserved
	 *
	 */
	NetSocket* GetNetSocket();

protected:

private:

private:

	int32				m_nServerID;				// 所属的服务器ID
	int32				m_nSendPackCount;			// 发包数量（校验包顺序、丢包情况）
	NetSocket*			m_pSocket;					// 与socket绑定
	EServerType			m_eRemoteServerType;		// 远程服务器类型
	EServerType			m_eLocalServerType;			// 本地服务器类型

};


/*
 *
 *	Detail: ss管理器
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class ServerSessionMgr : public BaseSingle<ServerSessionMgr>
{
	typedef map<int32,ServerSession*>	SocketIDSessionMapType;	// 连接上来的Socket临时记录() 
	typedef map<int32,ServerSession*>	ServerIdSessionMapType;	// 已经登录创建的ServerSession 
public:

	ServerSessionMgr();
	~ServerSessionMgr();

	/*
	 *
	 *	Detail: 创建一个ServerSession
	 *   
	 * Copyright (c) Created by hzd 2015-5-27.All rights reserved
	 *
	 */
	ServerSession* CreateSession(int32 nSessionID,NetSocket& rSocket);

	/*
	 *
	 *	Detail: 初始化ServerSession
	 *   
	 *  Created by hzd 2015-5-27
	 *
	 */
	bool InitSession(int32 nServerID,ServerSession* pSession);

	/*
	 *
	 *	Detail: 释放Session(二选一)
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void RemoveSession(uint32 nServerID);

	/*
	 *
	 *	Detail: 释放Session(二选一)
	 *   
	 *  Created by hzd 2015-5-27
	 *
	 */
	void RemoveSession(NetSocket& rSokcet);

	/*
	 *
	 *	Detail: 获得Session
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	ServerSession*	GetSession(uint32 nServerID);

	/*
	 *
	 *	Detail: 该函数要函数到是不是同一个socket来取数据 
	 *   
	 *  Created by hzd 2015-5-25
	 *
	 */
	ServerSession* GetSession(NetSocket& rSokcet);
		  
	/*
	 *
	 *	Detail: 广播消息
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void SendToAll(NetMsgHead* pMsg,int32 nSize);

	/*
	 *
	 *	Detail: 更新
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Update(uint32 nDelay);

	/*
	 *
	 *	Detail: 统计被服务器连接数
	 *   
	 *  Created by hzd 2015-5-22
	 *
	 */
	int32 CountConnected();

private:

	SocketIDSessionMapType				m_mapSocketIDSession;		// 记录刚连接上来的Socket
	ServerIdSessionMapType				m_mapServerIdSession;		// 记录已经登录ServerSession，serverID找Session
	static ObjPool<ServerSession>		s_cServerSessionFactory;	// ServerSession工厂 

};

#endif


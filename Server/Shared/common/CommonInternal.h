#ifndef __COMMON_INTERNAL_H_
#define __COMMON_INTERNAL_H_

/*
 *
 *	Detail: 内部通信协议
 *   
 *  Created by hzd 2015-5-21
 *
 */

#include "CommonDefine.h"

enum EProReqInternal
{
	PRO_SS_REQ_LOGIN_WS		= PRO_INTERNAL_BASE + 0,	// 登录处理，包括,所有服务器登录到ws
	PRO_SS_REQ_LOGIN_S		= PRO_INTERNAL_BASE + 1,	// 登录到dp,fep
	PRO_SS_REQ_LOAD_LEST	= PRO_INTERNAL_BASE + 2,	// 分配服务器信息 
	PRO_SS_REQ_PING_S		= PRO_INTERNAL_BASE + 3,	// PING协议客户端ping服务端，ping后，就socket->setTimeout,收到数据，则取消 
	PRO_SS_REQ_CLIENT_EXIT	= PRO_INTERNAL_BASE + 4,    // 客户端退出
	
};

enum EProRepInternal
{
	PRO_SS_REP_LOGINED			= PRO_INTERNAL_BASE + 50, // 登录结果 
	PRO_SS_REP_SERVERINFO_LIST	= PRO_INTERNAL_BASE + 51, // 服务器信息
	PRO_SS_NOFTTY_SERVERID		= PRO_INTERNAL_BASE + 52, // 更新服务器信息
	PRO_SS_REP_PING_S			= PRO_INTERNAL_BASE + 53, // 服务器返回PING的结果，将socket取消定时器 
};


#pragma pack(push,1)


//------------------------req------------------------------

struct SSReqLoginWs : public NetMsgHead
{
	SSReqLoginWs():NetMsgHead(PRO_SS_REQ_LOGIN_WS)
	{
	}
	char arrServerAddr[32]; // 服务器地址 
	int32 nServerPort;		// 端口
	int32 nServerID;		// 服务器标识ID
	int32 nServerType;		// 服务器类型
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

struct SSReqLoginS : public NetMsgHead 
{
	SSReqLoginS():NetMsgHead(PRO_SS_REQ_LOGIN_S)
	{

	}
	int32 nServerID;		// 服务器标识ID
	int32 nServerType;		// 服务器类型
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

struct SSReqLoadLest : public NetMsgHead
{
	SSReqLoadLest():NetMsgHead(PRO_SS_REQ_LOAD_LEST)
	{

	}
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

struct SSNofityClientExit : public NetMsgHead
{
	SSNofityClientExit(): NetMsgHead(PRO_SS_REQ_CLIENT_EXIT)
	{

	}
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};


//--------------------------rep------------------------------------

struct SSRepLogined : public NetMsgHead
{
	SSRepLogined():NetMsgHead(PRO_SS_REP_LOGINED)
	{

	}

	enum 
	{
		SUCCESS,
		FAIL,
	};

	int32 nResult;

	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

// 服务器列表信息
struct SSRepServerInfoList : public NetMsgHead
{
	SSRepServerInfoList() :NetMsgHead(PRO_SS_REP_SERVERINFO_LIST)
	{
		nCount = 0;
	}
	int32 nCount;
	ServerInfoData sServerInfoData[MAX_SERVERINFO_LIST];
	inline int32 GetPackLength()const
	{
		return sizeof(*this) - sizeof(sServerInfoData) + nCount * sizeof(ServerInfoData);
	}
};

struct SSSessionNofitySInfo : public NetMsgHead
{
	SSSessionNofitySInfo() :NetMsgHead(PRO_SS_NOFTTY_SERVERID)
	{
		nFepServerID = 0;
		nLsServerID = 0;
		nDpServerID = 0;
		nSsServerID = 0;
	}
	int32 nFepServerID;
	int32 nLsServerID;
	int32 nDpServerID;
	int32 nSsServerID;
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

struct SSRepPingS : public NetMsgHead
{
	SSRepPingS():NetMsgHead(PRO_SS_REP_PING_S)
	{

	}
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

#pragma pack(pop)





#endif
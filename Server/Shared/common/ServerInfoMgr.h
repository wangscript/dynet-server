#ifndef __SERVER_INFO_MGR_H_
#define __SERVER_INFO_MGR_H_

#include "BaseDefine.h"
#include "BaseSingle.h"
#include "BaseSession.h"
#include "CommonDefine.h"

/*
 *
 *	Detail: 服务器信息管理(新增，转发，退出)
 *   
 * Copyright (c) Created by hzd 2015-5-14.All rights reserved
 *
 */
class ServerSession;

struct ServerInfo
{
	ServerInfo()
	{
		memset(this, 0, sizeof(*this));
	}
	char arrHosrt[32];
	int32 nPort;
	int32 nServerType;
	int32 nServerID;
	int32 nLoadCount;
	ServerSession* pServerSession;
};

typedef std::vector<ServerInfo> ServerInfoVecType;
typedef std::map<uint32, ServerInfo> ServerInfoMapType;


class ServerInfoMgr : public BaseSingle<ServerInfoMgr>
{
public:

	ServerInfoMgr();
	~ServerInfoMgr();

	// 获得某服务器信息
	const ServerInfo* GetServerInfo(uint32 nServerID);

	// 登录一新服务器
	bool OnServerLogin(ServerSession* pSession,const char arrHost[32],int32 nPort,int32 nServerID,uint32 nServerType);

	// 某服务器退出
	void OnServerExit(uint32 nServerID);

	// 负载+1
	void OnLoadAddOne(uint32 nServerID);

	// 负载-1
	void OnLoadSubOne(uint32 nServerID);

	// 获得服务器列表
	void GetServerInfoList(int32 nServerType,ServerInfoVecType& o_rServerList);

	// 获得负载最少的服务器
	ServerSession* GetLoadLestSession(int32 nServerType);

	// 广播服务器信息
	void BoardcastDpAndFepServerInfo2LsAndSs(uint32 nLoginServerID);

private:

	ServerInfoMapType m_mapServerInfo; // 服务器列表 

};



#endif





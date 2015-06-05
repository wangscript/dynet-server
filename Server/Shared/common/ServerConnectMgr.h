#ifndef __SERVER_CONNECTED_MGR_H_
#define __SERVER_CONNECTED_MGR_H_

#include "../DyNetSocket/NetClient.h"

#include "BaseDefine.h"
#include "BaseSingle.h"

struct NetClientInfo
{
	NetClientInfo()
	{
		nServerID	= 0;
		nServerType = 0;
		pNetClient	= NULL;
	}

	NetClient* pNetClient;
	uint32 nServerID;
	uint32 nServerType;

};

/*
 *
 *	Detail: 连接外部服务器连接管理
 *   
 *  Created by hzd 2015-5-25
 *
 */
class ServerConnectMgr : public BaseSingle<ServerConnectMgr>
{
	typedef vector<NetClientInfo*> NetClientInfoVecterType;
	typedef vector<uint32> DisconnectServerIDVectorType;
public:
	ServerConnectMgr();
	~ServerConnectMgr();

	typedef PNetServerEnterHandler NetMsgEnter;
	typedef PNetServerOnMsgHandler NetMsgOn;
	typedef PNetServerExistHandler NetMsgExit;

	// 连接到一个新的服务器 
	bool ConnectToServer(int32 nServerID,int32 nServerType, const char arrHost[32],int32 nPort,NetMsgEnter fEnter,NetMsgOn fMsg,NetMsgExit fExit);

	// 删除一个Server
	void RemoveServer(int32 nServerID);

	// 查找一个NetClient
	NetClientInfo* GetNetClient(NetSocket& rSocket);

	// 统计被连接数 
	int32 CountConnected();

	// update，处理NetClient中的socket信息 
	void Update(int32 nSrvTime);

private:

	NetClientInfoVecterType			m_vecNetClientInfo;	// 连接外部服务器信息
	DisconnectServerIDVectorType	m_vecNetClient;		// 断开连接的服务器ID

};


#endif



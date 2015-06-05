
#include "../DyNetSocket/NetServer.h"

#include "LogManager.h"
#include "ForServerMsgHandler.h"
#include "ServerSession.h"
#include "ClientSession.h"
#include "CommonServerOpt.h"

#include "DeallineAsync.h"

/*
*
*  Detail: ws程序主入口
*  (ls|fep|ss)->ws<-dp 
*  Created by hzd 2014-11-28
*
*/

NetServer*	gNetServerForClient;
DeallineAsync* gDeallien;

bool gIsRun = true;

void Init();
void ServerStart();
void RunServerExit();

int main(int argc, const char * argv[])
{
	// 初始化服务器所都要准备好的数据 
	Init();

	// 启动服务 
	ServerStart();

	// 执行退出的相关操作
	RunServerExit();

	return 0;

}

void InteravalUpdate(int32 nSrvTime)
{
	
	if(nSrvTime % 5 == 0) // 1秒 
	{
		ClientSessionMgr::Instance()->Update(nSrvTime);
	}else if(nSrvTime % 300 == 0) // 60秒
	{
		// ping 
	}
	
}

void ServerStart()
{
	FLOG_NORMAL("SERVER STARTED.");
	int32 nRunTimes = 0;
	while(gIsRun)
	{
		SSleep(1);
		nRunTimes++;
		gNetServerForClient->Update(0);
		if(nRunTimes > RUN_PRINT_TIMES)
		{
			int32 nServerNum = gNetServerForClient->ConnectedSockets();
			int32 nClientNum = ClientSessionMgr::Instance()->ConnectedSessions();
			FLOG_INFO("Server Info: Server Connected Num:%d ,Client Connected Num:%d", nServerNum, nClientNum);// 标识主线程还在做事中
			nRunTimes = 0;
		}
	}
}

void RunServerExit()
{

}

void Init()
{
	bool bResult = NetServerOpt::Instance()->LoadServerConfig("ws_cfg.xml",ESERVER_TYPE_WS);
	if(!bResult)
	{
		FLOG_INFO("Load cfg fail");
		return;
	}

	// 初始化Scoket服务
	ServerInfoOpt& rLocalInfo = NetServerOpt::Instance()->GetLocalServerInfo();
	gNetServerForClient = new NetServer(MAX_WS_SOCKET_CONNECT);
	gNetServerForClient->SetTimeout(60);
	gNetServerForClient->SetAddress(rLocalInfo.strHost.c_str(), rLocalInfo.nPort);
	gNetServerForClient->SetHandler(boost::bind(&ForServerMsgHandler::OnNetMsgEnter, ForServerMsgHandler::Instance(), _1),
		boost::bind(&ForServerMsgHandler::OnNetMsg, ForServerMsgHandler::Instance(), _1, _2, _3), 
		boost::bind(&ForServerMsgHandler::OnNetMsgExit, ForServerMsgHandler::Instance(), _1));

	// 启动Socket服务
	gNetServerForClient->Start();

	// 启动定时器
	//gDeallien = new DeallineAsync(3000, 0, InteravalUpdate);
	//gDeallien->Start();

}





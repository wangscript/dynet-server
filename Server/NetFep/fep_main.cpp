#include "ClientSession.h"

#include "../DyNetSocket/NetServer.h"
#include "../DyNetSocket/NetClient.h"

#include "ServerSession.h"
#include "ForClientMsgHandler.h"
#include "ForWorldMsgHandler.h"
#include "ForServerMsgHandler.h"
#include "LogManager.h"
#include "CommonServerOpt.h"



/*
*
*	Detail: 程序主入口
*   
*  Created by hzd 2014-11-28
*
*/

NetClient*	gNetClientForWs;
NetServer*	gNetServerForServer;//for ls,ss and so on
NetServer*	gNetServerForClient;

bool gIsRun		= true;

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

void ServerStart()
{
	int32 nRunTimes = 0;
	while(true)
	{
		SSleep(1);
		gNetClientForWs->Update(0);
		gNetServerForServer->Update(0);
		gNetServerForClient->Update(0);
		nRunTimes++;
		if(nRunTimes > RUN_PRINT_TIMES)
		{
			int32 nServerNum = ServerSessionMgr::Instance()->CountConnected();
			int32 nClientNum = gNetServerForClient->ConnectedSockets();
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
	
	bool bResult = NetServerOpt::Instance()->LoadServerConfig("fep_cfg.xml",ESERVER_TYPE_FEP);
	if(!bResult)
	{
		FLOG_INFO("Load cfg fail");
		return;
	}

	// 连接WS服务
	ServerInfoOpt& rRemoteInfo = NetServerOpt::Instance()->GetRemoteServerInfo();
	gNetClientForWs = new NetClient();
	gNetClientForWs->SetAddress(rRemoteInfo.strHost.c_str(),rRemoteInfo.nPort);
	gNetClientForWs->SetHandler(boost::bind(&ForWorldMsgHandler::OnNetMsgEnter,ForWorldMsgHandler::Instance(),_1),
		boost::bind(&ForWorldMsgHandler::OnNetMsg,ForWorldMsgHandler::Instance(),_1,_2,_3),
		boost::bind(&ForWorldMsgHandler::OnNetMsgExit,ForWorldMsgHandler::Instance(),_1));
	gNetClientForWs->Start();

	// 初始化ForServer服务
	ServerInfoOpt& rLocalInfo = NetServerOpt::Instance()->GetLocalServerInfo();
	gNetServerForServer = new NetServer(32);
	gNetServerForServer->SetTimeout(60);
	gNetServerForServer->SetAddress(rLocalInfo.strHost.c_str(), rLocalInfo.nPort);
	gNetServerForServer->SetHandler(boost::bind(&ForServerMsgHandler::OnNetMsgEnter, ForServerMsgHandler::Instance(), _1),
		boost::bind(&ForServerMsgHandler::OnNetMsg, ForServerMsgHandler::Instance(), _1, _2,_3),
		boost::bind(&ForServerMsgHandler::OnNetMsgExit, ForServerMsgHandler::Instance(), _1));
	gNetServerForServer->Start();

	// 初始化ForClient服务
	ServerInfoOpt& rClientInfo = NetServerOpt::Instance()->GetClientServerInfo();
	gNetServerForClient = new NetServer(MAX_FEP_SOCKET_CONNECT);
	gNetServerForClient->SetAddress(rClientInfo.strHost.c_str(),rClientInfo.nPort);
	gNetServerForClient->SetHandler(boost::bind(&ForClientMsgHandler::OnNetMsgEnter,ForClientMsgHandler::Instance(),_1),
		boost::bind(&ForClientMsgHandler::OnNetMsg,ForClientMsgHandler::Instance(),_1,_2,_3),
		boost::bind(&ForClientMsgHandler::OnNetMsgExit,ForClientMsgHandler::Instance(),_1));

	// 启动Socket服务
	gNetServerForClient->Start();

}





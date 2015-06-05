

#include "../DyNetSocket/NetServer.h"
#include "../DyNetSocket/NetClient.h"

#include "ForWorldMsgHandler.h"
#include "ForServerMsgHandler.h"
#include "ServerSession.h"
#include "CommonServerOpt.h"
#include "ClientSession.h"
#include "ServerConnectMgr.h"
#include "LogManager.h"


/*
*
*	Detail: 程序主入口
*
*  Created by hzd 2014-11-28
*
*/

NetClient*	gNetClientForWs; // connected to ws

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

void ServerStart()
{
	int32 nRunTimes = 0;
	while(gIsRun)
	{
		SSleep(1);
		gNetClientForWs->Update(0.0);
		ServerConnectMgr::Instance()->Update(0.0); // NetClient.update
		nRunTimes++;
		if(nRunTimes > RUN_PRINT_TIMES)
		{
			int32 nServerNum = ServerSessionMgr::Instance()->CountConnected();
			int32 nClientNum = ClientSessionMgr::Instance()->ConnectedSessions();
			FLOG_INFO("Server Info: Server Connected Num:%d ,Client Connected Num:%d", nServerNum, nClientNum);
			nRunTimes = 0;
		}
	}
}

void RunServerExit()
{

}

void Init()
{
	bool bResult = NetServerOpt::Instance()->LoadServerConfig("ls_cfg.xml",ESERVER_TYPE_LS);
	if(!bResult)
	{
		FLOG_INFO("Load cfg fail");
		return;
	}

	// 连接WS服务
	ServerInfoOpt& rRemoteInfo = NetServerOpt::Instance()->GetRemoteServerInfo();
	gNetClientForWs = new NetClient();
	gNetClientForWs->SetAddress(rRemoteInfo.strHost.c_str(), rRemoteInfo.nPort);
	gNetClientForWs->SetHandler(boost::bind(&ForWorldMsgHandler::OnNetMsgEnter, ForWorldMsgHandler::Instance(), _1),
		boost::bind(&ForWorldMsgHandler::OnNetMsg, ForWorldMsgHandler::Instance(), _1, _2, _3),
		boost::bind(&ForWorldMsgHandler::OnNetMsgExit, ForWorldMsgHandler::Instance(), _1));

	gNetClientForWs->Start();

}


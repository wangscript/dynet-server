#include "Client_PCH.h"
// clientCtl.cpp : 定义控制台应用程序的入口点。
//

#include "ClientCommand.h"
#include "NetMsgHandler.h"
#include "ClientPlayer.h"

GameCommand gGameCommond;

bool bExist = false;

void ClientInit();
void ClientRun();
void GameUpdate(float delay);
void RunClientExit();

int main(int argc, const char * argv[])
{
	ClientInit();
	ClientRun();
	RunClientExit();
	return 0;
}


void ClientInit()
{
	 InitNetMsg();

	 for (int32 i = 0; i < MAX_CLIENT_COUNT; ++i)
	 {
		 NetClient* gNetClient = new NetClient();
		 gNetClient->SetAddress("127.0.0.1", 9900);
		 gNetClient->SetHandler(OnNetMsgEnter, OnNetMsg, OnNetMsgExit);
		 gNetClient->Start();

		 ClientPlayer* gPlayer = new ClientPlayer();
		 gPlayer->SetSocket(gNetClient->GetSocket());

         ClientPlayerMgr::Instance()->AddSocket(gNetClient,gPlayer);
	 }

	//命令调试
	//gGameCommond.Init(gNetClient->GetSocket());
	//GameCommand::CommandHelp();

}

void	ClientRun()
{
	timer t;
	float32 now = 0;
	float32 last = 0;

	float delay = 0.0;
	while (true)
	{
		if(bExist)
			break;
		SSleep(10);
		now		= (float32)t.elapsed();
		delay	= now - last;
		gGameCommond.Command(delay);
		GameUpdate(delay);
		ClientPlayerMgr::Instance()->Update();
		last = now;
	}

}

void GameUpdate(float delay)
{

}

void RunClientExit()
{
	// 保存 & 释放资源

}
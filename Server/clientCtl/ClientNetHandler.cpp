#include "Client_PCH.h"
#include "ClientNetHandler.h"
#include "NetMsgHandler.h"
#include "ClientPlayer.h"
#include "CommonC2F.h"
#include "CommonF2C.h"
#include "CommonC2W.h"

//extern	ClientPlayer* gPlayer;


ClientNetHandler::ClientNetHandler(void)
{
}


ClientNetHandler::~ClientNetHandler(void)
{
}

void ClientNetHandler::SendRequestClose(ClientPlayer* gPlayer)
{

}

void ClientNetHandler::SendEncrypt(ClientPlayer* gPlayer)
{
	C2FepEncryptInfo sMsg;
	gPlayer->SendMsg(&sMsg,sMsg.GetPackLength());
}

void ClientNetHandler::RecvEncryptInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{
	const F2CEncryptInfo& rPacket = *static_cast<const F2CEncryptInfo*>(pHead);
	gPlayer->SetEncryptKey(rPacket.arrEncryptKey);
	SendLogin(gPlayer);
}

void ClientNetHandler::RecvLoginReadyFinish(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{
	C2WSwitchScene sMsg;
	sMsg.nSceneID = 101;
	gPlayer->SendMsg(&sMsg,sMsg.GetPackLength());
}

void ClientNetHandler::SendLogin(ClientPlayer* gPlayer)
{
	// 使用默认账号与密码

}

void ClientNetHandler::SendCharacterList(ClientPlayer* gPlayer)
{

}

void ClientNetHandler::SendChar2World(ClientPlayer* gPlayer)
{

}
void ClientNetHandler::SendRegister(ClientPlayer* gPlayer)
{

}

void ClientNetHandler::SendLoadResed(ClientPlayer* gPlayer)
{

}


////////////////////////////接收//////////////////////////////////////////

void ClientNetHandler::RevcCharacterList(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{

}




void ClientNetHandler::RevcLoginResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{

}

void ClientNetHandler::RecvSelectResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{
	
}

void ClientNetHandler::RecvRegisterResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{

}

void ClientNetHandler::RecvCharacterInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{
	
}

void ClientNetHandler::RecvChatInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize)
{


}


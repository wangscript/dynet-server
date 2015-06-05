#include "LoginServerProcHandler.h"

#include "BaseSession.h"
#include "../DyNetSocket/NetConfig.h"
#include "LogManager.h"
#include "ServerSession.h"
#include "CommonL2W.h"
#include "CommonW2L.h"
#include "ServerInfoMgr.h"


LoginServerProcHandler::LoginServerProcHandler()
{
}


LoginServerProcHandler::~LoginServerProcHandler()
{
}


void LoginServerProcHandler::ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize)
{

	//// ls请求登录
	//const Ls2WsLogin* pPacket = static_cast<const Ls2WsLogin*>(pMsg);
	//const LoginData2Ws& sLoginData = pPacket->sLoginData;
	//if(!CheckLogin(*pSession, sLoginData, ESERVER_TYPE_LS))
	//{
	//	FLOG_ERROR("CheckLogin Fail!");
	//	return;
	//}

	//ServerSession* pClient = static_cast<ServerSession*>(pSession);

	//// 状态
	//pClient->SetStatus(EPLAYER_STATUS_LOGINED);
	//pClient->SetServerType(ESERVER_TYPE_LS);
	//pClient->SetServerID(sLoginData.nServerID);

	//// 返回登录成功
	//W2LsLogined sMsg;
	//sMsg.nResult = W2LsLogined::SUCCESS;
	//pClient->SendMsg(&sMsg, sMsg.GetPackLength());

	//// 记录新登入进来的server
	//if(!ServerInfoMgr::Instance()->OnServerLogin(pSession,sLoginData.arrServerAddr,sLoginData.nServerPort,sLoginData.nServerID,ESERVER_TYPE_LS))
	//{
	//	FLOG_ERROR("Login fail:from ls");
	//	return;
	//}

	//// 广播新的server到其它server,
	//ServerInfoMgr::Instance()->BoardcastDpAndFepServerInfo2LsAndSs(pPacket->sLoginData.nServerID);



}
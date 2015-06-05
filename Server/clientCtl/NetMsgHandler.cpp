#include "Client_PCH.h"
#include "NetMsgHandler.h"
#include "ClientNetHandler.h"
#include "CommonF2C.h"

static NetMsgHandlerMap	 gMsgHeandlerMap;
extern bool bExist;

bool	CheckMsg(const NetMsgHead* pMsg , uint32 nSize )
{
	return true;
}


void	InitNetMsg()
{

	// 收到的消息处理
    static NetMsgHandler handler[] =
    {
		// 协议=>处理函数
		// 登录类协议
		{PRO_F2C_ENCRYPT_INFO, ClientNetHandler::RecvEncryptInfo},
		{PRO_F2C_LOGIN_READY_FINISH,ClientNetHandler::RecvLoginReadyFinish},

		// end
	

    };
    
    int32 i = 0;
    while ( handler[i].nType )
    {
        gMsgHeandlerMap[ handler[ i ].nType ] = handler[ i ];
        i++;
    }
}


void OnNetMsgEnter( NetSocket& rSocket )
{
	//
	ClientPlayer* pClientPlayer = ClientPlayerMgr::Instance()->GetClientPlayer(rSocket);
	ClientNetHandler::SendEncrypt(pClientPlayer);
}


void OnNetMsg( NetSocket& rSocket,NetMsgHead* pMsg ,int32 nSize )
{

	ClientPlayer* gPlayer = ClientPlayerMgr::Instance()->GetClientPlayer(rSocket);

	// 解密处理
	static char msgBuffer[65536];
	memcpy(&msgBuffer[0], pMsg,nSize);

	Encrypt::XorCode(nSize,gPlayer->GetEncryptKey(),msgBuffer,nSize);
	pMsg = ( NetMsgHead*)&msgBuffer[0];

	NetMsgHandlerMapIter iter = gMsgHeandlerMap.find( pMsg->nType );
	if ( iter == gMsgHeandlerMap.end() )
	{
		FLOG_INFO("Not find Pro:%d", pMsg->nType);
		ASSERT(0);
	}
	else
	{
		ClientPlayer* pClientPlayer = ClientPlayerMgr::Instance()->GetClientPlayer(rSocket);
		(iter->second.pFun)(pClientPlayer,pMsg ,nSize);
		FLOG_INFO("OnNetMsg %d", pMsg->nType);
	}
}



void OnNetMsgExit( NetSocket& socket )
{
	FLOG_INFO("=========Had Exist===============","");
	bExist = true;
}



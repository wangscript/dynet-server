
#include "ProcWorldHandler.h"
#include "CommonW2F.h"
#include "ServerSession.h"

#include "LogManager.h"


ProcWorldHandler::ProcWorldHandler()
{
}


ProcWorldHandler::~ProcWorldHandler()
{
}

void ProcWorldHandler::ReqLogin(BaseSession* pBaseSession, const NetMsgHead* pMsg,int32 nSize)
{

	if(pBaseSession->Status() != ESERVER_STATUS_CONNETED)
	{
		FLOG_INFO("pBaseSession->Status() != EWORLD_STATUS_CONNECTED");
		return;
	}

	// 判断
	pBaseSession->SetStatus(ESERVER_STATUS_LOGINED);

	// ok finish!
	// 等待其它 ls,ss的连接 

}

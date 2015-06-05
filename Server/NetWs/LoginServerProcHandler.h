#ifndef _LOGIN_SERVER_PROC_HANDLER_H_
#define _LOGIN_SERVER_PROC_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseProcHandler.h"

class BaseSession;
struct NetMsgHead;

class LoginServerProcHandler : public BaseProcHandler, public BaseSingle<LoginServerProcHandler>
{
public:
	LoginServerProcHandler();
	~LoginServerProcHandler();

	// ls µÇÂ¼
	void ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

};




#endif


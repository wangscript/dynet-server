#ifndef __WS_CLIENT_PROC_HANDLER_H_
#define __WS_CLIENT_PROC_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseProcHandler.h"

class BaseSession;
struct NetMsgHead;

class ClientProcHandler : public BaseProcHandler, public BaseSingle<ClientProcHandler>
{
public:
	ClientProcHandler(void);
	~ClientProcHandler(void);

	void ReqSwitchScene(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

};


#endif





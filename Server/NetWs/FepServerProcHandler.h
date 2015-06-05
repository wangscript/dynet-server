#ifndef _FEP_SERVER_PROC_HANDLER_H_
#define _FEP_SERVER_PROC_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseProcHandler.h"

class BaseSession;
struct NetMsgHead;

class FepServerProcHandler : public BaseProcHandler, public BaseSingle<FepServerProcHandler>
{
public:
	FepServerProcHandler();
	~FepServerProcHandler();

	// fep µÇÂ¼
	void ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);

};



#endif


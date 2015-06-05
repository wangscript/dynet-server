#ifndef _DP_SERVER_PROC_HANDLER_H_
#define _DP_SERVER_PROC_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"
#include "BaseProcHandler.h"

class DpServerProcHandler : public BaseProcHandler, public BaseSingle<DpServerProcHandler>
{
public:
	DpServerProcHandler();
	~DpServerProcHandler();

	// dp µÇÂ¼
	void ReqLogin(BaseSession* pSession, const NetMsgHead* pMsg,int32 nSize);
};

#endif




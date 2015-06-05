#ifndef __BASE_PROC_HANDLER_H_
#define __BASE_PROC_HANDLER_H_


/*
 *
 *	Detail: 逻辑处理父类
 *   
 * Copyright (c) Created by hzd 2015-5-8.All rights reserved
 *
 */

#include "BaseDefine.h"
#include "BaseSingle.h"
#include "CommonDefine.h"
#include "BaseSession.h"
#include "../DyNetSocket/NetConfig.h"
#include "LogManager.h"
#include "ClientSession.h"

class BaseProcHandler
{
public:

	bool CheckLogin(BaseSession& pBaseSession, const LoginData2Ws& pLoginData,EServerType eServerType) 
	{
		// 请求登录
		if(pBaseSession.Status() != ECLIENT_STATUS_CONNECTED)
		{
			FLOG_INFO("pBaseSession->Status() != EPLAYER_STATUS_CONNECTED");
			return false;
		}

		// 这里需要做检验工作，防止外挂的入侵

		return true;
	}
};











#endif


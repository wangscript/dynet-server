#ifndef _DB_CALL_REQUEST_H_
#define _DB_CALL_REQUEST_H_

#include "DbConfig.h"

/*
 *
 * Detail: 请求结构类
 *   
 * Copyright (c) Created by hzd 2015-4-22.All rights reserved
 *
 */

class DLLAPI DbRequest
{
public:

	DbRequest();
	~DbRequest();

public:

	char	m_arrSql[DB_MAX_SQL];
	void*	m_pData;
	SQLResultHandler	m_pHandler;
	EResultHandlerType	m_eResultHandleType;

};

#endif



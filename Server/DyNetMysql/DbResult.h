#ifndef _DB_CALL_RESULT_H_
#define _DB_CALL_RESULT_H_

#include "DbConfig.h"

/*
 *
 *	Detail: 结果数据类
 *   
 *  Created by hzd 2015-4-22
 *
 */

class DLLAPI DbResult
{
public:

	DbResult();
	~DbResult();

	DbResult& operator=(DbResult* pResult);

	void Release();
	void DBCallRead(DbResult* pResult);

public:
	
	void*	m_pData;
	bool   	m_bFlag;
	SQLResultHandler	m_pCallResult;
	DbIRecordSet*		m_setRecordSet;
	EResultHandlerType	m_eHandlerType;

};


#endif





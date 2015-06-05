#include "DbResult.h"


DbResult::DbResult()
{
}


DbResult::~DbResult()
{
}

void DbResult::Release()
{
	if(m_setRecordSet)
	{
		m_setRecordSet->Release();
	}
	m_setRecordSet = NULL;
	delete this;
}

DbResult& DbResult::operator=(DbResult* pResult)
{
	if(this == pResult)
		return *this;

	this->m_setRecordSet = pResult->m_setRecordSet;
	this->m_pCallResult = pResult->m_pCallResult;
	this->m_bFlag = pResult->m_bFlag;
	this->m_setRecordSet = pResult->m_setRecordSet;
	this->m_eHandlerType = pResult->m_eHandlerType;
	return *this;
}

void DbResult::DBCallRead(DbResult* pResult)
{
	if(!pResult)
		return;

	switch(pResult->m_eHandlerType)
	{
		case ECALLBACK_DB_NULL: // ÎÞÐë»Øµ÷
			break;

		case ECALLBACK_DB_HANDLER:
		{
			DbIRecordSet* pRes = pResult->m_setRecordSet;
			if(!pRes)
				 return;
			(*m_pCallResult)(pResult->m_pData, pRes);
		}
			break;
	}
}

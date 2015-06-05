#ifndef SWA_DB_RECORD_H_
#define SWA_DB_RECORD_H_

#include "DbConfig.h"
#include "DbRecordSet.h"


class DLLAPI DbField;
class DLLAPI DbRecord : public DbIRecord
{
	friend class DbRecordSet;
	friend class DbField;
public:

	virtual	void Release();

	virtual DbIData& Field(uint32 unIndex);
	virtual DbIData& Field(uint32 unIndex) const;
	virtual DbIData& Field(const char* pszName);
	virtual DbIData& Field(const char* pszName) const;

	virtual	ulong GetFieldCount();

	virtual DbIData& Key();

	virtual bool Update(bool bSync = true);
	virtual bool Delete(bool bArchive = false);
	virtual bool Insert();
	virtual void ClsEditFlag();

	const char*	KeyName();
	bool BuildSQLOperation(char* pszOperationSQL);
	
	virtual void* GetRowBuff();

	vector<DbField>& GetMFields();
	void SetBuff(const char* pszValue, uint32 nLen, int32 nType);

private:

	DbRecord(const DbRecord& rRecord);
	DbRecord(DbRecordSet& rRecordSet, uint32 nFieldNum);
	DbRecord(DbRecordSet& rRecordSet, MYSQL_ROW row, uint32& nFieldNum);
	DbRecord(DbRecordSet& rRecordSet, MYSQL_FIELD* fields, uint32& nFieldNum);
	virtual ~DbRecord();

	DbRecord& operator= (const DbRecord& rRecord);
	void BuildSQLCondition(char* pszConditionSQL);

private:

	DbRecordSet&	m_rRecordSet;
	vector<DbField>	m_vecFields;
	char			m_arrRowBuff[65536]; // 每一行内容记录，每个字段记录它的开始位置即可，则一行记录最大为64k 
	uint32			m_nBuffBegin; // 已经使用的位置

};


#endif


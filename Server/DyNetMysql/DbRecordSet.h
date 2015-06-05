#ifndef SWA_DB_RECORDSET_H_
#define SWA_DB_RECORDSET_H_

#include "DbConfig.h"
#include "DbField.h"
#include "DbSession.h"

class DLLAPI DbRecord;
class DLLAPI DbRecordSet : public DbIRecordSet
{
	friend class DbSession;
	friend class DbRecord;
public:

	bool Create( MYSQL_RES* pRES );
	virtual void Release();
	virtual DbIRecord* GetRecord() const;
	virtual void* GetRecordBuff(uint32 nIndex) const;
	virtual ulong CountRecord() const;

	virtual void Move( uint32 nIndex );
	virtual bool Update( bool bSync = true );
	virtual void ClsEditFlag();
	virtual DbIRecord* MakeDefRecord();

	vector<DbFieldInfo>& GetFieldInfos();

	int32 FieldsCount() const;

protected:

	DbRecordSet(DbSession& rDataBase, EDbMode eMode);
	virtual ~DbRecordSet();
	bool Create( const char* pszSQL );
	void Remove( DbRecord* pRec );
	DbRecord* MakeDefaultRecord( const char* pszTable );

	char* GetTableName();

	const DbFieldInfo&	GetFieldInfo( int32 nIndex ) const;

	void BuildSQLCondition( char* pszConditionSQL );
	void BuildSQLOperation( char* pszOperationSQL );
			 
	bool UpdateRecord( DbRecord* pRec , bool bSync = true );
	bool DeleteRecord( DbRecord* pRec , bool bArchive = false );
	bool InsertRecord( DbRecord* pRec );

private:                                                                                                                                                                                                                                                                                                                                                                           

	DbRecordSet( const DbRecordSet& res );
	DbRecordSet& operator= ( const DbRecordSet& res );

private:

	DbSession&	m_rDataBase;
	EDbMode		m_eMode;
	int32		m_nCursor;
	char		m_arrSQL[DB_MAX_SQL];
	char		m_arrTableName[32];
	uint32		m_nKeyIndex;
	uint32		m_nAutoIncIndex;

	vector<DbRecord*>		m_vecRecord;
	vector<DbFieldInfo>		m_vecFieldInfo;
	

};





#endif




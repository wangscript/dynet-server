#ifndef SWA_DB_CONFIG_H_
#define SWA_DB_CONFIG_H_


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/container/set.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <memory.h>
#include <mysql.h>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

using namespace std;

#ifdef WIN32
#define SSleep(ms) Sleep(ms)
#else
#define SSleep(ms) usleep(ms)
#endif

#ifdef DLL_FILE
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#define S_SAFE_DELETE(a) if( a ){ delete a;a=NULL; }
#define S_SAFE_RELEASE(a) if( a ){ a->Release(); }

#ifdef WIN32
typedef __int64                     int64;
#else
typedef int64_t                     int64;
#endif
typedef unsigned long long          uint64;
typedef wchar_t						wchar;
typedef unsigned char				uchar;
typedef char						int8;
typedef unsigned char				uint8;
typedef short						int16;
typedef unsigned short				uint16;
typedef int							int32;
typedef unsigned int				uint32;
typedef unsigned int				ulong;
typedef float						float32;
typedef double						float64;

enum EDbMode
{
	E_DB_MODE_NONE = 0,
	E_DB_MODE_READ,
	E_DB_MODE_EDIT
};

const int32	DB_MAX_SQL = 5000;

typedef void(*SQLResultHandler)(void*, const void*);

// 回调类型
enum EResultHandlerType
{
	ECALLBACK_NULL = 0,	// 非法参数
	ECALLBACK_DB_NULL = 1,	// 执行数据后无需回调
	ECALLBACK_DB_HANDLER = 2,	// 数据结果回调（查询完后调回）
};

class DbIData
{
public:
		
	virtual	void Release() = 0;
	virtual char* GetValue() = 0;
	virtual ulong GetLen() const = 0;
	virtual ulong GetType() const = 0;

	virtual operator char() const = 0;
	virtual operator uchar() const = 0;
	virtual operator int16() const = 0;
	virtual operator uint16() const = 0;
	virtual operator int32() const = 0;
	virtual operator uint32() const = 0;
	virtual operator int64() const = 0;
	virtual operator uint64() const = 0;
	virtual operator float32() const = 0;
	virtual operator float64() const = 0;
	virtual operator char*() const = 0;
	virtual operator const char*() const = 0;
		
	virtual DbIData& operator = ( char chOp ) = 0;
	virtual DbIData& operator = ( uchar chOp ) = 0;
	virtual DbIData& operator = ( int16 nOp ) = 0;
	virtual DbIData& operator = ( uint16 unOp ) = 0;
	virtual DbIData& operator = ( int32 iOp ) = 0;
	virtual DbIData& operator = ( uint32 uOp ) = 0;
	virtual DbIData& operator = ( int64 uOp ) = 0;
	virtual DbIData& operator = ( uint64 uOp ) = 0;
	virtual DbIData& operator = ( float32 fltOp ) = 0;
	virtual DbIData& operator = ( float64 dblOp ) = 0;
	virtual DbIData& operator = ( const char* szVal ) = 0;
};
	
class DbIRecord
{
public:
		
	virtual	void Release() = 0;
	virtual DbIData& Field( uint32 idx ) = 0;
	virtual DbIData& Field( uint32 idx ) const = 0;
	virtual DbIData& Field( const char* pszName ) = 0;
	virtual DbIData& Field( const char* pszName ) const = 0;
	virtual ulong GetFieldCount() = 0;
	virtual DbIData& Key() = 0;
	virtual bool Update( bool bSync = true ) = 0;
	virtual bool Delete( bool bArchive = false ) = 0;
	virtual bool Insert() = 0;
	virtual void ClsEditFlag() = 0;
	virtual	void* GetRowBuff() = 0;
};
	
class DbIRecordSet
{
public:
		
	virtual bool Create( MYSQL_RES* pRES ) = 0;
	virtual	void Release() = 0;
	virtual DbIRecord* GetRecord() const = 0;
	virtual void* GetRecordBuff(uint32 nIndex) const = 0;
	virtual ulong CountRecord() const = 0;
	virtual void Move( uint32 index ) = 0;
	virtual bool Update( bool bFlag = true ) = 0;
	virtual void ClsEditFlag	() = 0;
	virtual DbIRecord* MakeDefRecord() = 0;

};

class DbResult;
		
class IDbSession
{
public:

	virtual uint32 ID() = 0;
	virtual	void Release() = 0;
	virtual uint32 GetRequestSize() = 0;
	virtual uint32 GetResultSize() = 0;
	virtual bool ExecuteAsyncSQL( const char* pszSQL , void* pUser , SQLResultHandler pCallback ) =0 ;
	virtual DbResult* GetAsyncResult()= 0;
	virtual MYSQL_RES* ExecuteRsyncSQL( const char* pszSQL ) = 0;
			
};
extern "C" IDbSession*	DatabaseCreate( const char* szDBServer , const char* szLoginName , const char* szPassword , const char* szDBName , bool bEnableSQLChk = true ); 
		
#endif
	
	

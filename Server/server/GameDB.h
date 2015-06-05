#ifndef _GAMEDB_H_
#define _GAMEDB_H_

/*
 *
 *	Detail: 大部分数据，直接与mysql打交道，player里的数据，只是缓存，不用每次从mysql中读取，或player中只是缓存不太重要的数据
 *  断开连接也会保存部分需要的数据
 *   
 *  Created by hzd 2014-10-7
 *
 */

const int32 MAX_SQL_BUFFER = 512;
extern char SQL_BUFFER[ MAX_SQL_BUFFER ];

#pragma pack(push,1)

// 平台账号登录结果 
struct DBAccountLoginResult
{
	uint32 nID;
	char  arrUsername[32];
};

// 角色创建结果 
struct DBCreateRoleResult
{
	uint64 nResult; // 1-3失败，> 3 成功，为角色ID
};

struct DBSelectedResult
{
	int32 nRes;//0失败，后继数据无效,1成功，后继数据有效
	int32 nCharAddr; // 角色信息的首地址
};

struct DBCharacherData
{
	uint64 nUID;
	char arrNick[32];
	int32 nType;
	int32 nLevel;
	int32 nLandId;
	int32 nLandX;
	int32 nLandY;
};

#pragma pack(pop)


#endif


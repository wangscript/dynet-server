#ifndef __DEF_CHARACTER_H_
#define __DEF_CHARACTER_H_

#include "CommonDefine.h"

enum CharacterType
{
	CHARACTER_TYPE_NULL		= 0,    // 非法
	CHARACTER_TYPE_SHAOLIN	= 1,	// 少林
	CHARACTER_TYPE_MOJIAO	= 2,	// 魔教
	CHARACTER_TYPE_GUMU		= 3,	// 古墓
	CHARACTER_TYPE_XIAOYAO	= 4,	// 逍遥
};

enum CharacterConst
{
	MAX_CHARACTER_NAME_LENGTH		= 32,			// 角色名最大长度
	MAX_CHARACTER_NUM				= 8,			// 最大角色数量
};

#pragma pack(push, 1)
//
struct CharacterOperateInfo
{
	uint64			nCharacterID;						// 角色ID
	int8			arrName[MAX_CHARACTER_NAME_LENGTH];	// 角色名
	bool			bSelected;							// 被选中
	CharacterType	eCharacterType;						// 角色类型[职业]
	int32			nLevel;								// 等级

	// 装备等～
};

//for ws
struct CharacterData
{
	uint32 nLandMapId;			//世界地图ID
	uint32 nInstanceMapId;		//副本地图ID
};

//for ss
struct CharacterDataForSS
{
	int8				arrName[MAX_CHARACTER_NAME_LENGTH];
	CharacterType		nRoleType;
	uint32				nLevel;
	uint32				nInViewDistance;
	uint32				nOutViewDistance;
	uint32				nLandMapId;
	int32				nLandMapX;
	int32				nLandMapZ;
	uint32				nInstanceMapId;
	int32				nInstanceMapX;
	int32				nInstanceMapZ;
};

//角色身上穿的三件装备
struct CharacterWearData
{
	uint64 nCharacterID; //角色ID
	uint32 nClothesID;	//衣服ID
	uint32 nWeaponID;   //武器ID
};

#pragma pack(pop)

#endif
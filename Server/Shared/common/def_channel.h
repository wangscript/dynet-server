#ifndef __DEF_CHANNEL_H_
#define __DEF_CHANNEL_H_

#include <vector>

#include "Geometry2D.h"
#include "def_entity.h"


enum ChannelRegistType
{
	CHANNEL_REGIST_TYPE_NULL				= 0x0000000,	// 无
	CHANNEL_REGIST_TYPE_POSITION_BROADCAST	= 0x00000001,	// 位置信息广播
	CHANNEL_REGIST_TYPE_POSITION_COLLECT	= 0x00000002,	// 位置信息收集
	CHANNEL_REGIST_TYPE_CREATURE_BROADCAST	= 0x00000004,	// 生物体信息广播
	CHANNEL_REGIST_TYPE_CREATURE_COLLECT	= 0x00000008,	// 生物体信息收集
	CHANNEL_REGIST_TYPE_EQUIPMENT_BROADCAST = 0x00000010,   // 装备广播
	CHANNEL_REGIST_TYPE_EQUIPMENT_COLLECT	= 0x00000020,   // 装备收集
	CHANNEL_REGIST_TYPE_SPELL_BROADCAST		= 0x00000040,	// 技能广播
	CHANNEL_REGIST_TYPE_SPELL_COLLECT		= 0x00000080,   // 技能收集
	CHANNEL_REGIST_TYPE_BUFF_BROADCAST		= 0x00000100,	// Buff信息广播	
	CHANNEL_REGIST_TYPE_BUFF_COLLECT		= 0x00000200,	// Buff信息收集
	CHANNEL_REGIST_TYPE_MAX
};

enum 
{
	MAX_CHANNEL_BUFF		= 15,	// 最大Buff数量	
	MAX_CHARACTER_NAME_LENGTH_C = 32 ,
};

enum
{
	MAX_POSITION_COUNT = 100, // 最多可存放周边玩家位置数据
	MAX_CREATURE_COUNT = 100, // 最多可存放周边玩家生物数据
};

#pragma pack(push,1)


// 位置信息
struct ChannelPosition
{
	uint64			nID;			//实体ID
	EntityType		nType;			//实体类型
	uint32			nTypeID;		//实体类型子ID
	int32			nPositionX;		//位置x
	int32			nPositionZ;		//位置z
	int32			nHeadingX;		//朝向X已经扩大了100
	int32			nHeadingZ;		//朝向X已经扩大了100
};

// 生物体信息
struct ChannelCreature
{
	uint64		nID;									// 实体ID
	int8		arrName[MAX_CHARACTER_NAME_LENGTH_C];	//昵称
	int32		nRed;									// 红(血)
	int32		nRedMax;								// 红上限
	int32		nBlue;									// 蓝(法术)
	int32		nBlueMax;								// 蓝上限
};

// 装备信息
struct ChannelWear
{
	uint64		nID;				//实体ID
	uint32		nClothesID;			//衣服ID
	uint32		nWeaponID;			//武器装备ID
};

// 技能信息
struct ChannelSpell
{
	uint64		nID;			// 实体ID
	uint32      nSpellID;		// 技能ID
};

// Buff信息
struct ChannelBuff
{
	uint32		nNum;							// Buff数量
	uint64		nID;							// 实体ID
	uint32		arrBuffID[MAX_CHANNEL_BUFF];    // BuffID 
};

#pragma pack(pop)

#define FUNCTION_CHANNEL_BROADCAST(InfoType)\
virtual void ChannelBroadcast(const std::set<const InfoType*>& rsetValues) {}
#define DEFINE_FUNCTION_CHANNEL_BROADCAST(InfoType)\
virtual void ChannelBroadcast(const std::set<const InfoType*>& rsetValues);
#define IMPLEMENT_FUNCTION_CHANNEL_BROADCAST(ClassName, InfoType)\
void ClassName::ChannelBroadcast(const std::set<const InfoType*>& rsetValues)

#define FUNCTION_CHANNEL_COLLECT(InfoType)\
typedef const InfoType* P##InfoType;\
virtual void ChannelCollect(P##InfoType& o_rpInfo) { \
o_rpInfo = &m_s##InfoType; }
#define DEFINE_FUNCTION_CHANNEL_COLLECT(InfoType)\
virtual void ChannelCollect(P##InfoType& o_rpInfo);
#define IMPLEMENT_FUNCTION_CHANNEL_COLLECT(ClassName, InfoType)\
void ClassName::ChannelCollect(P##InfoType& o_rpInfo)


#endif
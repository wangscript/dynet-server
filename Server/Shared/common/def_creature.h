#ifndef __DEF_CREATURE_H_
#define __DEF_CREATURE_H_

#include "CommonDefine.h"

#define MAX_SPELL_CONTROL_NUM 4

enum CreatureStatus
{
	CREATURE_STATUS_NULL,

	CREATURE_STATUS_ALIVE = 0x00000001,	// 活着
	CREATURE_STATUS_DEAD = 0x00000002,	// 死亡

	CREATURE_STATUS_DEHUANMAN	= 0x00000008,	// 抗缓慢
	CREATURE_STATUS_XUANYUN		= 0x00000010,	// 眩晕
	CREATURE_STATUS_DEXUANYUN	= 0x00000020,	// 抗眩晕
	CREATURE_STATUS_HUNLUAN		= 0x00000040,	// 混乱
	CREATURE_STATUS_DEHUNLUAN	= 0x00000080,	// 抗混乱
	CREATURE_STATUS_DINGSHEN	= 0x00000100,	// 定身
	CREATURE_STATUS_DEDINGSHEN	= 0x00000200,	// 抗定身
	CREATURE_STATUS_CHENMO		= 0x00000400,	// 沉默
	CREATURE_STATUS_DECHENMO	= 0x00000800,	// 抗沉默
	CREATURE_STATUS_MIWU		= 0x00001000,	// 迷雾
	CREATURE_STATUS_DEMIWU		= 0x00002000,	// 抗迷雾
	CREATURE_STATUS_FUKONG		= 0x00004000,	// 浮空
	CREATURE_STATUS_DEFUKONG	= 0x00008000,	// 抗浮空
	CREATURE_STATUS_JIDAO		= 0x00010000,	// 击倒
	CREATURE_STATUS_DEJIDAO		= 0x00020000,	// 抗击倒
	CREATURE_STATUS_SHOUJI		= 0x00040000,	// 受击
	CREATURE_STATUS_DESHOUJI	= 0x00080000,	// 抗受击
	CREATURE_STATUS_DEJITUI		= 0x00200000,	// 抗击退
};

enum CreatureAttackType
{
	CREATURE_ATTACK_TYPE_NULL,

	CREATURE_ATTACK_TYPE_Melee		= 0x00000001,	// 近战
	CREATURE_ATTACK_TYPE_Remote		= 0x00000002,	// 远程
	CREATURE_ATTACK_TYPE_PHYSIC		= 0x00000004,	// 物理
	CREATURE_ATTACK_TYPE_SPELL		= 0x00000008,	// 法术
};

struct CreatureAttribute
{
	int32		nStatus;							// 状态(CreatureStatus二进制位运算值)
	int32		nLevel;								// 等级

	int32		nRed;								// 红(血)
	int32		nRedMax;							// 红上限
	int32		nBlue;								// 蓝(法术)
	int32		nBlueMax;							// 蓝上限

	int32		nExp;								// 经验
	int32		nSpeed;								// 速度
	int32		nRedRecover;						// 红恢复
	int32		nBlueRecover;						// 蓝恢复

	int32		nPhysicAttack;						// 物理攻击
	int32		nPhysicDefend;						// 物理防御
	int32		nSpellAttack;						// 法术攻击
	int32		nSpellDefend;						// 法术防御

	int32		nWuli;								// 武力(仅用于加成和显示)
	int32		nShenshou;							// 身手(仅用于加成和显示)
	int32		nNeili;								// 内力(仅用于加成和显示)

	int32		nAttackType;						// 攻击类型(CreatureAttackType二进制与或运算值)

	int32		nMingzhonglv;						// 命中率
	int32		nShanbilv;							// 闪避率
	int32		nBaoji;								// 暴击
	int32		nBaojilv;							// 暴击率
	int32		nGedang;							// 隔挡
	int32		nGedanglv;							// 隔挡率

	int32		nDeAttack;							// 物理、法术伤害减免
	int32		nDeMeleeAttack;						// 近战伤害减免
	int32		nDeRemoteAttack;					// 远程伤害减免
	int32		nDeSpellCD;							// 技能CD减免

	int32		nAttackRedRecover;					// 击中红恢复
	int32		nAttackBlueRecover;					// 击中蓝恢复
};

struct SpellData
{
	uint32 nSpellID;	// 技能ID
	int32 nPosition;    // 携带位置 (-1未携带,0 - 4 携带在对应的位置上)
};

#endif
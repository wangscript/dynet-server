#ifndef __BUFF_H_
#define __BUFF_H_

#include "CommonDefine.h"
#include "def_buff.h"
#include "ObjPool.h"

class Creature;

struct Buff
{
	Buff(const BuffData& rRes, Creature* _pFrom)
		: rResData(rRes), pFrom(_pFrom), nTimeElapse(0) {}

	const BuffData&		rResData;									// 资源信息
	Creature*			pFrom;										// 施法者
	int32				nTimeElapse;								// 已持续时间
	int32				arrReverseValue[MAX_BUFF_EFFECT_NUM];		// 可逆值
};

void Reverse(Buff& rBuff, Creature* pOwner);

extern ObjPool<Buff> g_cBuffFactory;

#endif
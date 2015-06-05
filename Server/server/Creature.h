#ifndef __CREATURE_H_
#define __CREATURE_H_

#include <map>

#include "Entity.h"
#include "def_creature.h"
#include "UnorderedSet.h"

struct Buff;

class Creature : public Entity, public IChannelOperate(ChannelCreature), public IChannelOperate(ChannelSpell) , public IChannelOperate(ChannelBuff)
{
public:
	Creature(EntityType eEntityType,uint32 nEntityTypeID,bool bListen,int32 registChannelFlags);

	// 收集技能信息
	DEFINE_FUNCTION_CHANNEL_COLLECT(ChannelSpell);

protected:
	
protected:

	std::multimap<int32, Buff*>		m_mapBuffs;					// 身上所带Buff
	std::queue<uint32>				m_queSpell;					// 技能施放队列

private:

	void InitAttributeOffet();

private:

	CreatureAttribute				m_sAttribute;									// 生物体属性
};


class CreatureManager : public BaseSingle<CreatureManager>
{
	typedef UNORDERED_SET<Creature*> CreatureUSetType;
public:
	CreatureManager(){}
	~CreatureManager(){}
	
	//生物体添加
	void AddCreature(Creature& rCreature);

	//生物体消失
	void DelCreature(Creature& rCreature);

	// 更新(nTimeElapse单位:毫秒)
	void Update(int32 nTimeElapse);

private:
	
	CreatureUSetType	m_usetCreaturies;			//所有生物体容器

};

#endif

#include "Server_PCH.h"
#include "Creature.h"

#include "Buff.h"
#include "util.h"
#include "UnorderedMap.h"

using namespace std;

Creature::Creature(EntityType eEntityType,uint32 nEntityTypeID,bool bListen,int32 registChannelFlags)
	: Entity(eEntityType,nEntityTypeID,bListen,registChannelFlags | CHANNEL_REGIST_TYPE_CREATURE_COLLECT | CHANNEL_REGIST_TYPE_SPELL_COLLECT)
{
	m_sChannelCreature.nID = ID();
	InitAttributeOffet();
}

IMPLEMENT_FUNCTION_CHANNEL_COLLECT(Creature,ChannelSpell)
{
	if(m_queSpell.size())
	{
		//o_rpInfo.nSpellID = m_queSpell.front(); TODO
		m_queSpell.pop();
	}else
	{
		o_rpInfo = NULL;
	}
}

void Creature::InitAttributeOffet()
{

#define	INIT_ATTRIBUTE_OFFSET(index, name, set, handler)	\
						{\
			m_sAttributeOffset[index].nOffset = &(this->m_sAttribute.name);\
			m_sAttributeOffset[index].nSize = sizeof(this->m_sAttribute.name);\
			m_sAttributeOffset[index].bCanSet = set;\
			m_sAttributeOffset[index].hHandler = handler;\
						}

	// Æ«ÒÆÐÅÏ¢
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_STATUS, nStatus, false, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_LEVEL, nLevel, false, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_RED, nRed, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_REDMAX, nRedMax, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_BLUE, nBlue, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_BLUEMAX, nBlueMax, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_EXP, nExp, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_SPEED, nSpeed, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_REDRECOVER, nRedRecover, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_BLUERECOVER, nBlueRecover, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_PHYSICATTACK, nPhysicAttack, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_PHYSICDEFEND, nPhysicDefend, true, 0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_SPELLATTACK,nSpellAttack,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_SPELLDEFEND,nSpellDefend,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_WULI,nWuli,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_SHENSHOU,nShenshou,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_NEILI,nNeili,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_ATTACKTYPE,nAttackType,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_MINGZHONGLV,nMingzhonglv,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_SHANBILV,nShanbilv,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_BAOJI,nBaoji,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_BAOJILV,nBaojilv,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_GEDANG,nGedang,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_GEDANGLV,nGedanglv,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_DEATTACK,nDeAttack,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_DEMELEEATTACK,nDeMeleeAttack,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_DEREMOTEATTACK,nDeRemoteAttack,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_DESPELLCD,nDeSpellCD,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_ATTACKREDRECOVER,nAttackRedRecover,true,0);
	INIT_ATTRIBUTE_OFFSET(ATTRIBUTE_ATTACKBLUERECOVER,nAttackBlueRecover,true,0);

#undef INIT_ATTRIBUTE_OFFSET
}

void	CreatureManager::AddCreature(Creature& rCreature)
{
	m_usetCreaturies.insert(&rCreature);
}

void	CreatureManager::DelCreature(Creature& rCreature)
{
	m_usetCreaturies.erase(&rCreature);
}

void CreatureManager::Update(int32 nTimeElapse)
{

}

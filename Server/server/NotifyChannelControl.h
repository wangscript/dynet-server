#ifndef __NOTIFY_CONTROL_H_
#define __NOTIFY_CONTROL_H_

#include "def_channel.h"
#include "ObjPool.h"


/*
 *
 *	Detail: 玩家信息管理
 *   
 *  Created by hzd 2014-12-29
 *
 */

class Player;

/*
 *
 *	Detail: 收集与通知该玩家周边信息的控制器
 *   
 *  Created by hzd 2015-1-7
 *
 */

class NotifyChannelControl
{
public:
	NotifyChannelControl(Player& rPlayer);
	~NotifyChannelControl(void);

	// 添加位置信息
	void InsertPosition(const ChannelPosition& rPosition);

	// 添加生物属性信息
	void InsertCreature(const ChannelCreature& rCreature);

	// 添加技能信息
	void InsertSpell(const ChannelSpell& rSpell);

	// 添加Buff信息
	void InsertBuff(const ChannelBuff& rBuff);

	// 更新位置信息
	void NotifyPosition();
	
	// 更新生物属性信息
	void NotifyCreature();

	// 更新技能信息
	void NotifySpell();

	// 更新Buff信息
	void NotifyBuff();

public:

protected:

protected:

private:

	//发送消息
	bool SendMsg(NetMsgHead* pMsg) const;

private:

	Player&								m_rPlayer;

	int32								m_nPositionCount;
	ChannelPosition*					m_arrPosition[MAX_POSITION_COUNT];	//位置集合信息

	int32								m_nCreatureCount;
	ChannelCreature*					m_arrCreature[MAX_CREATURE_COUNT];	//生物属性信息

	static ObjPool<ChannelPosition>		s_cChanelPositionFactory;			// 位置集合信息工厂
	static ObjPool<ChannelCreature>		s_cChanelCreatureFactory;			// 生物集合信息工厂

};


#endif



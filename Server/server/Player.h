#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

#include "Creature.h"
#include "NotifyChannelControl.h"
#include "GameDB.h"

class NetSocket;
struct NetMsgHead;

// 玩家状态
enum EClientStatus
{
	ECLIENT_STATUS_INITED		 ,	// 初始化完成 
	ECLIENT_STATUS_CONNECTED	 ,  // 已经接受了连接完成 
	ECLIENT_STATUS_ENCRYPTED	 ,	// 获得密钥成功完成 
	ECLIENT_STATUS_LOGINED		 ,	// 登录完成完成 
	ECLIENT_STATUS_SELECTED		 ,  // 选择角色完成 
	ECLIENT_STATUS_OK			 ,	// 可以进行数据交互完成  
};

// 角色类型 
enum EPlayerRoleType
{
	EPLAYER_ROLE_TYPE_1,
	EPLAYER_ROLE_TYPE_2,
	EPLAYER_ROLE_TYPE_3,
	EPLAYER_ROLE_TYPE_4,
};

/*
 *
 *	Detail: 玩家类
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class Player  : public Creature , public IChannelOperate(ChannelWear)
{
public:

	Player(NetSocket& rSocket);
	virtual ~Player();

	/*
	 *
	 *	Detail: 加载数据
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void LoadData(const DBCharacherData& rData);

	/*
	 *
	 *	Detail: 获得角色ID
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	uint64	UID();

	/*
	 *
	 *	Detail: 发送消息
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SendMsg( NetMsgHead* pMsg);
	
	/*
	 *
	 *	Detail: 获得平台账号
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	uint32 AccountID();

	/*
	 *
	 *	Detail: 设置平台ID
	 *   
	 *  Created by hzd 2015/02/14  
	 *
	 */
	void SetAccountID(int32 nAccountID);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SetStatus(EClientStatus eStatus);
	
	/*
	 *
	 *	Detail: 获得状态
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	EClientStatus GetStatus(){ return m_eStatus; };

	/*
	 *
	 *	Detail: 设置通信密钥
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SetEncryptKey(char arrNewKey[MAX_ENCRYPT_LENTH]);

	/*
	 *
	 *	Detail: 获得通信密钥
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void GetEncryptKey(char o_arrEncryptKey[MAX_ACCOUNT_LENG]);

	/*
	 *
	 *	Detail: 玩家退出
	 *   
	 *  Created by hzd 2015-1-21
	 *
	 */
	void Exist();

	/*
	 *
	 *	Detail: 定时循环，参数为延时毫秒
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void Update(uint32 nDelay);

	/*
	 *
	 *	Detail: 发送进入游戏初始化的一次性数据
	 *   
	 *  Created by hzd 2015-4-17
	 *
	 */
	void SendCharacterData();

protected:

		// 位置信息广播
		DEFINE_FUNCTION_CHANNEL_BROADCAST(ChannelPosition);

		// 生物体信息广播
		DEFINE_FUNCTION_CHANNEL_BROADCAST(ChannelCreature);

		// 装备信息广播
		DEFINE_FUNCTION_CHANNEL_BROADCAST(ChannelWear);

		// 技能信息广播
		DEFINE_FUNCTION_CHANNEL_BROADCAST(ChannelSpell);

		// Buff信息广播
		DEFINE_FUNCTION_CHANNEL_BROADCAST(ChannelBuff);

private:

	void InitAttributeOffet();

private:

	int32				m_nAccountID;						// 平台ID
	DBCharacherData		m_rCharacterData;					// 角色数据

	NetSocket&			m_rSocket;							// socket
	EClientStatus		m_eStatus;							// 工作流程状态

	int32				mSendPackCount;						//发包数量（校验包顺序、丢包情况）
	char				m_arrEncryptKey[MAX_ENCRYPT_LENTH];	//通信密钥


	NotifyChannelControl		m_rNotifyChannelControl;	// 频道消息控制器


};


/*
 *
 *	Detail: 玩家管理器
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class NetServer;

class PlayerMgr : public BaseSingle< PlayerMgr >
{

	typedef map< uint64 , Player* >	UIDPlayerMap;  // 
	typedef map< uint64 , Player* >::iterator	UIDPlayerMapIter;

public:

	PlayerMgr();
	~PlayerMgr();

	/*
	 *
	 *	Detail: 初始化所有等待连接的Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Init(NetServer& rNetServer);	

	/*
	 *
	 *	Detail: 释放Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Release();

	/*
	 *
	 *	Detail: 获得Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	Player*	GetPlayer( uint64 nUid);

	/*
	 *
	 *	Detail: 获得该Player(Player与Socket是永远绑定的，这样是为了提高效率)
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	Player&	GetPlayer(NetSocket& rSocket );
		  
	/*
	 *
	 *	Detail: 广播消息
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void SendToAll( NetMsgHead* msg );

	/*
	 *
	 *	Detail: 更新
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Update(uint32 nDelay);

private:

	Player*			m_arrPlayerCache[MAX_SOCKET_CONNECT];	// 所有玩家缓存(连接与未连接)
	UIDPlayerMap	m_mapUIDPlayer;							// UID对应Player	

};

#endif


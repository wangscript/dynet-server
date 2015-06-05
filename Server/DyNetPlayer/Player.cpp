#include "Player.h"

Player::Player():m_pSocket(NULL)
{
	//memcpy(m_arrEncryptKey,EncryptDefualt,MAX_ENCRYPT_LENTH);
	InitAttributeOffet();
}

Player::~Player()
{
}

uint64	Player::UID()
{
	//return m_rCharacterData.nUID;
	return 0;
}

void Player::InitAttributeOffet()
{
	// 常用的字段进行注册函数（修改、回调等函数）
}


void Player::SendMsg(NetMsgHead* pMsg)
{
	//// 加密处理
	//static char arrBuffer[MAX_SOCKET_BUFFER];

	//// 处理动态包情况
	//memcpy(&arrBuffer[0],pMsg,pMsg->GetPackLength());
	//
	//Encrypt::XorCode(pMsg->nSize,m_arrEncryptKey,&arrBuffer[sizeof(pMsg->nSize)],pMsg->nSize);
	//pMsg = ( NetMsgHead*)&arrBuffer[0];

	//m_rSocket.ParkMsg(pMsg,pMsg->GetPackLength());
	//m_rSocket.SendMsg();
}


void Player::SetStatus(EPlayerStatus eStatus)
{
	if (EPLAYER_STATUS_INITED == eStatus)
	{
		//memcpy(m_arrEncryptKey, EncryptDefualt, MAX_ENCRYPT_LENTH);
	}
	m_eStatus = eStatus;
}



uint32 Player::AccountID()
{
	return m_nAccountID;
}

void Player::SetAccountID(int32 nAccountID)
{
	m_nAccountID = nAccountID;
}


void Player::Exist()
{
	//m_rSocket.Disconnect();
}

void Player::Update(float64 nDelay)
{

}

void Player::SendCharacterData()
{

	SetStatus(EPLAYER_STATUS_OK);
}


PlayerMgr::PlayerMgr()
{
}

PlayerMgr::~PlayerMgr()
{

}


void PlayerMgr::Init()
{
	for(int32 i = 0; i < MAX_PLAYER_CONNECT; i++)
	{
		m_arrPlayerCache[i] = new Player();
	}
}


void PlayerMgr::Release()
{
	for(int32 i = 0; i < MAX_PLAYER_CONNECT; i++)
	{
		delete m_arrPlayerCache[i];
	}
	delete s_pInstance;
	s_pInstance = NULL;
}


Player*	PlayerMgr::GetPlayer(uint64 nUid)
{
	UIDPlayerMapIter iter = m_mapUIDPlayer.find(nUid);
	if(iter == m_mapUIDPlayer.end())
	{
		return NULL;
	}
	return iter->second;
}


Player&	PlayerMgr::GetPlayer(uint32 nIndex)
{
	return *m_arrPlayerCache[nIndex];
}


PlayerMgr* PlayerMgr::s_pInstance = NULL;

void PlayerMgr::SendToAll(NetMsgHead* pMsg)
{
	UIDPlayerMapIter iter = m_mapUIDPlayer.begin();
	UIDPlayerMapIter iterend = m_mapUIDPlayer.end();

	for(; iter != iterend; ++iter)
	{
		iter->second->SendMsg(pMsg);
	}
}

void PlayerMgr::Update(uint32 nDelay)
{

	UIDPlayerMapIter it = m_mapUIDPlayer.begin();
	UIDPlayerMapIter itEnd = m_mapUIDPlayer.end();
	Player* pPlayer = NULL;
	for(; it != itEnd; ++it)
	{
		pPlayer = it->second;
		pPlayer->Update(nDelay);
	}

}


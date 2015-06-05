#include "Client_PCH.h"
#include "ClientPlayer.h"

ClientPlayer::ClientPlayer(void)
{
	memcpy(m_arrEncryptKey,EncryptDefualt,MAX_ENCRYPT_LENTH);

}


ClientPlayer::~ClientPlayer(void)
{
}


void	ClientPlayer::SendMsg( NetMsgHead* pMsg ,int32 nSize)
{
	// ¼ÓÃÜ´¦Àí
	static char msgBuffer[65536];
	memcpy(&msgBuffer[0],pMsg,nSize);
	Encrypt::XorCode(nSize,m_arrEncryptKey,msgBuffer,nSize);
	pMsg = ( NetMsgHead*)&msgBuffer[0];
	mSocket->ParkMsg(pMsg,nSize);
	mSocket->SendMsg();
}

void					ClientPlayer::SetSocket( NetSocket& rSocket )
{
	mSocket = &rSocket;
}

NetSocket*              ClientPlayer::GetSocket()
{
	return mSocket;
}

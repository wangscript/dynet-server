#include "Server_PCH.h"
#include "NetLoginHandler.h"
#include "NetMsgHandler.h"
#include "DBLoginHandler.h"

#include "Player.h"
#include "GlobalVar.h"
#include "GameDB.h"

void NetRequestClose( Player& rPlayer , NetMsgHead* pHead )
{
	rPlayer.Exist();
}

void NetLoginEncryptInfoHandler( Player& rPlayer , NetMsgHead* pHead )
{
	if (ECLIENT_STATUS_CONNECTED == rPlayer.GetStatus())
	{
		char arrRandKey[MAX_ENCRYPT_LENTH];
		Encrypt::RandKey(arrRandKey);

		{
			S2CEncryptInfo sMsg;
			memcpy(sMsg.arrEncryptKey, arrRandKey, MAX_ENCRYPT_LENTH);
			rPlayer.SendMsg(&sMsg);
		}

		// 要先发数据后才能改
		rPlayer.SetEncryptKey(arrRandKey);
		rPlayer.SetStatus(ECLIENT_STATUS_ENCRYPTED);
		
	}
}


void NetLoginAccountHandler( Player& rPlayer , NetMsgHead* pHead )
{

	if (ECLIENT_STATUS_ENCRYPTED == rPlayer.GetStatus())
	{
		const C2SsLogin& rPacket = *static_cast<const C2SsLogin*>(pHead);
		if(IDbSession* pDB = g_rDbDatabaseMgr.GetMainDB())
		{
			SPRINTF(SQL_BUFFER,"SELECT `type_idx`,`username` FROM `account` WHERE  `username`='%s' AND `password`='%s';", rPacket.arrUsername,rPacket.arrPassword);
			SQL_BUFFER[ MAX_SQL_BUFFER - 1 ] = '\0';
			pDB->ExecuteAsyncSQL(SQL_BUFFER,&rPlayer,DBLoginRepAccount);
			return;
		}
	}

}

void NetLoginCharacterListHandler( Player& rPlayer , NetMsgHead* pHead )
{

}

/*
* 选择角色进入游戏
*/
void NetLoginSelectCharacterHandler( Player& rPlayer , NetMsgHead* pHead )
{

	if(ECLIENT_STATUS_LOGINED == rPlayer.GetStatus())
	{
		const C2SSelectCharacter& rPacket = *static_cast<const C2SSelectCharacter*>(pHead);
		if(IDbSession* pDB = g_rDbDatabaseMgr.GetMainDB())
		{
			// 设置为 去掉旧的 selected ，加新的selected
			// 返回需要的所有数据  存储过程 
			SPRINTF(SQL_BUFFER, "CALL SP_SELECTED_CHARCATER(%ld)", rPacket.nID);
			SQL_BUFFER[MAX_SQL_BUFFER - 1] = '\0';
			pDB->ExecuteAsyncSQL(SQL_BUFFER, &rPlayer, DBLoginRepSelectRole);
			return;
		}
	}

}

void NetLoginCreateCharacterHandler( Player& rPlayer , NetMsgHead* pHead )
{

	if(ECLIENT_STATUS_LOGINED == rPlayer.GetStatus())
	{
		const C2SCreateRole& rPacket = *static_cast<const C2SCreateRole*>(pHead);
		// 使用存储过程创建角色，检查重复昵称，数量等
		if(IDbSession* pDB = g_rDbDatabaseMgr.GetMainDB())
		{
			SPRINTF(SQL_BUFFER, "CALL SP_CREATE_ROLE(%d,'%s',%d,%d);", rPlayer.AccountID(), rPacket.arrNick, rPacket.nType , MAX_ROLE_TYPE_COUNT);
			SQL_BUFFER[MAX_SQL_BUFFER - 1] = '\0';
			pDB->ExecuteAsyncSQL(SQL_BUFFER, &rPlayer, DBLoginRepCreateCharacter);
			return;
		}
	}
}

void NetLoginResourcesLoaded( Player& rPlayer , NetMsgHead* pHead )
{

	if(ECLIENT_STATUS_SELECTED == rPlayer.GetStatus())
	{

		// 发送角色进入游戏所需要的一次性数据 TODO
		rPlayer.SendCharacterData();
		
	}
}



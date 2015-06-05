#ifndef __COMMON_F2C_H_
#define __COMMON_F2C_H_

#include "CommonDefine.h"

enum EProF2C
{

	PRO_F2C_ENCRYPT_INFO			= PRO_F2C_BASE + 0,	// 返回密钥信息
	PRO_F2C_LOGIN_READY_FINISH		= PRO_F2C_BASE + 1, // 服务器对即将登录请求准备完成

};

#pragma pack(push, 1)


// 返回密钥
struct F2CEncryptInfo : public NetMsgHead
{
	F2CEncryptInfo():NetMsgHead(PRO_F2C_ENCRYPT_INFO)
	{
		
	}
	char arrEncryptKey[MAX_ENCRYPT_LENTH];
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

struct F2CLoginReadyFinish : public NetMsgHead
{
	F2CLoginReadyFinish():NetMsgHead(PRO_F2C_LOGIN_READY_FINISH)
	{

	}
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};


#pragma pack(pop) 

#endif
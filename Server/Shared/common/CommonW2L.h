#ifndef __COMMON_W2L_H_
#define __COMMON_W2L_H_


#include "BaseDefine.h"
#include "../DyNetSocket/NetConfig.h"
#include "CommonDefine.h"

// ---------------w2ls 300 - 399

enum EProW2Ls
{

	PRO_W2LS_LOGINED = 300, // 登录结果
	PRO_W2LS_SERVERINFO_LIST = 301,


};

#pragma pack(push,1)

struct W2LsLogined : public NetMsgHead
{

	W2LsLogined():NetMsgHead(PRO_W2LS_LOGINED)
	{

	}
	enum
	{
		SUCCESS,
		FAIL,
	};
	int32 nResult; // 0 成功,其他失败
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};



#pragma pack(pop)




#endif
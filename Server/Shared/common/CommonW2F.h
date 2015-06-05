#ifndef __COMMONW2F_H_
#define __COMMONW2F_H_


#include "BaseDefine.h"
#include "../DyNetSocket/NetConfig.h"

//----------w2fep 200-300-------------

enum EProW2Fep
{
	PRO_W2FEP_LOGINED = 200 , // 登录结果

};



#pragma pack(push, 1)

// fep登录结果
struct W2FepLogined : public NetMsgHead
{
	W2FepLogined():NetMsgHead(PRO_W2FEP_LOGINED)
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

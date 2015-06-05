#ifndef __COMMON_C2W_H_
#define __COMMON_C2W_H_


#include "CommonDefine.h"

enum EProC2W
{

	PRO_C2W_SWITCH_SCENE = PRO_C2W_BASE + 0, // ¸ü»»³¡¾° 

};


#pragma pack(push,1)

struct C2WSwitchScene : public NetMsgHead
{
	C2WSwitchScene():NetMsgHead(PRO_C2W_SWITCH_SCENE)
	{
		nSceneID = 0;
	}
	int32 nSceneID;
	inline int32 GetPackLength()const
	{
		return sizeof(*this);
	}
};

#pragma pack(pop)










#endif
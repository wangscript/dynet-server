#ifndef _BASE_SESSION_H_
#define _BASE_SESSION_H_

#include "BaseDefine.h"
#include "../DyNetSocket/NetSocket.h"

struct NetMsgHead;


class BaseSession
{
public:

	BaseSession():m_nStatus(0)
	{

	}

	virtual void Exist() = 0;

	virtual void SetStatus(int32 nStatus) = 0;

	virtual int32 Status() = 0;
	

protected:

	int32		m_nStatus; // ״̬

private:


};

#endif

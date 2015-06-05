
/*
 *
 *	Detail: IOBuffer
 *   
 *  Copyright (c) Created by hzd 2012. All rights reserved.
 *
 */

#ifndef SWA_NETIOBUFFER_H_
#define SWA_NETIOBUFFER_H_

#include "NetConfig.h"

class DLLAPI NetIOBuffer
{
public:

	NetIOBuffer();
	~NetIOBuffer();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   InitBuffer(uint32 nMax);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   ReleaseBuffer();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	bool   Write(char* c, uint32 nLen);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   Read(void** b, uint32 nLen);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	uint32 ReadRemove(void* pMsg,uint32 nLen);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   RemoveBuffer(uint32 nLen);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	uint32 GetLen();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	uint32 GetSpace();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void*  GetBuffer();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void*  GetStart();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void*  GetBufferEnd();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   ClearBuffer();

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/05/31  
	 *
	 */
	void   MoveBuffer();

protected:

	mutex   m_cMutex;		// 读/写锁
	uint32  m_nBegin;       // 下一次读/写的位置
	uint32  m_nLen;			// 已经接收数据长度(可能有多个包) 
	char*   m_pBuffer;		// 字段容器 
	uint32	m_nMax;         // 最大字段长度 

};

#endif




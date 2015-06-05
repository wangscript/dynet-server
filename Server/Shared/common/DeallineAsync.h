#ifndef __DEALINE_ASYNC_H_
#define __DEALINE_ASYNC_H_

#include "BaseDefine.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio::ip;
using namespace boost::asio;


/*
 *
 *	Detail: 采用策略设计模式（因为每一种的定时器都不一样）
 *	
 *   
 *  Created by hzd 2015-6-4
 *
 */


/*
 *
 *	Detail: 异步定时器
 *   
 * Copyright (c) Created by hzd 2015-6-4.All rights reserved
 *
 */

typedef boost::function<void(int32 nSrvTimes)> DeallineAsyncHandler;

class DeallineAsync : public io_service
{
public:

	/*
	 *
	 *	Detail: 回调时间(毫秒)，次数，回调函数
	 *   
	 *  Created by hzd 2015-6-4
	 *
	 */
	DeallineAsync(int32 nIntervalTime,int32 nCallTimes,DeallineAsyncHandler pHander);

	~DeallineAsync();

	/*
	 *
	 *	Detail: 开始启动
	 *   
	 *  Created by hzd 2015-6-4
	 *
	 */
	void Start();

	/*
	 *
	 *	Detail: 回立刻回调最后一次
	 *   
	 *  Created by hzd 2015-6-4
	 *
	 */
	void Cancel(); 

private:

	void DoHandler();

private:

	int32   m_nIntervalTime;	// 每次间隔时间(毫秒)
	int32	m_nMaxCallTimes;    // 最大调用次数(0无限次，1次)
	int32	m_nHadCallTimes;    // 已经调用次数

	DeallineAsyncHandler	m_fAsyncHandler;  // 回调函数
	deadline_timer			m_ctimer;

};



#endif


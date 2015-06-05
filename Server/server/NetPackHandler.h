#ifndef NET_PACK_HANDLER_H_
#define NET_PACK_HANDLER_H_

#include "NetMsgHandler.h"

/*
 *
 *	Detail: 背包整理
 *   
 *  Created by hzd 2014-12-9
 *
 */
void	NetPackReorganizeHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 交换位置
 *   
 *  Created by hzd 2014-12-9
 *
 */
void	NetPackExchangePosition( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 开启格子
 *   
 *  Created by hzd 2014-12-9
 *
 */
void	NetPackOpenGrid( Player& rPlayer , NetMsgHead* pHead );

#endif


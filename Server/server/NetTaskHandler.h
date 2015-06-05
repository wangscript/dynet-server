
#ifndef _NET_TASK_HANDLER_H_
#define _NET_TASK_HANDLER_H_


#include "NetMsgHandler.h"


// 任务列表
void	NetTaskListHandler( Player& rPlayer , NetMsgHead* pHead );

// 任务接受
void	NetTaskAcceptHandler( Player& rPlayer , NetMsgHead* pHead );

// 任务完成(提交)
void	NetTaskFinishHandler( Player& rPlayer , NetMsgHead* pHead );












#endif





#ifndef NET_RELATION_HANDLER_H_
#define NET_RELATION_HANDLER_H_

#include "NetMsgHandler.h"

/*
 *
 *	Detail: 好友列表
 *   
 *  Created by hzd 2014-12-11
 *
 */
void	NetRelationListHandler( Player* player , NetMsgHead* head );

/*
 *
 *	Detail: 添加好友by uid or by nick
 *   
 *  Created by hzd 2014-12-11
 *
 */
void	NetRelationAddByUidsHandler( Player* player , NetMsgHead* head );

/*
 *
 *	Detail: 通过昵称添加好友
 *   
 *  Created by hzd 2014-12-11
 *
 */
void	NetRelationAddByNickHandler( Player* player , NetMsgHead* head );

/*
 *
 *	Detail: 删除好友
 *   
 *  Created by hzd 2014-12-11
 *
 */
void	NetRelationRemoveHandler( Player* player , NetMsgHead* head );

/*
 *
 *	Detail: 好友详细信息
 *   
 *  Created by hzd 2014-12-11
 *
 */
void	NetRelationDetailHandler( Player* player , NetMsgHead* head );



#endif
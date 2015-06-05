#ifndef _NET_PET_HANDLER_H_
#define _NET_PET_HANDLER_H_

#include "NetMsgHandler.h"

/*
 *
 *	Detail: 宠物相差接口
 *   
 *  Created by hzd 2014-10-2
 *
 */

/*
 *
 *	Detail: 宠物列表
 *   
 *  Created by hzd 2014-10-22
 *
 */
void	NetPetListHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 宠物详细信息
 *   
 *  Created by hzd 2014-10-22
 *
 */
void	NetPetInfoHandler( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 宠物培育，性格改变
 *    
 *  Created by hzd 2014-10-22
 *
 */
void	NetPetEducation( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 宠物训练，增加宠物经验，提升等级
 *   
 *  Created by hzd 2014-10-22
 *
 */
void	NetPetTrainning( Player& rPlayer , NetMsgHead* pHead );

/*
 *
 *	Detail: 宠物进化、母系不变、类型改变，需要进化丹
 *   
 *  Created by hzd 2014-10-22
 *
 */
 void	NetPetAdvanced( Player& rPlayer , NetMsgHead* pHead );                            

/*
 *
 *	Detail: 宠物变异、母系改变，需要全体丹
 *   
 *  Created by hzd 2014-10-22
 *
 */
void	NetPetVariation( Player& rPlayer , NetMsgHead* pHead );


void	NetPetAdd( Player& rPlayer , NetMsgHead* pHead );

#endif

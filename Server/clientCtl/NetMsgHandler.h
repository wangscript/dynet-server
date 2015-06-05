#ifndef _NETMSGHANDLER_H_
#define _NETMSGHANDLER_H_

#include "CommonDefine.h"

typedef void( *NetMsgHandlerf )(ClientPlayer* pPlayer,NetMsgHead* pHead ,int32 nSize);

typedef struct 
{
    uint32 nType;// 协议
    NetMsgHandlerf pFun;	// 处理函数

}   NetMsgHandler;

typedef	map< uint32 , NetMsgHandler > NetMsgHandlerMap;
typedef map< uint32 , NetMsgHandler >::iterator NetMsgHandlerMapIter;

bool CheckMsg(const NetMsgHead* msg , uint32 size );

#define	CHECKMSG( classType ) const classType* msg = (classType*)head; if ( !CheckMsg( msg , sizeof( classType ) ) ){ FLOG_ERROR("CheckMsg Error! %d",msg->nType); return ;}

void OnNetMsgEnter( NetSocket& socket );
void OnNetMsgExit( NetSocket& socket );
void OnNetMsg( NetSocket& socket, NetMsgHead* head,int32 nSize );

void OnError( int32 error , int32 ServiceID , NetSocket* socket , const char* buffer );
void InitNetMsg();

#endif


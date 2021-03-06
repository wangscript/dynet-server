#ifndef SWA_NETSOCKET_H_
#define SWA_NETSOCKET_H_


#include "NetIOBuffer.h"

/*
 *
 *	Detail: Socket类
 *
 *  Created by hzd 2014-1-1
 *
 */
enum ERecvType
{
	REVC_FSRS_NULL = 0,
	REVC_FSRS_HEAD,
	REVC_FSRS_BODY,
};
enum  EMsgRead
{
	MSG_READ_INVALID,
	MSG_READ_WAITTING,
	MSG_READ_OK,
	MSG_READ_REVCING,
};

class DLLAPI NetSocket : public tcp::socket
{
	friend class NetServer;
	friend class NetClient;
public:
	NetSocket(io_service& rIo_service,int32 nMaxRecivedSize,int32 nMaxSendoutSize,int32 nMaxRecivedCacheSize,int32 nMaxSendoutCacheSize);
	virtual ~NetSocket();

	/*
	 *
	 *	Detail: 获得SocketID，从0开始自增
	 *
	 *  Created by hzd 2015/01/21
	 *
	 */
	int32 SID();

	/*
	 *
	 *	Detail: 压入要准备发送的数据
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void ParkMsg(NetMsgHead* pHead,int32 nLength);

	/*
	 *
	 *	Detail: 发送数据
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void SendMsg();

	/*
	 *
	 *	Detail: 读取缓存中的数据
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	EMsgRead ReadMsg(NetMsgHead** pHead,int32& nSize);

	/*
	 *
	 *	Detail: 移除缓存中的数据
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void RemoveMsg(uint32 nLen);

	/*
	 *
	 *	Detail: 恢复到构造函数那种状态
	 *
	 *  Created by hzd 2015-1-20
	 *
	 */
	void Clear();

	/*
	 *
	 *	Detail: 启动Socket，可以进入收发数据
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void Run();

	/*
	 *
	 *	Detail: 获得Socket客户端IP地址
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	const string GetIp();

	/*
	 *
	 *	Detail: 将关闭Socket(主动断开客户端)
	 *   
	 * Copyright (c) Created by hzd 2015-4-29.All rights reserved
	 *
	 */
	void SetWillColse();

	/*
	 *
	 *	Detail: 超过断开
	 *   
	 *  Created by hzd 2015-6-2
	 *
	 */
	void SetTimeout(int32 nTimeout);

	/*
	 *
	 *	Detail: 定时启动
	 *   
	 *  Created by hzd 2015-6-2
	 *
	 */
	void TimeoutStart();

	/*
	 *
	 *	Detail: 定时关闭
	 *   
	 *  Created by hzd 2015-6-2
	 *
	 */
	void TimeoutCancel();

private:

	NetSocket();

	/*
	 *
	 *	Detail: 断开socket
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void Disconnect();

	/*
	 *
	 *	Detail: 关闭socket
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void HandleClose(const boost::system::error_code& error);

	/*
	 *
	 *	Detail: 收到指定长度数据回调函数
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void RecvMsg(const boost::system::error_code& ec, size_t bytes_transferred);

	/*
	 *
	 *	Detail: 发送数据回调函数，发送后发现有新数据，则会选择断续发送（一般不会发生这种情况）
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void SendMsg(const boost::system::error_code& ec, size_t bytes_transferred);

	/*
	 *
	 *	Detail: 读消息头
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void ReadHead();

	/*
	 *
	 *	Detail: 读消息主体
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void ReadBody();

	/*
	 *
	 *	Detail: 超时回调函数，将会关闭socket
	 *
	 *  Created by hzd 2015-1-21
	 *
	 */
	void HandleWait(const boost::system::error_code& error);

private:

	int32				m_nID;				// socketID，一个进程所的Socket唯一ID从0开始 
	int32				m_nIndexID;			// 自定义ID, 可以用数组下标，方便管理

	deadline_timer		m_cTimer;			// 收发定时器，用于检测用户多久没有与服务器通信，否则会角色HandleWait函数 
	deadline_timer		m_cCloseTimer;		// 关闭连接定时器

	NetIOBuffer			m_cIBuffer;			// 管理收到的字节管理器 
	NetIOBuffer			m_cOBuffer;			// 管理发出的字节管理器 

	mutable_buffers_1	m_cHeadBuffer;		// 收到头数据缓存绑定类 
	mutable_buffers_1	m_cBodyBuffer;      // 收到的主体数据绑在绑定类 
	mutable_buffers_1	m_cSendBuffer;      // 发送的数据缓存绑定类 

	ERecvType			m_eRecvStage;
	uint32				m_nBodyLen;			// 主体数据长度（不包括包头） 
	char				m_arrRecvBuffer[65536];

	int32				m_bSending;
	char				m_arrSendBuffer[65536];

	int32				m_nMaxRecivedSize;
	int32				m_nMaxSendoutSize;

	int32				m_nTimeout;			// 超时断开,0不,>0指定秒数

	bool				m_bExit;

};

typedef boost::function<void(NetSocket& pSocket)>	PNetServerEnterHandler;
typedef boost::function<void(NetSocket& pSocket)>	PNetServerExistHandler;
typedef boost::function<void(NetSocket& pSocket, NetMsgHead* pMsg,int32 nSize)>	PNetServerOnMsgHandler;


typedef list<NetSocket*> SocketList;
typedef list<NetSocket*>::iterator SocketListIter;
typedef vector<NetSocket*> SocketVector;
typedef vector<NetSocket*>::iterator SocketVectorIter;


#endif

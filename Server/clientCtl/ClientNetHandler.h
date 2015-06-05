#ifndef CLIENT_NET_HANDLER_H_
#define CLIENT_NET_HANDLER_H_

/*
 *
 *	Detail: 客户端Handler事件
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class ClientNetHandler
{
public:

	ClientNetHandler(void);
	~ClientNetHandler(void);

	// 请求由服务器主动断开
	static void SendRequestClose(ClientPlayer* gPlayer);

	/////////////////////////////发送/////////////////////////////////////////////
	static void SendEncrypt(ClientPlayer* gPlayer);
	static void SendLogin(ClientPlayer* gPlayer);
	static void SendCharacterList(ClientPlayer* gPlayer);
	static void SendRegister(ClientPlayer* gPlayer);
	static void SendChar2World(ClientPlayer* gPlayer);
	static void SendLoadResed(ClientPlayer* gPlayer);

	/////////////////////////////接收/////////////////////////////////////////////

	static void RecvEncryptInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RecvLoginReadyFinish(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);

	static void RevcCharacterList(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RevcLoginResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RecvSelectResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RecvRegisterResult(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RecvCharacterInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);
	static void RecvChatInfo(ClientPlayer* gPlayer,NetMsgHead* pHead,int32 nSize);


};

#endif


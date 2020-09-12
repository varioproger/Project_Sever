#pragma once
#include "MySQLDataBaseManager.h"
#include "ListenSocket.h"
#include "IOCPManager.h"
#include "CProtocol.h"
#include "ClientManager.h"
#include "LoginManager.h"
#include "ChatManager.h"
#include "Lobby_Manager.h"
#include "Player_Manager.h"

class CServerManager : public CIOCPManager
{
private:
	static CServerManager* pthis;

	CServerManager();
	~CServerManager();

	CListenSocket* mListenSock;
public:
	static CServerManager* GetInstance();
	static void Destory();

	bool Begin();
	void Run();
	void End();

	bool Recv(void* ptr, int len);
	bool Send(void* ptr, int len);
	void Disconneted(void* ptr);
	void Accept(void* ptr);
};
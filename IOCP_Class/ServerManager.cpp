#include "ServerManager.h"
#include"LoginState.h"
#include"ChatState.h"
#include"Lobby_State.h"
#include"Player_State.h"
CServerManager* CServerManager::pthis = nullptr;

CServerManager* CServerManager::GetInstance()
{
	/*Create 순서
	ERROR -> PROTOCOL -> ClientManager-> DB -> Login -> Lobby -> CHAT n PLAYER -> ServerManager
	*/
	CError::Create();
	CProtocol::Create();
	ClientManager::Create();
	CMySQLDBManager::Create();
	LoginManager::Create();
	Lobby_Manager::Create();
	ChatManager::Create();
	Player_Manager::Create();
	if (!pthis) pthis = new CServerManager();
	return pthis;
}

void CServerManager::Destory()
{
	/*Destroy 순서
	CHAT n PLAYER-> Lobby-> Login-> ClientManager-> DB -> PROTOCOL-> ERROR-> ServerManager
	*/
	Player_Manager::Destroy();
	ChatManager::Destroy();
	Lobby_Manager::Destroy();
	LoginManager::Destroy();
	ClientManager::Destroy();
	CMySQLDBManager::Destroy();
	CProtocol::Destroy();
	CError::Destroy();
	if (pthis) delete pthis;
}

CServerManager::CServerManager()
{
	mListenSock = nullptr;
}

CServerManager::~CServerManager()
{
	if (mListenSock != nullptr)
		delete mListenSock;
}

bool CServerManager::Begin()
{
	

	// WSA Setting
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

	// IOCP Setting
	if (!BeginIOCP()) return false;

	// Listen Setting
	mListenSock = new CListenSocket();
	mListenSock->TCP_Setting(nullptr, PORT);
	mListenSock->Listen();

	// Manager Setting/
	CMySQLDBManager::GetInstance()->Begin();
	ClientManager::GetInstance()->Begin();
	LoginManager::GetInstance()->Begin();
	Lobby_Manager::GetInstance()->Begin();
	ChatManager::GetInstance()->Begin();
	Player_Manager::GetInstance()->Begin();

	/*
	state input
	INIT_STATE =  0x0100000000000000,
	LOGIN_STATE = 0x0200000000000000,
	CHAT_STATE =  0x0300000000000000,
	LOBBY_STATE = 0x0400000000000000,
	PLAYER_STATE = 0x0500000000000000,
	*/
	this->class_state.push_back((unsigned __int64)CLASS_STATE::INIT_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::CHAT_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::LOBBY_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::LOGIN_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::PLAYER_STATE);
	return true;
}

void CServerManager::Run()
{
	while (1)
	{
		SOCKET sock = mListenSock->Accept();
		if (sock == INVALID_SOCKET)
		{
			CError::GetInstance()->err_display("Accept");
			break;
		}

		RegisterIOCP(sock, 0);

		WSAOverLapped_EX* wsa_accept = new WSAOverLapped_EX;
		memset(wsa_accept, 0, sizeof(WSAOverLapped_EX));

		wsa_accept->ptr = (void*)sock;
		wsa_accept->type = IO_ACCEPT;

		PostQueuedCompletionStatus(hcp, 0, sock, (LPOVERLAPPED)wsa_accept);
	}
}

void CServerManager::End()
{
	/*Destroy 순서
	CHAT n PLAYER-> Lobby-> Login-> ClientManager-> DB -> PROTOCOL-> ERROR-> ServerManager
	*/
	Player_Manager::GetInstance()->End();
	ChatManager::GetInstance()->End();
	Lobby_Manager::GetInstance()->End();
	LoginManager::GetInstance()->End();
	ClientManager::GetInstance()->End();
	CMySQLDBManager::GetInstance()->End();
	WSACleanup();
	Destory();
}

bool CServerManager::Recv(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Recv");
		return true;
	}

	int result = client->CompleteRecv(len);

	switch (result)
	{
	case SOC_ERROR:
	case SOC_FALSE:
		return false;
	case SOC_TRUE:
		break;
	}
	//class state 마다 setstate를 해줘야 함. 그럴려면 여기서 Clock 있어야 하는데...
	unsigned __int64 full_code = client->GetProtocol();

	for (int i = 0; i < class_state.size(); i++)
	{
		if (CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)class_state[i], NULL, NULL))
		{
			switch ((CLASS_STATE)class_state[i])
			{
			case CLASS_STATE::INIT_STATE:
				printf("INIT_STATE\n");
				break;
			case CLASS_STATE::LOGIN_STATE:
				printf("LOGIN_STATE\n");
				client->SetState(client->GetLoginState());
				break;
			case CLASS_STATE::CHAT_STATE:
				printf("CHAT_STATE\n");
				client->SetState(client->GetChatState());
				break;
			case CLASS_STATE::LOBBY_STATE:
				printf("LOBBY_STATE\n");
				client->SetState(client->GetLobbyState());
				break;
			case CLASS_STATE::PLAYER_STATE:
				printf("PLAYER_STATE\n");
				client->SetState(client->GetPlayerState());
				break;
			}
			//초기화를 init_state로 했기 때문에 여기선 따로 설정하지 않는다. 시작과 동시에 CHAT_STATE 로 가게끔 설정해둠.		
		}
	}
	printf("OUT\n");
	client->GetState()->RecvProcess(client);

	if (!client->Recv())
	{
		return false;
	}

	return true;
}

bool CServerManager::Send(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Recv");
		return true;
	}

	int result = client->CompleteSend(len);

	switch (result)
	{
	case SOC_ERROR:
	case SOC_FALSE:
		return false;
	case SOC_TRUE:
		break;
	}

	client->GetState()->SendProcess(client);
	return true;
}

void CServerManager::Accept(void* ptr)
{
	printf("Accept\n");

	WSAOverLapped_EX* overlapped = (WSAOverLapped_EX*)ptr;
	SOCKET sock = (SOCKET)overlapped->ptr;
	// Add
	CClientSection* client = ClientManager::GetInstance()->AddClient(sock);

	delete overlapped;

	if (!client->Recv())
	{
		return;
	}
}

void CServerManager::Disconneted(void* ptr)
{
	printf("Disconneted\n");

	// Remove
	Player_Manager::GetInstance()->Remove_From_Game((CClientSection*)ptr);
	LoginManager::GetInstance()->Logout((CClientSection*)ptr);
	ChatManager::GetInstance()->Remove((CClientSection*)ptr);
	Lobby_Manager::GetInstance()->Remove((CClientSection*)ptr);
	ClientManager::GetInstance()->RemoveClient((CClientSection*)ptr);
	
}
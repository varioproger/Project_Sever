#pragma once
#include "Packing.h"
#include "State.h"
#include "Player.h"

class CInitState;
class CLoginState;
class ChatState;
class Lobby_State;
class Player_State;

class CClientSection : public CPacking
{
protected:
	UserInfo* user;
	bool login_state;

	Player* player;
	CState* mState;

	CInitState* mInitState;
	CLoginState* mLoginState;
	ChatState* mChatState;
	Lobby_State* mLobbyState;
	Player_State* mPlayerState;
public:
	CClientSection();
	CClientSection(SOCKET _sock);
	~CClientSection();

	void SetState(CState* state);
	CState* GetState();
	CInitState* GetinitState();
	CLoginState* GetLoginState();
	ChatState* GetChatState();
	Lobby_State* GetLobbyState();
	Player_State* GetPlayerState();
	void Login(UserInfo* _user);
	void Logout();

	UserInfo* GetUser() { return user; }
	bool IsLogin() { return login_state; }

	void player_online(Player* _player);
	void player_offline(Player* _player);
	Player* Getplayer() { return player; }
};
#pragma once
#include"Lobby.h"
#include "CProtocol.h"

class Lobby_Manager : public CMultiThreadSyns<Lobby_Manager>
{
public:
	
	void Begin();
	void End();
	bool Add(CClientSection* ptr);
	bool Remove(CClientSection* ptr);
private:
	Lobby_Manager();
	~Lobby_Manager() = default;
	//이부분 수정
	enum class RESULT
	{
		NOT_READY,
		READY
	};
	int Packing(char* buf, int result, const char* str);
private:
	DECLARE_SINGLETONE(Lobby_Manager)
	CProtocol* protocol;
	vector<Lobby*>* lobby_list;
};


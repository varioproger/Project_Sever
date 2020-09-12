#pragma once
#include "MySQLDataBaseManager.h"
#include "CProtocol.h"
#include "ClientSection.h"


class LoginManager : public CMultiThreadSyns<LoginManager>
{
private:
	DECLARE_SINGLETONE(LoginManager)
	CProtocol* protocol;
	LoginManager();
	~LoginManager();
private:
	vector<UserInfo*>* mUser_List;
	list<UserInfo*>* mNowLoginUser_List;
	//�̺κ� ����
	enum RESULT
	{
		NODATA = -1,
		JOIN_SUCCESS = 1,
		ERROR_JOIN_EXISTS,
		LOGIN_SUCCESS,
		ERROR_LOGIN_ID,
		ERROR_LOGIN_PW,
		ERROR_LOGIN_EXISTS
	};
	
public:
	//�̺κ� ����
	enum class PROTOCOL : unsigned __int64
	{
		JOIN_INFO =   0x0000010000000000,
		JOIN_RESULT = 0x0000020000000000,
		LOGIN_INFO =  0x0000030000000000,
		LOGIN_RESULT =0x0000040000000000
	};
	bool UserDataLoad();

	void Begin();
	void End();
	bool Join(CClientSection* ptr);
	bool Login(CClientSection* ptr);
	void Logout(CClientSection* ptr);
	int Packing(char* buf, int result, const char* str);
	int Packing(char* buf, int result,int num ,const char* str);
	void UnPacking(const char* buf, char* id, char* pw);
	void UnPacking(const char* buf, char* id, char* pw, char* name);
};
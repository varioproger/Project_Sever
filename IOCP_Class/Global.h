#pragma once
#pragma warning (disable : 4996)

#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

#include <mysql.h>
#pragma comment (lib, "libmysql.lib")

#include <stdio.h>
#include <stdlib.h>

#define PORT 9050
#define BUFSIZE 4096
#define MSGSIZE 128

#include <vector>
#include <list>
#include <queue>
#include<iostream>
using namespace std;

#define DB_HOST "localhost"	// DB IP�ּ� �Ǵ� ������ �Ǵ� localhost
#define DB_USER "root"		// DB���� ���� ��
#define DB_PASS "1234"		// DB���� ���� ��ȣ
#define DB_NAME "userdb"	// DB �̸�

#define JOIN_SUCCESS_MSG "Join Success\n"
#define ERROR_JOIN_MSG "Join Error\n"
#define LOGIN_SUCCESS_MSG "Login Success\n"
#define ERROR_LOGIN_ID_MSG "ID Error\n"
#define ERROR_LOGIN_PW_MSG "PW Error\n"
#define ERROR_LOGIN_EXISTS_MSG "Exists Error\n"

#define GAME_NOT_READY "Wait for other player"
#define GAME_READY "GAME START"

enum class CLASS_STATE : unsigned __int64
{ //OB �� ������ ǥ��
  //���� �տ� 0 ���̸� 8���� 
  //���� �տ� 0X ���̸� 16����
	NO_STATE =   0x0000000000000000,
	INIT_STATE =  0x0100000000000000,
	LOGIN_STATE = 0x0200000000000000,
	CHAT_STATE =  0x0300000000000000,
	LOBBY_STATE = 0x0400000000000000,
	PLAYER_STATE = 0x0500000000000000,
};

enum IO_TYPE
{
	IO_RECV = 1,
	IO_SEND,
	IO_ACCEPT,
};

enum
{
	SOC_ERROR = -1,
	SOC_TRUE,
	SOC_FALSE,
};

enum UserInfo_Data
{
	ID = 0,
	PW,
	NAME
};

struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	LPVOID ptr;
	IO_TYPE type;
};

struct UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];
	char name[MSGSIZE];
	int number;
	bool login;
	UserInfo() 
	{
		memset(this, 0, sizeof(*this));
		login = false;
	}
	UserInfo(char* _id, char* _pw, char* _name, int numb)
	{
		strcpy(id, _id);
		strcpy(pw, _pw);
		strcpy(name, _name);
		number = numb;
		login = false;
	}
};







#define MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
               CLASSNAME(const CLASSNAME&){}                                \
               CLASSNAME& operator=(const CLASSNAME&);

// ��Ŭ�� ���� ���� ��ũ��
#define DECLARE_SINGLETONE(CLASSNAME)                                       \
        MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
			   static CLASSNAME* mPthis;									\
        public:                                                             \
               static CLASSNAME* Create();									\
			   static CLASSNAME* GetInstance();								\
               static void Destroy();										 

// �̱��� ���� ���� ��ũ��

#define IMPLEMENT_SINGLETON(CLASSNAME)                              \
               CLASSNAME* CLASSNAME::mPthis= NULL;					\
                                                                    \
               CLASSNAME* CLASSNAME::Create()						\
			   {													\
                       if(mPthis == NULL)							\
						{											\
                              mPthis=new CLASSNAME();				\
						}											\
                        return mPthis;								\
               }                                                    \
               CLASSNAME* CLASSNAME::GetInstance()					\
               {                                                    \
                    return mPthis;									\
               }													\
			   void CLASSNAME::Destroy()							\
               {													\
					if(mPthis) delete mPthis;						\
			   }																
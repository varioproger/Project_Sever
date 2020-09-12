#pragma once
#include "Global.h"

class CError 
{
private:
	static CError* pthis;

	CError() {}
	~CError() {}
public:
	static CError* GetInstance();
	static void Destory();

	void err_quit(const char* msg);
	void err_display(const char* msg);
	void err_display(const int code);
};
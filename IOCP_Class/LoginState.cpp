#include "LoginState.h"
#include "ClientSection.h"
#include"CProtocol.h"



CLoginState::CLoginState()
{
	mState = STATE::MENU_SELECT_STATE;
}

CLoginState::~CLoginState()
{

}

unsigned __int64 CLoginState::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void CLoginState::RecvProcess(CClientSection* ptr)
{
	LoginManager::CLock lock;
	unsigned __int64 full_code = ptr->GetProtocol();
	cout <<"CLoginState"<< hex << full_code << endl;
	//�� �κ� ���� Ŭ���̾�Ʈ�� main state Ȯ��
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::LOGIN_STATE,NULL,NULL);
	if (code_check == TRUE)
	{
		//�� �κ� ���� Ŭ���̾�Ʈ�� sub state Ȯ��
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL,(unsigned __int64)STATE::MENU_SELECT_STATE,NULL);
		if (code_check == TRUE)
		{
			//�� �κ� ���� Ŭ���̾�Ʈ�� protocol Ȯ��
			code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, NULL, (unsigned __int64)LoginManager::PROTOCOL::JOIN_OVERLAP);
			if (code_check == TRUE)
			{
				printf("OverLap Check\n");
				LoginManager::GetInstance()->OverLap(ptr);
				mState = STATE::SEND_RESULT_STATE;
				return;
			}

			code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL,NULL,(unsigned __int64)LoginManager::PROTOCOL::JOIN_INFO);
			if (code_check ==TRUE)
			{
				printf("JOIN_INFO123\n");
				LoginManager::GetInstance()->Join(ptr);
				mState = STATE::SEND_RESULT_STATE;
				return;
			}
			code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, NULL, (unsigned __int64)LoginManager::PROTOCOL::LOGIN_INFO);
			if (code_check == TRUE)
			{
				printf("LOGIN_INFO123\n");
				LoginManager::GetInstance()->Login(ptr);
				mState = STATE::SEND_RESULT_STATE;
				return;
			}
		}
	}
	else
	{
		return;
	}	
}

void CLoginState::SendProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case STATE::SEND_RESULT_STATE:
		mState = STATE::MENU_SELECT_STATE;
		break;
	}
}
#include "Lobby_State.h"

Lobby_State::Lobby_State()
{
	mState = STATE::NONE;
}

unsigned __int64 Lobby_State::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void Lobby_State::RecvProcess(CClientSection* ptr)
{
	unsigned __int64 full_code = ptr->GetProtocol();
	cout << "Lobby_State" << hex << full_code << endl;
	//�� �κ� ���� Ŭ���̾�Ʈ�� main state Ȯ��
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE, NULL, NULL);
	if (code_check == TRUE)
	{
		//�� �κ� ���� Ŭ���̾�Ʈ�� sub state Ȯ��
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::ENTER, NULL);
		if (code_check == TRUE)
		{
			printf("ENTER\n");
			mState = STATE::ENTER;
			Lobby_Manager::GetInstance()->Add(ptr);
			return;
		}
		//�� �κ� ���� Ŭ���̾�Ʈ�� sub state Ȯ��
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::LEAVE, NULL);
		if (code_check == TRUE)
		{
			printf("Remove\n");
			mState = STATE::LEAVE;
			Lobby_Manager::GetInstance()->Remove(ptr);
			return;
		}
	}
	else
	{
		return;
	}
}

void Lobby_State::SendProcess(CClientSection* ptr)
{
	return;
}

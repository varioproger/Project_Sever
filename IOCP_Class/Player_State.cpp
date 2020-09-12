#include "Player_State.h"
#include"Player_Manager.h"
Player_State::Player_State()
{
	mState = STATE::NONE;
}

unsigned __int64 Player_State::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void Player_State::RecvProcess(CClientSection* ptr)
{
	flag = true;
	unsigned __int64 full_code = ptr->GetProtocol();
	cout << "Lobby_State" << hex << full_code << endl;
	//�� �κ� ���� Ŭ���̾�Ʈ�� main state Ȯ��
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE, NULL, NULL);
	if (code_check == TRUE)
	{
		//�� �κ� Lobby Manager���� �ٷ� ����
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::ONLINE, NULL);
		if (code_check == TRUE)
		{
			printf("ONLINE\n");
			mState = STATE::ONLINE;
			Player_Manager::GetInstance()->Send_Into_Game(ptr);
			return;
		}
		//�� �κ� Lobby Manager���� �ٷ� ����
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::MOVEMENT, NULL);
		if (code_check == TRUE)
		{
			printf("MOVEMENT\n");
			mState = STATE::MOVEMENT;
			if (!Player_Manager::GetInstance()->Player_Movement(ptr))
			{
				flag = false;
			}
			return;
		}
		//�� �κ� ���� Ŭ���̾�Ʈ�� sub state Ȯ��
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::OFFLINE, NULL);
		if (code_check == TRUE)
		{
			printf("OFFLINE\n");
			mState = STATE::OFFLINE;
			Player_Manager::GetInstance()->Remove_From_Game(ptr);
			return;
		}
	}
	else
	{
		return;
	}
}

void Player_State::SendProcess(CClientSection* ptr)
{
	//player send�� ������ ��� �� ������ �ڵ� ����
	switch (mState)
	{
	case Player_State::STATE::NONE:
		break;
	case Player_State::STATE::ONLINE:
		break;
	case Player_State::STATE::MOVEMENT:
		break;
	case Player_State::STATE::OFFLINE:
		break;
	}
	return;
}

#include "ClientSection.h"
#include "InitState.h"
#include "ChatState.h"
#include"LoginState.h"
CInitState::CInitState()
{
	mState = INIT_STATE;
}

CInitState::~CInitState()
{

}

unsigned __int64 CInitState::Get_Sub_State()
{
	return INIT_STATE;
}

void CInitState::RecvProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case INIT_STATE:
		printf("Init\n");
		//�ٸ� class�� state�� ���� ���⼭ ���� �ʿ伺�� �ʱ� �۾��� ������ ���� �޶�����. \
		  Ȥ�� � �۾��� �����ϱ��� �ؾ��ϴ� �۾��� �ִٴ°� �����Ͽ� ���⼭ �ٸ� class�� state�� ���� ����. \
		  �ʱ� �۾��� �� �̷���� �ٷ� �� �� �ٷ� ����Ǿ� �ϴ� state�� ���⼭ ��������.
	 
		ptr->SetState(ptr->GetLoginState());
		break;
	}
}

void CInitState::SendProcess(CClientSection* ptr)
{

}
#include "Lobby_Manager.h"
IMPLEMENT_SINGLETON(Lobby_Manager)


Lobby_Manager::Lobby_Manager()
{
	lobby_list = nullptr;
	protocol = nullptr;
}

int Lobby_Manager::Packing(char* buf, int result, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
}

void Lobby_Manager::Begin()
{
	lobby_list = new vector<Lobby*>();
	Lobby* tmp = new Lobby();
	lobby_list->push_back(tmp);
}

void Lobby_Manager::End()
{
	lobby_list->clear();
	if (lobby_list != nullptr)
	{
		delete lobby_list;
	}
}

bool Lobby_Manager::Add(CClientSection* ptr)
{
	CLock lock;

	bool flag = false;
	char buf[BUFSIZE];
	int size = 0;
	unsigned __int64 full_code = NULL;
	ZeroMemory(buf, sizeof(buf));
	for (int i = 0; i < lobby_list->size(); i++)
	{
		if (lobby_list->at(i)->Add(ptr))//방인원수가 꽉 차지 않았을 경우
		{			
			protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);
			protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());

			if (!lobby_list->at(i)->Check_Full())//인원이 꽉차지 않았을 경우
			{
				printf("NOT FULL\n");
				size = Packing(buf, (int)RESULT::NOT_READY, GAME_NOT_READY);
				//이부분 수정		
				
			}
			else//인원 꽉 찼을 경우 보내고 바로 나온다.
			{
				printf("FULL\n");
				size = Packing(buf, (int)RESULT::READY, GAME_READY);	
				printf("lobby_list->at(i)->Get_User_List()->size() = %d\n", lobby_list->at(i)->Get_User_List()->size());
				for (int j = 0; j < lobby_list->at(i)->Get_User_List()->size(); j++)
				{
					lobby_list->at(i)->Get_User_List()->at(j)->PackingData(full_code, buf, size);
					if (!lobby_list->at(i)->Get_User_List()->at(j)->Send())
					{
						return false;
					}
				}
				return true;
			}
		}
		else//다 찼을 경우 방을 새로 생성해서 넣어준다.
		{
			Lobby* tmp = new Lobby();
			tmp->Add(ptr);
			lobby_list->push_back(tmp);
			protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);
			protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());

			printf("NOT FULL\n");
			size = Packing(buf, (int)RESULT::NOT_READY, GAME_NOT_READY);
			flag = true;
		}
	}
	ptr->PackingData(full_code, buf, size);
	if (!ptr->Send())
	{
		return false;
	}
	return flag;
}

bool Lobby_Manager::Remove(CClientSection* ptr)
{
	CLock lock;
	if (lobby_list->size() == 0)//처음에는 방이 없으므로.
	{
		printf("삭제할 방이 없습니다.\n");
		return false;
	}
	for (int i = 0; i < lobby_list->size(); i++)
	{
		if (lobby_list->at(i)->Remove(ptr))//제거할 인원이 확인 됐을때
		{
			return true;
		}
	}
	return false;
}



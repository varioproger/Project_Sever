#include "Lobby_Manager.h"
IMPLEMENT_SINGLETON(Lobby_Manager)


Lobby_Manager::Lobby_Manager()
{
	lobby_list = nullptr;
	protocol = nullptr;
}

int Lobby_Manager::Packing(char* buf,int lobby_num ,int result, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &lobby_num, sizeof(lobby_num));
	ptr = ptr + sizeof(lobby_num);
	size = size + sizeof(lobby_num);

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
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
		
		if (!lobby_list->at(i)->Check_Full())//���ο����� �� ���� �ʾ��� ���
		{			
			protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);
			protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());

			if (lobby_list->at(i)->Add(ptr))//�ο��� ������ �ʾ��� ���
			{
				printf("NOT FULL\n");
				size = Packing(buf, lobby_list->size(), (int)RESULT::NOT_READY, GAME_NOT_READY);
				//�̺κ� ����		
				
			}
			else//�ο� �� á�� ��� ������ �ٷ� ���´�.
			{
				printf("FULL\n");
				size = Packing(buf, (int)RESULT::READY, GAME_READY);	
				for (int j = 0; j < lobby_list->at(i)->Get_User_List()->size(); j++)
				{
					lobby_list->at(i)->Get_User_List()->at(j)->PackingData(full_code, buf, size);
					if (!lobby_list->at(i)->Get_User_List()->at(j)->Send())
					{
						return false;
					}
				}

				printf("���� ����\n");
				Lobby* tmp = new Lobby();
				lobby_list->push_back(tmp);

				return true;
			}
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
	if (lobby_list->size() == 0)//ó������ ���� �����Ƿ�.
	{
		printf("������ ���� �����ϴ�.\n");
		return false;
	}
	for (int i = 0; i < lobby_list->size(); i++)
	{
		if (lobby_list->at(i)->Remove(ptr))//������ �ο��� Ȯ�� ������
		{
			return true;
		}
	}
	return false;
}



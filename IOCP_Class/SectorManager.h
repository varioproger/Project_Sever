#pragma once
#include "Sector.h"

char Map[18][21] = {
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####O  ############",
	"#####  O############",
	"###  O@  ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# O             ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
};

class CSectorManager
{
private:
	vector< vector<CSector*>* >* vSector;
public:
	CSectorManager()
	{
		vSector = nullptr;
	}
	~CSectorManager()
	{
		if (vSector != nullptr) delete vSector;
	}

	void Begin()
	{
		vSector = new vector< vector<CSector*>* >();
		vSector->assign(18, new vector<CSector*>(21, nullptr));

		int id = 1;

		for (int i = 0; i < 18; i++)
		{
			vector<CSector*>* v = new vector<CSector*>();

			for (int j = 0; j < 21; j++)
			{
				CSector* sector = new CSector(id++);
				v->push_back(sector);
			}

			vSector->at(i) = v;
		}

		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						int check_x = x + j;
						int check_y = y + i;

						if (check_x >= 0 && check_x < 21 && check_y >= 0 && check_y < 18)
						{
							vSector->at(y)->at(x)->Insert_AlmostSector(vSector->at(check_y)->at(check_x));
						}
					}
				}
			}
		}

		BeginSector();
	}

	void End()
	{
		vSector->clear();
	}

	void BeginSector()
	{
		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				if (Map[y][x] == '@' || Map[y][x] == 'O')
				{
					vSector->at(y)->at(x)->OnEnter(Map[y][x]);

					for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
					{
						CSector* sector = *data;
						// sector->Send();

						printf("%d Send\n", sector->GetID());
					}
					printf("\n");
				}
			}
		}
	}

	void UpdateSector(/*CClientSection* ptr*/)
	{
		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				CSector* sector = vSector->at(y)->at(x);

				if (sector->GetChar() == '@')
				{
					for (auto data = sector->GetAlmostSectorList()->begin(); data != sector->GetAlmostSectorList()->end(); data++)
					{
						CSector* almost = *data;

						if (almost->GetChar() == '@')
						{
							// almost->Send();
							printf("Client Sector ID : %d\n", almost->GetID());
						}

						if (almost->GetChar() == 'O')
						{
							printf("Object Sector ID : %d\n", almost->GetID());
						}
					}

					printf("\n");
				}
			}
		}

		/*
		int x , y;

		// ID �� ���� ã��
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (vSector->at(i)->at(j)->GetID() == ptr->GetID())
				{
					x = i;
					y = j;
					break;
				}
			}
		}

		// �÷��̾ �̵� ���� ���
		if (GetProtocol() == MOVE)
		{
			MoveSector(ptr, x, y);
		}

		// ���� ������ Ŭ���̾�Ʈ ����Ʈ���� ������ ����
		for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
		{
			CSector* sector = *data;
			// sector->Send();

			printf("%d Send\n", sector->GetID());
		}
		*/
	}

	void MoveSector(/*CClientSection* ptr, int& _x, int& _y*/)
	{
		/*
		// ���� �÷��̾ �ڽ��� ���� ������ ��Ż�ϰ� �ٸ� ���� ������ ���� �� Ȯ��
		if (. . .)
		{
			int pre_id = vSector->at(_y)->at(_x)->GetID();

			int x = _x;
			int y = _y;

			// ��� �������� �̵��ߴ� �� Ȯ�� ��, x,y ������ ��ġ ����
			if ()
			else if ()
			. . .

			vSector->at(_y)->at(_x)->OnExit(ptr);
			vSector->at(y)->at(x)->OnEnter(ptr);

			// ���� ������ �α� ���͵鿡�� ������ ����
			for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
			{
				CSector* sector = *data;
				// sector->Send();

				printf("%d Send\n", sector->GetID());
			}

			_x = x;
			_y = y;
		}
		else
			return;
		*/
	}
};

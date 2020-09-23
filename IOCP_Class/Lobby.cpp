#include "Lobby.h"

Lobby::Lobby()
{
    full = false;
    user_list = new vector< CClientSection*>();
}

bool Lobby::Add(CClientSection* ptr)
{
    user_list->push_back(ptr);
    if (user_list->size() % 2 == 0)
    {
        printf("2일때\n");
        full = true;
        return false;
    }
    else
    {
        printf("1일때\n");
        full = false;
        return true;
    }  
}

bool Lobby::Remove(CClientSection* ptr)
{
    for (auto it = user_list->begin(); it != user_list->end(); ++it)
    {
        if (*it == ptr)
        {
            user_list->erase(it);
            return true;
        }
    }
    return false;
}

int Lobby::Get_User_Count()
{
    return user_list->size();
}

bool Lobby::Check_Full()
{
    return this->full;
}

vector<CClientSection*>* Lobby::Get_User_List()
{
    return this->user_list;
}

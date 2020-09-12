#pragma once
#include "ClientSection.h"
#include<vector>
class Lobby
{
private:
    vector< CClientSection*>* user_list;
    bool full;
public:
    Lobby();
    ~Lobby() = default;
    bool Add(CClientSection* ptr);
    bool Remove(CClientSection* ptr);
    int Get_User_Count();
    bool Check_Full();
    vector< CClientSection*>* Get_User_List();
};


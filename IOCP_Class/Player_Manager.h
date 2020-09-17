#pragma once
#include"Global.h"
#include"Player.h"
#include"CProtocol.h"
#include"ClientSection.h"

class Player_Manager : public CMultiThreadSyns<Player_Manager>
{
private:
    DECLARE_SINGLETONE(Player_Manager)
    Player_Manager();
    ~Player_Manager() = default;
	vector< CClientSection*>* player_list;
    CProtocol* protocol;
    Vector3D vect_pos[4] =
    {
        {-8.9,-3.56,-56.56},
         {-11,-3.56,-56.56},
        {5,1,1},
        {7,1,1}
    };//기본 시작 좌표
    enum class PROTOCOL : unsigned __int64
    {
        SPAWN = 0x0000010000000000,
        POSITION = 0x0000020000000000,
        ROTATION = 0x0000030000000000,
        CHARACTERFORWARD = 0x0000040000000000
    };
public:
    void Begin();
    void End();
    int Packing(char* buf, int number, Player* player);
    int Packing(char* buf, int serial, Vector3D* position);
    int Packing(char* buf, int serial, Quaternion* rotation);
    int UnPacking(const char* buf, float& rotVertical, Vector3D& m_CamForward, float& rotHorizontal, Vector3D& Cam_right);
    void UnPacking(const char* buf, Player* player);
    void Send_Into_Game(CClientSection* ptr);
    bool Player_Movement(CClientSection* ptr);
    void Remove_From_Game(CClientSection* ptr);
    void Character_Forward(CClientSection* ptr);
};


#include "Player_Manager.h"

IMPLEMENT_SINGLETON(Player_Manager)

Player_Manager::Player_Manager()
{
    player_list = nullptr;
    protocol = nullptr;
}

void Player_Manager::Begin()
{
    player_list = new vector< CClientSection*>();
    protocol = CProtocol::GetInstance();
}

void Player_Manager::End()
{
    player_list->clear();
    if (player_list != nullptr)
    {
        delete player_list;
    }
}

int Player_Manager::Packing(char* buf, int number ,Player* player)
{
    char* ptr = buf;
    unsigned long serial = player->Get_Serial_num();//시리얼 넘버
    int size = 0;

    //id
    memcpy(ptr, &number, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);
    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    //vector 3 x
    memcpy(ptr, &player->Get_Position()->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    //vector 3 y
    memcpy(ptr, &player->Get_Position()->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vector 3 z
    memcpy(ptr, &player->Get_Position()->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion w
    memcpy(ptr, &player->Get_Rotation()->w, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion x
    memcpy(ptr, &player->Get_Rotation()->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vQuaternion y
    memcpy(ptr, &player->Get_Rotation()->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion z
    memcpy(ptr, &player->Get_Rotation()->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}
int Player_Manager::Packing(char* buf, int serial, Vector3D* position)
{
    char* ptr = buf;
    int size = 0;

    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);
    //vector 3 x
    memcpy(ptr, &position->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    //vector 3 y
    memcpy(ptr, &position->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vector 3 z
    memcpy(ptr, &position->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}
int Player_Manager::Packing(char* buf, int serial,  Quaternion* rotation)
{
    char* ptr = buf;
    int size = 0;

    //serial
    memcpy(ptr, &serial, sizeof(int));
    ptr += sizeof(int);
    size += sizeof(int);

    //Quaternion w
    memcpy(ptr, &rotation->w, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion x
    memcpy(ptr, &rotation->x, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //vQuaternion y
    memcpy(ptr, &rotation->y, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);
    //Quaternion z
    memcpy(ptr, &rotation->z, sizeof(float));
    ptr += sizeof(float);
    size += sizeof(float);

    return size;
}
int Player_Manager::UnPacking(const char* buf, float& rotVertical, Vector3D& m_CamForward, float& rotHorizontal, Vector3D& Cam_right)
{
    const char* ptr = buf;

    memcpy(&rotVertical, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&m_CamForward.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&m_CamForward.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&m_CamForward.z, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&rotHorizontal, ptr, sizeof(float));
    ptr += sizeof(float);
    //
    memcpy(&Cam_right.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&Cam_right.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&Cam_right.z, ptr, sizeof(float));
    ptr += sizeof(float);


    return 0;
}
void Player_Manager::UnPacking(const char* buf, Player* player)
{
    int bool_size = 0;
    const char* ptr = buf;
    Quaternion rot;
    memcpy(&bool_size, ptr, sizeof(int));
    ptr += sizeof(int);
    player->input = new bool[bool_size];

   // printf("bool_size = %d\n", bool_size);
    for (int i = 0; i < bool_size; i++)
    {
        memcpy(&player->input[i], ptr, sizeof(bool));
        // printf("player->input[%d] = %d\n", i, player->input[i]);
        ptr += sizeof(bool);
    }
    memcpy(&rot.w, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.x, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.y, ptr, sizeof(float));
    ptr += sizeof(float);
    memcpy(&rot.z, ptr, sizeof(float));

    printf("W = %f\n", rot.w);
    printf("X = %f\n", rot.x);
    printf("Y = %f\n", rot.y);
    printf("Z = %f\n", rot.z);
    player->Set_Rotation(rot);


}
void Player_Manager::Send_Into_Game(CClientSection* ptr)
{
    CLock lock;
    int size = 0;
    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    printf("Send_Into_Game\n");
    //create_Player obj
    ptr->player_online(new Player());
    player_list->push_back(ptr);

    ptr->Getplayer()->Set_Serial_num(player_list->size());
    ptr->Getplayer()->Set_Position(vect_pos[player_list->size() - 1]);
    ptr->Getplayer()->Set_Rotation(new Quaternion());

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::SPAWN);

    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            if (player_list->at(i)->Getplayer()->Get_Serial_num() != ptr->Getplayer()->Get_Serial_num())//나를 제외한
            {
                printf("1\n");
                ZeroMemory(buf, sizeof(buf));
                size = Packing(buf, player_list->at(i)->GetUser()->number,player_list->at(i)->Getplayer());//다른 사람들의 정보를 패킹
                ptr->PackingData(full_code, buf, size);
                if (!ptr->Send())//나한테 보낸다.
                {
                    printf("Player_Spawn_Send_Error_1\n");
                    return;
                }
            }
        }
    }
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            printf("2\n");
            ZeroMemory(buf, sizeof(buf));
            size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer());//나의 대한 정보를 패킹
            player_list->at(i)->PackingData(full_code, buf, size);
            if (!player_list->at(i)->Send())//전체에게 보낸다.
            {
                printf("Player_Spawn_Send_Error_2\n");
                return;
            }
        }
    }
}
bool Player_Manager::Player_Movement(CClientSection* ptr)
{
    CLock lock;
    printf("Player_Movement\n");
    int size = 0;
    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    UnPacking(ptr->UnPackData(), ptr->Getplayer());
    for (int i = 0; i < 4; i++)
    {
        printf("player->input[%d] = %d\n", i, ptr->Getplayer()->input[i]);
    }
    printf("W = %f\n", ptr->Getplayer()->Get_Rotation()->w);
    printf("X = %f\n", ptr->Getplayer()->Get_Rotation()->x);
    printf("Y = %f\n", ptr->Getplayer()->Get_Rotation()->y);
    printf("Z = %f\n", ptr->Getplayer()->Get_Rotation()->z);
    ptr->Getplayer()->Update_Character_Vector3D(ptr->Getplayer()->input);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::POSITION);

    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            ZeroMemory(buf, sizeof(buf));

            size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer()->Get_Position());//내 정보를 패킹
            player_list->at(i)->PackingData(full_code, buf, size);
            if (!player_list->at(i)->Send())//전체에게 보낸다.
            {
                printf("Player_Movement_Send_Error_1\n");
                return false;
            }
        }
    }
    full_code = NULL;
    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::ROTATION);
    cout << "ROTATION" << hex << full_code << endl;
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            if (player_list->at(i)->Getplayer()->Get_Serial_num() != ptr->Getplayer()->Get_Serial_num())//나를 제외한
            {
                ZeroMemory(buf, sizeof(buf));
                size = Packing(buf, ptr->GetUser()->number, ptr->Getplayer()->Get_Rotation());//다른 사람들의 정보를 패킹
                player_list->at(i)->PackingData(full_code, buf, size);
                if (!player_list->at(i)->Send())//전체에게 보낸다.
                {
                    printf("Player_Movement_Send_Error_2\n");
                    return false;
                }
            }
        }
    }

}
void Player_Manager::Remove_From_Game(CClientSection* ptr)
{
    CLock lock;
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            if (player_list->at(i)->Getplayer()->Get_Serial_num() == ptr->Getplayer()->Get_Serial_num())//나를 제외한
            {
                ptr->player_offline(ptr->Getplayer());
                player_list->erase(player_list->begin() + i);
            }
        }
    }
}

void Player_Manager::Character_Forward(CClientSection* ptr)
{
    float rotVertical;
    Vector3D m_CamForward;
    float rotHorizontal;
    Vector3D Cam_right;

    unsigned __int64 full_code = NULL;
    char buf[BUFSIZE];
    ZeroMemory(buf, sizeof(buf));

    ZeroMemory(&m_CamForward, sizeof(m_CamForward));
    ZeroMemory(&Cam_right, sizeof(Cam_right));

    UnPacking(ptr->UnPackData(), rotVertical,m_CamForward,rotHorizontal,Cam_right);

    protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE);
    protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
    protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::CHARACTERFORWARD);


    Vector3D Rotatedirection = ptr->Getplayer()->Character_Forward(rotVertical, m_CamForward, rotHorizontal, Cam_right);
    int size = Packing(buf, ptr->GetUser()->number, &Rotatedirection);//다른 사람들의 정보를 패킹
    for (int i = 0; i < player_list->size(); i++)
    {
        if (player_list->at(i) != nullptr)
        {
            ZeroMemory(buf, sizeof(buf));
            player_list->at(i)->PackingData(full_code, buf, size);
            if (!player_list->at(i)->Send())//전체에게 보낸다.
            {
                printf("Player_Movement_Send_Error_1\n");
                return;
            }
        }
    }
}

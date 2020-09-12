#pragma once
#include"Vector3D.h"
#include"Quaternion.h"
//#include"ClientSection.h"

class Player
{
public:
    Player();
    ~Player() = default;
    void Update_Character_Vector3D(bool* _input);
    Vector3D* Get_Position();
    void Set_Position(Vector3D _pos);
    Quaternion* Get_Rotation();
    void Set_Rotation(Quaternion* _rot);
    void Set_Rotation(Quaternion _rot);
    int  Get_Serial_num();
    void Set_Serial_num(int _num);

public:
    bool* input;
    float moveSpeed;

private:
    int serial_num;
	Vector3D position;
	Quaternion rotation;
    void Move(Vector3D _inputDirection);
    Vector3D Character_Forward(float rotVertical, Vector3D m_CamForward , float rotHorizontal, Vector3D Cam_right);

};


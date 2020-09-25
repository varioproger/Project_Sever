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
    Vector3D Character_Forward(float rotVertical, Vector3D m_CamForward, float rotHorizontal, Vector3D Cam_right);
    void Character_Rotation(float _mouseVertical, float _mouseHorizontal)
    {
        verticalRotation += _mouseVertical * sensitivity * 0.0333f;
        verticalRotation = Mathf_Clamp(verticalRotation, -clampAngle, clampAngle);
        //https://hvstudy.tistory.com/7
        horizontalRotation += _mouseHorizontal * sensitivity * 0.0333f;

        /*
        ¿ø·¡ ½Ä.
        verticalRotation += _mouseVertical * sensitivity * Time.deltaTime;
        horizontalRotation += _mouseHorizontal * sensitivity * Time.deltaTime;

        verticalRotation = Mathf.Clamp(verticalRotation, -clampAngle, clampAngle);
        */

    }
	float Get_verticalRotation()
	{
        return this->verticalRotation;
	}
	float Get_horizontalRotation()
	{
        return this->horizontalRotation;
	}
public:
    bool* input;
	float moveSpeed;
	float sensitivity = 100.0f;
	float clampAngle = 85.0f;
	float verticalRotation;
	float horizontalRotation;
private:
    int serial_num;
	Vector3D position;
	Quaternion rotation;
    void Move(Vector3D _inputDirection);
    float Mathf_Clamp(float input, float min, float max);

};


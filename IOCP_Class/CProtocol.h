#pragma once
#include"Global.h"

class CProtocol
{
private:
    //1$3 = ��ġ
    //2&6 = ��ġ
    //2&7 = ��ġ 
    //& ������ �ϱ� ������ 1�� �ƴѰ��� �� 0���� ó�� �ϱ� ����.
    //1, 3, 7, f, 3f, 7f, ff
    DECLARE_SINGLETONE(CProtocol);
    CProtocol() = default;
    ~CProtocol() = default;
    enum class FULL_CODE :unsigned __int64
    {
        MAIN = 0xff00000000000000,
        SUB =  0x00ff000000000000,
        PROTOCOL = 0x0000ff0000000000
    };
public:
    void ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input);
    bool ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol);
};



#include "CProtocol.h"
IMPLEMENT_SINGLETON(CProtocol)

void CProtocol::ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input)
{
    Full_Code = Full_Code | input;
}

bool CProtocol::ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol)
{
    unsigned __int64 temp = 0;
    int variable_count = 0;//�Է¹��� �Ű����� ����
    if (main != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::MAIN;
        variable_count++;
    }
    if (sub != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::SUB;
        variable_count++;
    }
    if (protocol != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::PROTOCOL;
        variable_count++;
    }
    if (variable_count == 2)
    {
        printf("�Է� ���� �Ű������� ���� �ʰ��ϼ̽��ϴ�.");
        return false;
    }
    if (temp == main|| temp == sub || temp == protocol)
    {
        printf("��ġ\n");
        return true;
    }
    else
    {
        printf("����ġ\n");
        return false;
    }

}

#include "stdafx.h"
#include "..\SP2Packet.h"
#include ".\iochannelingnodeparent.h"


ioChannelingNodeParent::ioChannelingNodeParent(void)
{
}

ioChannelingNodeParent::~ioChannelingNodeParent(void)
{
}

bool ioChannelingNodeParent::IsBillingTestID( const ioHashString &rsPublicID )
{
	// �Ʒ� "�ݹ�", "�ݹ���" ���̵� ������ �ݵ�� ���������� �������ּ���.
	// �ݹ� : �Ϲ�, �ݹ��� : ������, �Ǹ��ܺ��̺� : ���̹� (��쾾ID)
	if( rsPublicID == "�ݹ�" || rsPublicID == "�ݹ���" || rsPublicID == "�Ǹ��ܺ��̺�" ) //Except Extracting Hangeul
		return true;

	return false;
}

const char * ioChannelingNodeParent::GetBillingErrorMent(ioHashString &sServerError)
{
	return "Billing Error : ";
}
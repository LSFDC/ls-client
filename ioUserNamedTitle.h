#pragma once

#include "NamedTitleEnumType.h"

class ioUserNamedTitle
{
protected:
	NamedTitleSlotList m_NamedTitleSlotList;

public:
	//����ȭ
	void ApplyNamedTitleSlotData( SP2Packet &rkPacket );

public:
	//���� �߰�
	bool AddNamedTitleSlotData( const NamedTitleSlot &sNewSlot );
	//���� ����
	bool DeleteNamedTitleSlotData( DWORD dwCode );
	//���� ����
	bool SetNamedTitleSlot( const NamedTitleSlot &sNamedTitleSlot );

public:
	//���� ����
	void SetNamedTitleEquip( DWORD dwCode );

	//��� ����
	void SetNamedTitleGrade( DWORD dwCode, int nLevel );
	
	//�����̾� ����
	void SetNamedTitlePremium( DWORD dwCode );

public:
	void ClearNamedTitleEquip();
	void ClearNamedTitleNew();

public:
	//���� ���� ���� ���
	bool GetNamedTitleSlotEquip( NamedTitleSlot &sNamedTitleSlot );
	//���� ���(���� �ڵ�)
	bool GetNamedTitleSlot( DWORD dwCode, NamedTitleSlot &sNamedTitleSlot );
	//���� ���(���� �迭)
	bool GetNamedTitleSlotArray( int nArray, NamedTitleSlot &sNamedTitleSlot );

public:
	//���� ���� ���
	int GetNamedTitleSlotCnt();
	//���ο� Īȣ
	bool IsNewNamedTitle();
	//�����̾� ���� Īȣ
	bool IsEnablePremiumNamedTitle();

public:
	ioUserNamedTitle();
	virtual ~ioUserNamedTitle();
};


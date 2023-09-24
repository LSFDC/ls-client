#pragma once
#include "iobuff.h"
class ioCreateDummyByAttackedBuff :
	public ioBuff
{
protected:
	enum Buff_Sync
	{
		BS_CREATE_DUMMY,
		BS_END,
	};
protected:
	vDummyCharLoadInfoList m_DummyList;
	ioHashStringVec m_vDestroyDummyList;

	//�ǰ� ����
	int				m_iMaxAttackedCount;		//���� ��ȯ�� ���� �ʿ��� �ǰ� Ƚ��
	int				m_iCurAttackedCount;		//���� �ǰ� Ƚ��
	
	//���� ���� ����
	int				m_iMaxCreateDummyCount;		//������ �� �ִ� �ִ� ���� ��
	int				m_iCurCreatedDummyCount;	//������� ������ ������ ��
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual void ApplyExtraBuffInfo( SP2Packet &rkPacket );
	virtual void ProcessBuff( float fTimePerSec );
public:
	void OwnerCharApplyWeapon();
	void CreateDummyChar( ioBaseChar *pOwner );
public:
	ioCreateDummyByAttackedBuff(void);
	ioCreateDummyByAttackedBuff( const ioCreateDummyByAttackedBuff &rhs );
	virtual ~ioCreateDummyByAttackedBuff(void);
};

inline ioCreateDummyByAttackedBuff* ToCreateDummyByAttackedBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CREATE_DUMMY_BY_ATTACKED_BUFF )
		return NULL;

	return dynamic_cast< ioCreateDummyByAttackedBuff* >( pBuff );
}
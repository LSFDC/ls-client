//////////////////////////////////////////////////////////////////////////
//�ۼ� : ����ö
//���� : �Ƿ� ���� ��ų�߿� ��ų�� ����Ͽ� weapon�� �߻��Ͽ� Ÿ���� ���ݽ� 
//       ��ų�������� �����Ű� �����Ѵٸ� ������ �ϰ� �ٽ� �����ڿ��� �߻�ü�� �����Ͽ�
//	     �����ڿ� ������ �������� ȸ�������ش�.         
//	     ���� ���� ��ų�������� �������� ������ �����ڿ��� �ٸ� �߻�ü�� �����Ѵ�.
//       ( �߻�ü�� buff create����� �ִ�. �� ���⺰�� ������ �ȴ�. )
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioSkillGaugeCheckBuff : public ioBuff
{
	int m_nSuccessWeapon;
	int m_nFailWeapon;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

	virtual void ApplyExtraBuffInfo(  SP2Packet &rkPacket  );

protected:
	void CreateSuccusWeapon( bool bSend );
	void CreateFailWeapon( bool bSend );

public:
	ioSkillGaugeCheckBuff();
	ioSkillGaugeCheckBuff( const ioSkillGaugeCheckBuff &rhs );
	virtual ~ioSkillGaugeCheckBuff();
};


inline ioSkillGaugeCheckBuff* ToSkillGaugeCheckBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_SKILL_GAUGE_CHECK )
		return NULL;

	return dynamic_cast< ioSkillGaugeCheckBuff* >( pBuff );
}



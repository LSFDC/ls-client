#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioDummyAttachBuff : public ioBuff
{
public:
	enum BuffState
	{
		BS_CreateAreaWeapon,
		BS_CreateDummy,
		BS_CreateDummyWeapon,
		BS_PlayHornSound,
		BS_PlayAttackSound,
	};

	//���� ����
	struct DummyInfo 
	{
		//�ε���
		int nIndex;
		//��ġ
		D3DXVECTOR3 vPos;
	};

	typedef std::vector<DummyInfo> vDummyInfoList;

protected:	
	//���� ������
	vDummyCharLoadInfoList m_DummyCharList;
	//���� ���� �̸�
	ioHashStringVec m_vDestroyDummyCharList;
	//���� ����
	vDummyInfoList m_DummyInfoList;
	//Ȱ�� �ð�
	DWORD m_dwActiveDuration;
	//Ȯ�� �ð�
	DWORD m_dwCheckDuration;
	//Ȱ�� �ð� ��� ����
	bool m_bSetActiveDuration;
	//���� ���� ������ �Ҹ�
	float m_fAreaWeaponUseGauge;
	//���� ���� �ð�
	DWORD m_dwAreaWeaponTick;
	//���� ��� �ð� Ȯ��
	DWORD m_dwCheckAreaWeapon;
	//������ ���� �̸�
	ioHashString m_szAreaWeaponName;	
	//���̰� ���� �ٴ� ���ΰ�
	bool m_bSetDummyAttach;
	//������ ���� ���� ����
	bool m_bStartGauge;
	//Area ���� ���
	bool m_bSetAreaWeapon;
	//���� ���� ���
	bool m_bSetDummyWeapon;
	//���� ���� ������ �Ҹ�
	float m_fDummyWeaponUseGauge;
	//���� ���� �ð�
	DWORD m_dwDummyWeaponTick;
	//���� ��� �ð� Ȯ��
	DWORD m_dwCheckDummyWeapon;
	//���� �̸�
	ioHashString m_szHornSoundName;
	//���ݻ��� �̸�
	ioHashString m_szAttackSoundName;
	//���� ��� �ð�
	DWORD m_dwDelayHornSoundTime;
	//���� Ȯ�� �ð�
	DWORD m_dwCheckHornSoundTime;
	//���� ��� �ð�
	DWORD m_dwDelayAttackSoundTime;
	//���� Ȯ�� �ð�
	DWORD m_dwCheckAttackSoundTime;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();
	
public:
	void ApplyExtraBuffInfo( SP2Packet &rkPacket );
	
protected:
	//Ű�Է�
	void ProcessKeyInput();
	//���� ����
	void CreateAreaWeapon();
	//�Ҹ� ���
	void PlayHornSound();
	void PlayAttackSound();
	//��ų������ ����
	void SetSkillGauge( float fSkillGauge );
	//���� ĳ���� ����
	void CreateDummyChar();
	//���� Ȯ��
	bool CheckDummy();
	//���� ����
	void ProcessDummyAttach();
	//���� ���� �ֱ�
	void AddDummyInfo(int nIndex);
	//������ ���� ����
	void CreateDummyWeapon();

public:
	ioDummyAttachBuff();
	ioDummyAttachBuff( const ioDummyAttachBuff &rhs );
	virtual ~ioDummyAttachBuff();
};

inline ioDummyAttachBuff* ToDummyAttachBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_DUMMY_ATTACH )
		return NULL;

	return dynamic_cast< ioDummyAttachBuff* >( pBuff );
}
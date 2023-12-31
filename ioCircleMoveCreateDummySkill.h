#pragma once

#include "ioNormalSkill.h"
#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;
class SP2Packet;

class ioCircleMoveCreateDummySkill : public ioNormalSkill
{
public:
	enum SkillState
	{
		SS_NONE,
		SS_LOOP,
		SS_ACTION,
		SS_END,
		SS_WAIT,
		SS_CREATE_DUMMY,
	};
	
protected:
	//마법진 설정
	float m_fCircleMinRange;
	float m_fCircleMaxRange;
	float m_fCircleMoveSpeed;
	float m_fCircleHeightGap;
	float m_fCircleOffSet;

	//이펙트 설정
	ioHashString m_ReadyCircle;
	ioHashString m_szReadyCircleDropZone;
	ioHashString m_szCurReadyCircle;
	ioHashString m_RedEffect;
	ioHashString m_BlueEffect;
	
	
	//무적 시간 설정
	DWORD m_dwSkillProtectTime;
	//공격 무기 설정
	ioHashString m_szFirstDummyName;
	ioHashString m_szSecondDummyName;
	//루프 설정
	ioHashString m_szLoopAni;
	DWORD m_dwLoopDuration;
	//첫번째 무기 생성 위치 이펙트
	ioHashString m_szFirstWeaponEffect;
	D3DXVECTOR3 m_vFirstWeaponEffectOffset;
	D3DXVECTOR3 m_vSecondWeaponEffectOffset;

	float m_fReduceSkillGauge;

	ioHashStringVec m_OwnerBuffList;
			
protected:
	//스킬 상태
	SkillState m_SkillState;
	//현재 스킬 보호 시간
	DWORD m_dwCurSkillProtectTime;
	//루프 시간 확인
	DWORD m_dwCheckLoopTime;
	//마법진 위치
	D3DXVECTOR3 m_vCirclePos;
	//마법진 위치
	D3DXVECTOR3 m_vFirstDummyPos;
	//공격 시간
	DWORD m_dwFireStartTime;

	bool m_bCreateDummy;
	DWORD m_dwFirstEffectID;

	//magic circle을 따라다니는 map effect
	ioHashString m_szCircleEffectName;
	DWORD m_dwCircleMapEffectID;
	float m_fCircleMapEffectAngle;
	D3DXQUATERNION m_qtMapEffectRot;
	//

	bool			m_bCreateDropZoneDummy;

	bool			m_bCheckUseOwnerBuff;
public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioSkill* Clone();

public:
	virtual bool OnSkillStart( ioBaseChar *pChar );
	virtual void OnSkillEnd( ioBaseChar *pChar );	
	virtual void OnProcessState( ioBaseChar *pChar, ioPlayStage *pStage );

	virtual void ApplyExtraSkillInfo( ioBaseChar *pChar, ioPlayStage *pStage, SP2Packet &rkPacket );
	virtual void ApplyAttributeToWeapon( ioWeapon *pWeapon, ioPlayStage *pStage );

	virtual bool CheckUseItemMesh();
	virtual bool IsProtected( int iDefenseBreakType ) const;

	virtual bool IsEndState() const;
	virtual bool IsSkillMotionEnd( DWORD dwActionStop, ioBaseChar* pOwner ) const;

	virtual bool CheckUseBuff( ioBaseChar *pChar );

protected:
	virtual void SetNormalSkill( ioBaseChar *pChar );
	
protected:
	//성장 범위 얻기
	float GetGrowthCircleRange( ioBaseChar *pChar );
	//마법진 생성
	void CreateMagicCircle( ioBaseChar *pChar );
	//마법진 업데이트
	void UpdateMagicCircle( ioBaseChar *pChar, ioPlayStage *pStage );
	//마법진 위치얻기
	void GetMagicCirclePos( ioBaseChar *pChar, bool bVisible );
	//마법진 이펙트변경(맵 높이에 따른)
	void CheckCircleByHeight( ioBaseChar *pChar, ioPlayStage *pStage );

protected:
	//루프 상태로 전환
	void SetLoopState( ioBaseChar *pChar );
	//공격 상태로 전환
	void SetActionState( ioBaseChar *pChar, ioPlayStage *pStage );
	//종료 상태 설정
	void SetEndState( ioBaseChar *pChar, ioPlayStage *pStage );

protected:
	//키입력 확인
	void CheckInputKey( ioBaseChar *pChar, ioPlayStage *pStage );
	//마법진 방향으로 회전
	void CheckRotate( ioBaseChar *pChar, ioPlayStage *pStage );

protected:	
	void CreateFirstWeaponEffect( ioBaseChar *pChar, ioPlayStage *pStage );
	void CreateSecondWeaponEffect( ioBaseChar *pChar, ioPlayStage *pStage  );
	void CreateDummies( ioBaseChar *pChar, ioPlayStage *pStage );
	void CreateDummy( ioBaseChar *pChar, ioPlayStage *pStage, ioHashString szDummyName, D3DXVECTOR3 vPos, int nIndex );
	void SetOwnerBuff( ioBaseChar *pChar );

	//magic circle을 따라다니는 map effect 생성
	void CreateMapEffect( ioBaseChar *pChar, ioPlayStage *pStage );
	//magic circle을 따라다니는 map effect 업데이트
	void UpdateCircleMapEffect( ioBaseChar *pChar, ioPlayStage *pStage );
	//a,s키로 회전이 된 이펙트 값을 더미에 세팅해준다.
	void SetDummyByMapEffect( ioBaseChar *pChar, ioPlayStage *pStage, ioDummyChar *pDummy );
	//a,s 키를 누를 때마다 해당 이펙트를 fAngle 만큼 회전시켜준다.
	void RotateCircleMapEffect( ioBaseChar *pChar, ioPlayStage *pStage, float fAngle );
	//

public:
	ioCircleMoveCreateDummySkill();
	ioCircleMoveCreateDummySkill( const ioCircleMoveCreateDummySkill &rhs );
	virtual ~ioCircleMoveCreateDummySkill();
};

inline ioCircleMoveCreateDummySkill* ToCircleMoveCreateWeaponSkill( ioSkill *pSkill )
{
	ioNormalSkill *pNormal = ToNormalSkill( pSkill );
	if( !pNormal || pNormal->GetSubSkillType() != NST_CIRCLE_MOVE_CREATE_DUMMY )
		return NULL;

	return static_cast< ioCircleMoveCreateDummySkill* >( pSkill );
}


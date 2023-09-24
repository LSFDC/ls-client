#pragma once

#include "ioWeapon.h"
#include "IORandom.h"

class ioEffect;
class ioUroborusGuideArrow;

class ioUroborusWeapon3 : public ioWeapon
{
public:
	enum UroborusState
	{
		US_MOVE,
		//US_WAIT,
		US_TARGET_MOVE,
		US_END,
	};

protected:
	UroborusState m_UroborusState;

	ioEffect *m_pFireMesh;
	ioUroborusGuideArrow *m_pGuideArrow;

	ioOrientBox m_LocalAttackBox;
	D3DXVECTOR3 m_vMoveDir;
	D3DXVECTOR3 m_vPrePos;

	ioHashString m_RedTeamEffect;
	ioHashString m_BlueTeamEffect;

	bool m_bFirst;

	float m_fCurMoveSpeed;
	float m_fCurMoveRange;

	D3DXVECTOR3 m_vTargetMovePos;
	CEncrypt<float> m_fTargetMoveSpeed;
	CEncrypt<float> m_fTargetMaxRange;
	CEncrypt<float> m_fTargetCurRange;

	ioHashString m_TargetMoveEffect;

	//Ÿ���� �ִٸ�
	bool m_bTraceTarget;
	ioHashString m_TraceTargetName;

	//��ų���� ������ ����
	bool m_bSkillWeapon;

	//�浹ü�� �����ϸ� ���Ŀ��� ���Ѵ�.
	bool m_bOneTimeColCheck;
	bool m_bColOccur;


	//Į���� �߰�
	D3DXVECTOR3		m_StartPos;
	bool			m_bUseEffectStartPosMode;
	bool			m_bUseStartReturnPos;
	bool			m_bUseRotateWeapon;
	float			m_fRotateAngle;
	float			m_fRotateAngle2;

	bool			m_bUseReturnWeapon;//�巡�� ���̴�ASó�� �����ð� �ڿ� ���� �´�.
	float			m_fReturnMaxLength;//�ش� �Ÿ���ŭ �̵��� �Ŀ� �ٽ� �ڷ� ���� �´�.

	bool			m_bUseDelayStartTime;
	DWORD			m_dwDelayStartTime;
	bool m_bNoItemCheck;
	bool m_bNoAllCheck;

	bool m_bCheckLand;//�������� �Ʒ��̸� �ȵȴ�.

	//ü�θ����Ǹ��ڶ� �߰�
	//������ �������� ��ȯ
	bool			m_bFixedDir;
public:
	static ActionStopType m_ActionStopType;
	static ActionStopType m_DieActionStopType;

protected:
	bool SetFireMesh( const ioHashString &szFileName );

	void OnFloating( float fTimePerSec, ioPlayStage *pStage );
	void OnTargetMove( float fTimePerSec, ioPlayStage *pStage );

	float GetMoveSpeed();
	bool CheckOwnerState();

	void ProcessGuidArrow();

	void SetGuidArrowPosMode();
	bool CheckLandHeight( ioPlayStage *pStage );
	
	//void SetWaitState( ioBaseChar *pChar );

public:
	void SetTargetMoveState( bool bSendNet );
	void SetEndState( bool bSkillEnd, bool bCheckCombo, bool bSendNet );

	void CheckTargetMoveState( ioBaseChar *pChar, bool bApplyCol );
	void SetTraceTargetName( const ioHashString &szName );

	bool IsUroborusMove();
	bool IsUroborusTargetMove();

	float GetMaxRange();
	inline float GetTargetMoveSpeed() const { return m_fTargetMoveSpeed; }

	void SetSkillWeapon( bool b ){ m_bSkillWeapon = b;}
	void SetOneTimeColCheck(bool b ){ m_bOneTimeColCheck = b;}

public:
	virtual bool InitResource();
	virtual void LoadProperty( ioINILoader &rkLoader );

	virtual void SetMoveDir( const D3DXVECTOR3 &vMoveDir );
	virtual void SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage );

	virtual void Process( float fTimePerSec, ioPlayStage *pStage );
	virtual void DestroyResource( ioPlayStage *pStage=NULL );
	virtual void SetWeaponDead();

	virtual D3DXVECTOR3 GetMoveDir();

	virtual void SetTeam( ioPlayStage *pStage, TeamType eType );
	virtual void SetWeaponIndex( DWORD dwIndex );

	virtual D3DXVECTOR3 GetAttackDir( const ioPlayEntity *pEntity ) const;

public:
	virtual const ioOrientBox& GetLocalAttackBox() const;

	virtual WeaponType GetType() const;
	virtual ActionStopType GetActionStopType() const;
	virtual ActionStopType GetDieActionStopType() const;

	virtual bool IsCollision( ioPlayEntity *pEntity, bool bMapCollision=false );
	virtual void CheckPreCollision( ioPlayEntity *pTarget, ioPlayStage *pStage );

	virtual DWORD GetCollisionEnableTime() const;

	virtual bool TestMapCollision( ioOpcodeShape *pShape, ioPlayStage *pStage, const D3DXMATRIX* pShapeWorldMat );

	virtual void SetOwnerChargeScaleRate( float fRate );
	virtual void ApplyWeaponSync( SP2Packet &rkPacket, ioPlayStage *pStage );

protected:
	virtual void UpdateWorldAttackBox( float fTimePerSec, bool bSetNull );

public:
	ioUroborusWeapon3( ioBaseChar *pOwner, const WeaponAttribute *pAttr );
	virtual ~ioUroborusWeapon3();
};

inline ioUroborusWeapon3* ToUroborusWeapon3( ioWeapon *pWeapon )
{
	if( !pWeapon || pWeapon->GetType() != ioWeapon::WT_UROBORUS3 )
		return NULL;

	return dynamic_cast< ioUroborusWeapon3* >( pWeapon );
}

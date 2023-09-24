#pragma once

class ioPlayStage;
class ioPlayMode;
class ioINILoader;
class SP2Packet;

class ioMoveAttack3Dummy : public ioDummyChar
{
public:
	enum OwnerControlSync
	{
		SYNC_ATTACK_KEY    = 1000,
	};

	enum MoveState
	{
		MS_WAIT,
		MS_MOVE,
	};

protected:
	MoveState  m_MoveState;

	CEncrypt<DWORD> m_dwDelayTime;
	DWORD m_dwReturnTime;

	TargetWoundType m_TargetWoundType;
	CEncrypt<float> m_fTargetRange;
	CEncrypt<float> m_fTargetAngle;
	CEncrypt<float> m_fDummyHeightOffset;
	CEncrypt<DWORD> m_dwTargetRefreshTime;

	float m_fCheckMoveRange;

	D3DXVECTOR3 m_vTargetPos;
	float		m_fNeedMoveLength;
	float		m_fMoveSpeed;
	bool		m_bCreateWeapon;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );

protected:
	virtual void ProcessState( float fTimerPerSec, DWORD dwPreTime );
	virtual void ProcessMoveState( float fTimePerSec );
	void ProcessStartState( float fTimePerSec );

	void SetWaitState();

public:
	//virtual void ApplyDummyCharSync( ioPlayStage *pStage, SP2Packet &rkPacket );
	//virtual void SetDieState( bool bEffect );

	virtual bool IsActivateWeapon();

	void SetMovePosition( const D3DXVECTOR3& vTargetPos );


public:
	ioMoveAttack3Dummy( ioEntityGroup *pGrp, ioPlayMode *pMode );
	virtual ~ioMoveAttack3Dummy();
};

inline ioMoveAttack3Dummy* ToioMoveAttack3Dummy( ioDummyChar *pDummy )
{
	if( !pDummy || pDummy->GetType() != DCT_MOVE_ATTACK3 )
		return NULL;

	return dynamic_cast< ioMoveAttack3Dummy* >( pDummy );
}
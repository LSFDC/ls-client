#pragma once

class ioBaseChar;

class ioSpecialStateBase
{
protected:
	enum 
	{
		STATE_FREEZING_VALUE = 500,
	};

	ioBaseChar* m_pOwner;

public:
	virtual void Initialize( ioBaseChar* pOwner, CharState eCurrState );					//�ʱ�ȭ(SetState�� ���ؼ� ���� State�� �����Ҷ�)
	virtual void Finalize( ioBaseChar* pOwner, CharState eCurrState, CharState eNewState );	//������(SetState�� ���ؼ� �ٸ� State�� �����Ͽ� ���� State�� �Ҹ��ų��)
	virtual void LoadProperty( ioINILoader &rkLoader ){}

public:
	virtual const char* GetStateName() PURE;
	virtual const CharState GetState() PURE;

public:
	//���ݼӵ� �����Լ�
	virtual float GetAttackSpeedRate( ioBaseChar* pOwner, float fMaxSpeed ){ return fMaxSpeed; }

public:
	virtual void CheckSpecialState( ioBaseChar* pOwner ){}
	virtual void ProcessState( IN ioBaseChar* pOwner ){}
	virtual void ProcessMoveSpeed( ioBaseChar* pOwner ){}

	virtual bool ProcessMove( ioBaseChar* pOwner, float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );
	virtual void ProcessKeyInput( ioBaseChar* pOwner );

public:
	virtual bool IsCanNoWound( ioBaseChar* pOwner ) { return false; };
	virtual bool IsCollisionAvailableWeapon( ioBaseChar* pOwner, ioWeapon *pWeapon ) { return true; }
	virtual bool IsEntityCollisionSkipState( const ioBaseChar *const pOwner, ioGameEntity* pColEntity ) const { return false; }
	virtual bool IsCharCollisionSkipState( const ioBaseChar *const pOwner,
										DWORD dwStartTime,
										DWORD dwEndTime,
										DWORD dwSkipType,
										bool bTeamOnly,
										bool bDefense,
										TeamType eOwnerTeam,
										TeamType eOtherTeam ) const;

	virtual bool IsSendControlInfoToUser( ioBaseChar* pOwner ){ return true; }	
	virtual bool IsIgnoreAttackOverDelay(){ return false; }
	virtual bool IsLandingEnable(){ return true; }
	virtual bool IsFallStateCheck(){ return true; }
	virtual bool IsEnableBlowWounded( ioBaseChar *pOwner ){ return true; }
	virtual bool IsNoDropState( ioBaseChar* pOwner ) { return false; }
	virtual void GetGravity( ioBaseChar* pOwner, float &fCurGravity );
	virtual bool IsCanChangeJumppingState( ioBaseChar* pOwner ) { return false; }

	virtual bool IsEnableDashState( ioBaseChar* pOwner ){ return false; }

	//Ư�����°� ���ӵǴ��� �˻��ϴ� �Լ�	
	virtual bool IsStateEnd( ioBaseChar* pOwner ){ return false; }
	virtual bool IsProcessFlyMove(){ return false; }	
	
	virtual bool IsFloatingState( const ioBaseChar * const pOwner ) const { return false; }	

public:
	// ���̽� ĳ���Ϳ��� SetState()�� ���� Ŭ���� ���� ������ ����ȭ ����(�߰� ����� ���¸� ����ȭ �ϱ� ����)
	virtual bool IsInitializeSync(){ return false; }

	// �߰� ���� �߻��� ����ȭ ����
	virtual bool IsJoinSync(){ return true; }

	//�߰� ���� ����ȭ
	virtual void FillJoinSync(  SP2Packet &rkPacket, ioBaseChar* pOwner  );
	virtual void ApplyJoinSync( SP2Packet &rkPacket, ioBaseChar* pOwner );

	//������Ʈ ���� ����ȭ
	virtual void ApplyStateSync( SP2Packet &rkPacket, ioBaseChar* pOwner );

public:
	virtual void CheckFallStateChange( ioBaseChar* pOwner, float fMinFallHeightRate, float fBottomHeight, bool bBottomMaxCheck, float fMaxBottomHeight );

public:
	ioSpecialStateBase();
	virtual ~ioSpecialStateBase();
};
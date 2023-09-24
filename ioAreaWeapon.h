#pragma once


#include "ioAttackAttribute.h"

class SP2Packet;
class ioINILoader;

class ioPlayStage;
class ioPlayEntity;
class ioBaseChar;
class ioSkill;
class ioItem;
class ioWeapon;


struct ChangeMoveInfo
{
	DWORD m_dwChangeTime;

	float m_fChangeSpeed;
	float m_fChangeAngleH;
	float m_fChangeAngleV;

	ChangeMoveInfo()
	{
		m_dwChangeTime = 0;

		m_fChangeSpeed = 0.0f;
		m_fChangeAngleH = 0.0f;
		m_fChangeAngleV = 0.0f;
	}
};

typedef std::vector< ChangeMoveInfo > ChangeMoveInfoList;

class ChangeMoveSort : std::binary_function< const ChangeMoveInfo&, const ChangeMoveInfo&, bool >
{
public:
	bool operator()( const ChangeMoveInfo &lhs , const ChangeMoveInfo &rhs ) const
	{
		if( lhs.m_dwChangeTime < rhs.m_dwChangeTime )
		{
			return true;
		}
		else if( lhs.m_dwChangeTime == rhs.m_dwChangeTime )
		{
			if( lhs.m_fChangeAngleV < rhs.m_fChangeAngleV )
				return true;
		}
		return false;
	}
};


class ioAreaWeapon
{
public:
	enum TargetType
	{
		TT_NONE,
		TT_OWNER,
		TT_TEAM,
		TT_ENEMY,
	};

	enum AreaType
	{
		AT_NONE,
		AT_TIC,
		AT_INOUT,
		AT_SCALE,
		AT_DOUBLE,
		AT_ATTACK,
	};

	enum AreaSubType
	{
		ST_NONE,
		ST_POINT_TO_POINT,
	};

	enum CreateType
	{
		CT_NORMAL,
		CT_STRUCT,
		CT_ATTACHED,
		CT_TARGET_ATTACHED,
		CT_MAP,
		CT_SKILL_DEPENDENCY,
	};

	enum AreaWeaponState
	{
		AWS_READY,
		AWS_ACTION,
	};

	enum AreaMoveType
	{
		AMT_NORMAL,
		AMT_OWNER,
		AMT_TRACE,
		AMT_OWNER_ROT,
	};

	enum AreaUsedType
	{
		AUT_NONE,
		AUT_USED,
	};

	enum AreaSubSyncType
	{
		ASST_POINT_TO_POINT,
	};

	enum VertDirType
	{
		VDT_NONE,
		VDT_UP,
		VDT_DOWN,
	};

	enum RandomGenerateFieldType
	{
		RFT_CIRCLE,
		RFT_RECTANGLE,
	};

protected:
	ioHashString m_AreaWeaponName;

	DWORD m_dwAreaWeaponIndex;
	DWORD m_dwAreaWeaponMapIndex;
	DWORD m_dwCreateWeaponIndex;

	CEncrypt<TeamType> m_TeamType;

	AreaWeaponState m_AreaWeaponState;
	
	ioHashString m_OwnerName;			// AreaWeapon ������
	ioHashString m_TargetName;			// AreaWeapon ������(�پ�ٴϴ� ���)
	
	ioHashString m_AreaWeaponEffectBlue;
	ioHashString m_AreaWeaponEffectRed;
	ioHashString m_AreaWeaponReadyEffectBlue;
	ioHashString m_AreaWeaponReadyEffectRed;
	ioHashString m_AreaWeaponEndEffectBlue;
	ioHashString m_AreaWeaponEndEffectRed;

	bool m_bForceEnd;

	ioHashString m_TargetMapEffect;

	UniqueObjID m_dwAreaWeaponEffect;
	ioHashString m_AreaweaponSound;
	UniqueObjID m_AreaweaponSoundID;

	ioHashString m_OwnerSkill;
	ioHashString m_OwnerItem;

	D3DXQUATERNION m_qtFirstRotate;
	D3DXQUATERNION m_qtRotate;

	D3DXVECTOR3 m_vFirstPos;
	D3DXVECTOR3 m_vStartPos;
	D3DXVECTOR3 m_vPreStartPos;

	//////////////////////////////////////////////////////////////////////////
	bool		m_bUseOffSet;
	float		m_fOffSet;
	//////////////////////////////////////////////////////////////////////////

	CEncrypt<bool> m_bEnableAttacker;
	CEncrypt<bool> m_bEnableAttackCnt;
	CEncrypt<bool> m_bOneTimeUse;
	CEncrypt<bool> m_bUsedAreaWeapon;

	// �������
	CreateType m_CreateType;

	// ������
	AreaType m_AreaType;

	//�� ���� ���
	AreaSubType m_SubAreaType;

	// ���� ����
	TargetColType m_TargetColType;
	TargetWoundType m_TargetWoundType;

	CEncrypt<float> m_fTargetEnableHeight;

	// �����ð�
	CEncrypt<DWORD> m_dwLiveTime;
	CEncrypt<DWORD> m_dwLiveTimeByNPC;
	CEncrypt<DWORD> m_dwLiveTimeByFC;
	CEncrypt<DWORD> m_dwReadyTime;
	CEncrypt<DWORD> m_dwCreatedTime;

	// ������ ���
	float m_fNeedGauge;
	DWORD m_dwGaugeTicTime;
	DWORD m_dwGaugeCheckTime;

	// ���� ���� �ֱ�
	CEncrypt<DWORD> m_dwCheckTicTime;
	DWORD m_dwCheckProcessTime;

	// ���� ����
	CEncrypt<float> m_fCollisionRange;
	CEncrypt<float> m_fCollisionInRange;
	CEncrypt<float> m_fAreaAngle;

	CEncrypt<float> m_fUpHeight;
	CEncrypt<float> m_fUnderHeight;
	CEncrypt<float> m_fWidth;

	CEncrypt<float> m_fCurRange;
	CEncrypt<float> m_fCurInRange;

	// Ȯ�� ���
	float m_fScaleRate;
	DWORD m_dwScaleSlerpTime;

	// ���� ���(�߰�:����ö)
	bool m_bUseLengthKill;
	float m_fKillLength;

	// Force
	CEncrypt<float> m_fAreaForceAmt;
	CEncrypt<float> m_fAreaForceFriction;

	// Move
	AreaMoveType m_AreaMoveType;

	bool m_bOwnerMoveDir;
	CEncrypt<float> m_fAreaMoveSpeed;
	D3DXVECTOR3 m_vAreaMoveDir;

	// Move : Change
	DWORD m_dwPreFrameTime;
	CEncrypt<DWORD> m_dwChangeStartTime;
	CEncrypt<DWORD> m_dwChangeCheckTime;
	ChangeMoveInfoList m_ChangeMoveInfoList;

	// Move : Trace
	bool m_bUsedTrace;
	DWORD m_dwLostTraceTime;
	CEncrypt<DWORD> m_dwNotFindWaitTime;

	DWORD m_dwSencingStartTime;
	CEncrypt<DWORD> m_dwSencingWaitTime;

	ioHashString m_TraceTarget;
	CEncrypt<float> m_fTargetSearchRange;
	CEncrypt<float> m_fTargetTraceRange;

	// Move : LiveArea
	bool m_bUseLiveArea;
	D3DXVECTOR3 m_vMinLivePos;
	D3DXVECTOR3 m_vMaxLivePos;

	DWORD m_dwLiveAreaTime;
	DWORD m_dwLiveAreaEndTime;

	// Move : Owner Rot
	bool m_bFirstMove;
	DWORD m_dwRotateMaxTime;

	// Rotate
	float m_fRotationTime;

	// ����
	ioHashStringVec m_vOwnerBuffList;
	ioHashStringVec m_vTeamBuffList;
	ioHashStringVec m_vEnemyBuffList;
	ioHashStringVec m_vTeamBuffOnCreateWeaponList;

	bool m_bLive;

	BaseCharList m_vNewTargetList;
	ioHashStringVec m_vCurTargetList;
	ioHashStringVec m_vCurBlockList;

	float m_fExtraBuffDuration;

	// AttackAttribute
	WeaponInfo m_WeaponInfo;
	DWORD m_dwWeaponDuration;
	DWORD m_dwWeaponBaseIndex;

	//�߰� yongjun
	//End AttackAttribute
	WeaponInfo m_EndWeaponInfo;
	DWORD m_dwEndWeaponDuration;
	ioHashString m_AreaweaponEndSound;
	//
	
	CEncrypt<bool> m_bUseRandomAttack;

	CEncrypt<DWORD> m_dwGenerateStartTime;
	CEncrypt<DWORD> m_dwMinGenerateTime;
	CEncrypt<DWORD> m_dwMaxGenerateTime;
	CEncrypt<DWORD> m_dwNextGenerateTime;
	
	CEncrypt<float> m_fGenerateHeight;

	GenerateList m_vGenerateList;
	CEncrypt<DWORD> m_dwGenerateSeed;
	D3DXVECTOR3 m_GeneratePos;

	VertDirType     m_VertDirType;
	RandomGenerateFieldType m_RandomGenerateFieldType;

	// double type
	CEncrypt<float> m_fDoubleInRange;

	ioHashStringVec m_vOwnerBuffList2;
	ioHashStringVec m_vTeamBuffList2;
	ioHashStringVec m_vEnemyBuffList2;
	ioHashStringVec m_WeaponEndBuffList;
		
	//only for point to point type data
	UniqueObjID m_dwFirstEffect;
	UniqueObjID m_dwSecondEffect;
	D3DXVECTOR3 m_vFirstPointPos;
	D3DXVECTOR3 m_vSecondPointPos;

	DWORD m_dwPointToPointDelayProcessTime;
	DWORD m_dwPointToPointStartProcessTime;

	float m_fEffectOffset;
	float m_fEffectHeightOffset;
	float m_fDropHeight;
	float m_fDropEnableRange;
	float m_fEnablePositionRange;
	float m_fEnableMaxPositionRange;
	
	float m_fPointToPointMoveRanage;
	float m_fPointToPointTotalMoveAmount;

	ioHashString m_pOwnerItemName;
	ioHashString szTargetIdentifyBuffName;

	// attack type
	D3DXVECTOR3 m_vTargetAttPos;
	DWORD m_dwTargetAttMaxCnt;
	DWORD m_dwTargetAttCurCnt;
	
	DWORD m_dwCheckTargetAttTime;
	CEncrypt<DWORD> m_dwTargetAttGap;

	ioHashStringVec m_TargetAttEffList;
	UniqueObjID m_dwCurTargetAttEff;

	CEncrypt<float> m_fInvisibleRange;

	// Refresh Collision list
	CEncrypt<DWORD> m_dwRefreshDuration;
	struct RefreshCollisionInfo
	{
		ioHashString m_CharName;
		DWORD        m_dwCollisionTime;
		RefreshCollisionInfo( const ioHashString &szName, DWORD dwCollisionTime )
		{
			m_CharName = szName;
			m_dwCollisionTime = dwCollisionTime;
		}
	};
	typedef std::vector<RefreshCollisionInfo> vRefreshCollisionList;
	vRefreshCollisionList m_RefreshCollisionList;

	bool m_bDelayCreateAreaAttack;
	DWORD m_dwDelayCreateAreaAttackTime;
	DWORD m_dwCheckCreateAreaAttackTime;

	bool m_bSetRepeatAttack;

public:
	ioAreaWeapon* Clone();
	void LoadProperty( ioINILoader &rkLoader );

public:
	void SetInitInfo( const ioHashString& szOwner,
					  const ioHashString& szTarget,
					  const ioHashString& szSkill,
					  ioPlayStage *pStage,
					  const D3DXVECTOR3 &vStartPos,
					  const D3DXQUATERNION &qtRot,
					  DWORD dwIndex,
					  int eCreateType );

	void ProcessState( ioPlayStage *pStage );
	void DestroyAreaWeapon( ioPlayStage *pStage, bool bActive = true );

	void SetLiveTime( DWORD dwLiveTime );

	DWORD GetCurLiveTime();
	void SetCurLiveTime( DWORD dwCurGapTime );
	void SetAreaReadyTime( DWORD dwReadyTime );

	void FillCurTargetList( SP2Packet &rkPacket );
	void ApplyCurTargetList( ioHashStringVec vTargetList );
	bool IsCreateByStruct();

	void SetAreaPosition( const D3DXVECTOR3& vNewPos, ioPlayStage *pStage );
	void SetAreaRotation( const D3DXQUATERNION &qtTargetRot );
	void SetAreaDirection( const D3DXVECTOR3& vDir );
	void SetAreaRange( float fRange ){ m_fCollisionRange = fRange; }
	void SetAreaWidth( float fWidth ){ m_fWidth = fWidth; }
	void SetAreaEffectScale( ioPlayStage *pStage, D3DXVECTOR3 vScale );
	
	float GetAreaRange(){ return m_fCollisionRange; }
	float GetAreaWidth(){ return m_fWidth; }

	inline bool IsLive() const { return m_bLive; }
	
	inline const ioHashString& GetAreaWeaponName() const { return m_AreaWeaponName; }
	inline int GetCreateType() const { return m_CreateType; }

	inline const ioHashString& GetOwnerName() const { return m_OwnerName; }
	inline const ioHashString& GetTargetName() const { return m_TargetName; }
	inline const ioHashString& GetOwnerSkillName() const { return m_OwnerSkill; }

	inline const D3DXVECTOR3& GetStartPos() const { return m_vStartPos; }
	inline const D3DXVECTOR3& GetPreStartPos() const { return m_vPreStartPos; }
	inline const D3DXQUATERNION& GetRotation() const { return m_qtRotate; }

	inline DWORD GetAreaWeaponIndex() const { return m_dwAreaWeaponIndex; }
	inline float GetAreaWeaponMoveSpeed() const { return m_fAreaMoveSpeed; }

	void ApplyMoveInfo( SP2Packet &rkPacket, ioPlayStage *pStage );
	void ApplySubInfo( SP2Packet &rkPacket, ioPlayStage *pStage );

	void SetAreaWeaponMapIndex( DWORD dwIndex );
	void SetUsedAreaWeapon( bool bUsed );
	void SetUsedTrace( bool bUsed );
	void SetTraceTarget( const ioHashString &szName );
	void SetCreateWeaponIndex( DWORD dwIndex );

	void SetPointToPointInfo( ioPlayStage* pStage,
							  D3DXVECTOR3 vFirstPos, 
							  D3DXVECTOR3 vSecondPos,
							  ioHashString szFirstEffectName,
							  ioHashString szSecondEffectName );

	void SetTargetAttInfo( const D3DXVECTOR3 &vTargetPos, DWORD dwMaxCnt );

	inline int GetAreaWeaponMoveType() const { return m_AreaMoveType; }
	inline DWORD GetAreaWeaponMapIndex() const { return m_dwAreaWeaponMapIndex; }
	inline const bool GetOneTimeUse() const { return m_bOneTimeUse; }
	inline const bool GetUsedAreaWeapon() const { return m_bUsedAreaWeapon; }
	inline const bool GetUsedTrace() const { return m_bUsedTrace; }
	inline const ioHashString& GetTraceTargetName() const { return m_TraceTarget; }
	inline DWORD GetCreateWeaponIndex() const { return m_dwCreateWeaponIndex; }

protected:
	void LoadOwnerBuffList( ioINILoader &rkLoader );
	void LoadTeamBuffList( ioINILoader &rkLoader );
	void LoadEnemyBuffList( ioINILoader &rkLoader );

	bool SetOwnerBuffList( ioBaseChar *pChar );
	bool SetTeamBuffList( ioBaseChar *pOwner, ioBaseChar *pChar );
	bool SetEnemyBuffList( ioBaseChar *pOwner, ioBaseChar *pChar, ioPlayStage *pStage );
	bool SetTeamBuffOnCreateWeaponList( ioBaseChar *pOwner, ioBaseChar *pChar );

	bool SetOwnerBuffList2( ioBaseChar *pChar );
	bool SetTeamBuffList2( ioBaseChar *pOwner, ioBaseChar *pChar );
	bool SetEnemyBuffList2( ioBaseChar *pOwner, ioBaseChar *pChar, ioPlayStage *pStage );

	void LoadAttributeInfo( ioINILoader &rkLoader );

	//�߰� yongjun end_attack ������ ini�� ����
	void LoadEndAttributeInfo( ioINILoader &rkLoader );
	//AreaWeapon�� �Ҹ�Ǹ� end_weapon ����
	void CreateEndAreaAttack( ioPlayStage *pStage );
	//
	void CreateAreaAttack( ioPlayStage *pStage );
	void CheckAreaAttack( ioPlayStage *pStage, bool bRefresh );
	void CheckAreaTargetTeleport( ioPlayStage *pStage );

	void CheckNeedGauge( ioPlayStage *pStage );
	void CheckAreaWeaponBuff( ioPlayStage *pStage );
	void CheckAreaWeaponTarget( ioPlayStage *pStage );
	void CheckAreaWeaponBuffByDouble( ioPlayStage *pStage );
	void CheckAreaWeaponBuffOnCreateWeapon( ioPlayStage *pStage );

	void CheckTargetRotation();
	void CheckTargetPosition();

	void UpdateTargetListByInOut( ioPlayStage *pStage );
	void UpdateTargetListByScale( ioPlayStage *pStage );
	void UpdateTargetListByDouble( ioPlayStage *pStage );

	void UpdateCollisionRange();
	
	void GetTargetCharListBySphere( ioPlayStage *pStage, BaseCharList &rkCharList );
	void GetTargetCharListByCylinder( ioPlayStage *pStage, BaseCharList &rkCharList );
	void GetTargetCharListByBox( ioPlayStage *pStage, BaseCharList &rkCharList );

	void RemoveTargetBuff( ioBaseChar *pTarget, TargetType eType, bool bSend );
	bool IsCollisioned( ioBaseChar *pTarget, TargetType eType );
	bool IsBlocked( ioBaseChar *pTarget );

	bool FindTarget( const ioHashString &szTarget );

	void SetAreaForce( ioBaseChar *pChar );

	bool CheckIgnoreProtectBuff();

	// Move
	void LoadChangeMoveInfoList( ioINILoader &rkLoader );

	void ProcessAreaWeaponMove( ioPlayStage *pStage );

	// Move : Change
	void CheckChangeMove( ioPlayStage *pStage );
	void CheckChangeMove( ioPlayStage *pStage, DWORD dwGapTime );
	void RotateMoveDir( float fAngleH, float fAngleV, ioPlayStage *pStage );

	// Move : Trace
	void FindTraceTarget( ioPlayStage *pStage );
	bool CheckTraceTarget( ioPlayStage *pStage );

	void SetReposition( ioPlayStage *pStage );

	// Move : Area
	bool CheckLiveAreaInAreaWeapon( ioPlayStage *pStage );
	bool CheckLiveAreaInChar( ioBaseChar *pChar, ioPlayStage *pStage );

	// Move : Owner Rot
	void CheckOwnerRot( ioBaseChar *pChar, ioPlayStage *pStage, float fTimePerSec );

	// Double
    bool CheckDoubleRange( ioBaseChar *pChar );

	void CheckVisible( ioPlayStage *pStage );

protected:
	void LoadRandomAttack( ioINILoader &rkLoader );
	void GenerateRandomAttack( ioBaseChar *pChar );
	void ProcessRandomAttack( ioBaseChar *pChar );
	void ProcessTargetAttack( ioBaseChar *pChar );

	bool CheckRefreshCollisionList( const ioHashString &szName, ioPlayStage *pStage );
	bool CheckTargetEquipObjectBuff( const ioHashString& szBuffName, ioBaseChar* pTarget );

public:
	DWORD GetLiveTime();

public:
	ioAreaWeapon();
	ioAreaWeapon( const ioAreaWeapon &rhs );
	virtual ~ioAreaWeapon();
};


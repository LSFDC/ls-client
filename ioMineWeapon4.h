

#include "ioWeapon.h"
#include "ioAttackAttribute.h"

class ioMineWeapon4 : public ioWeapon
{
protected:
	ioEntityGroup *m_pMine;

	DWORD	m_dwSensingStartTime;
	DWORD	m_dwTraceDelayTime;			// Target ���� ���� �ð�(INI)
	DWORD	m_dwCurTraceDelayTime;

	float	m_fExplosionRange;
	float	m_fMineBoomDist;
	float	m_fVisibleRange;
	float	m_fMineSensingRange;

	float m_fMineSizeRate;				// ������ ������ ����

	ioHashString m_BlueMineModel;
	ioHashString m_RedMineModel;
	ioHashString m_BoomEffect;
	ioHashString m_NormalDeadEffect;

	ioHashStringVec m_vTeamModelList;
	ioHashStringVec m_vEnemyModelList;

	ioOrientBox m_WorldCollisionBox;
	ioOrientBox m_LocalAttackBox;
	bool m_bMineVisible;
	bool m_bTeamModel;
	bool m_bWait;

	// Extend
	TeamType m_CurTeamColor;

	bool m_bUseTeamTargetBuff;
	bool m_bNoSencingMine;
	bool m_bNoMineExplosion;
	bool m_bNoMineUp;
	bool m_bNormalDead;

	BuffInfoList m_RedTargetAirBuffList;
	BuffInfoList m_RedTargetBuffList;
	BuffInfoList m_RedTargetDownBuffList;


	enum MineState
	{
		MS_CHARGE,
		MS_SENSING,
		MS_WAIT_EXPLOSION,						// Trace �� ��� ���� 
		MS_EXPLOSION,
		MS_NORMAL_DEAD,
	};
	MineState m_State;

	MineType m_MineType;

	CEncrypt<float> m_fMineOffsetY;

	ioHashString m_szRedMineEffect;
	ioHashString m_szBlueMineEffect;
	UniqueObjID m_MineEffectID;
protected:
	int  m_iConveyerMoveFrameIdx;

	// For Mine Sync
protected:
	int m_iAttributeIndex;

public:
	static ActionStopType m_ActionStopType;
	static ActionStopType m_DieActionStopType;

public:
	void SetSensingStartTime( DWORD dwTime ) { m_dwSensingStartTime = dwTime; }

public:
	bool IsCanExplosion() const;
	bool IsCanVisible( ioWeapon *pWeapon ) const;

	virtual const BuffInfoList& GetTargetAirBuffList() const;
	virtual const BuffInfoList& GetTargetBuffList() const;
	virtual const BuffInfoList& GetTargetDownBuffList() const;

public:
	void SetWaitExplosionMine( ioPlayStage *pStage, bool bSendNetwork );
	void SetNormalDeadMine( ioPlayStage *pStage, bool bSendNetwork );
	void TranslateByConveyer( const D3DXVECTOR3 &vMoveAmt );

	// bWait ���� ���������ӿ��� �ٸ� ������Ʈ(ĳ����, �����)�� ���� �̸� �������� ���
	// Process �������� SetWeaponDead()�� ��Ű�� �ʱ� ���� �����ϴ� ��
	void ExplosionMine( ioPlayStage *pStage, bool bWait=false );
	void NormalDeadMine( ioPlayStage *pStage, bool bWait=false );

protected:
	void OnCharge( ioPlayStage *pStage );
	void OnSensing( ioPlayStage *pStage, float fTimePerSec );
	void OnWaitExplosion( ioPlayStage *pStage );
	void OnExplosion();
	void OnNormalDead();

	bool DropZoneCheck( ioPlayStage *pStage );

	void VisibleCheck( ioPlayStage *pStage );
	void UpdateWorldCollisionBox();
	void CreateMineEffect( float fTimePerSec, ioPlayStage *pStage );
	void ProcessPrisionEffect( float fTimePerSec, ioPlayStage *pStage );
public:
	DWORD GetSensingStartGapTime();

	inline const ioOrientBox& GetWorldCollisionBox() { return m_WorldCollisionBox; }

	inline MineType GetMineType() const { return m_MineType; }
	inline bool IsCanMineSensing() const { return !m_bNoSencingMine; }
	inline bool IsNormalDead() const { return m_bNormalDead; }

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual void SetTeam( ioPlayStage *pStage, TeamType eType );
	virtual void SetStartPosition( const D3DXVECTOR3 &vPos, ioPlayStage *pStage );
	virtual void Process( float fTimePerSec, ioPlayStage *pStage );
	virtual void DestroyResource( ioPlayStage *pStage=NULL );

	virtual bool IsCollision( ioPlayEntity *pEntity, bool bMapCollision=false );
	virtual bool IsCanMineExplosion();

	virtual DWORD GetCollisionEnableTime() const;

public:
	virtual const ioOrientBox& GetLocalAttackBox() const;
	virtual WeaponType GetType() const;
	virtual ActionStopType GetActionStopType() const;
	virtual ActionStopType GetDieActionStopType() const;

public:
	ioMineWeapon4( ioBaseChar *pOwner, const WeaponAttribute *pAttr );
	virtual ~ioMineWeapon4();
};

inline ioMineWeapon4* ToMineWeapon4( ioWeapon *pWeapon )
{
	if( !pWeapon || pWeapon->GetType() != ioWeapon::WT_MINE4 )
		return NULL;

	return dynamic_cast< ioMineWeapon4* >( pWeapon );
}


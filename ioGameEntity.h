#pragma once

#include "../io3DEngine/io3DCommon.h"

#include "../io3DEngine/ioNode.h"
#include "../io3DEngine/ioMovableObject.h"
#include "../io3DEngine/ioEntityGroup.h"

#include "../io3DEngine/ioSphere.h"
#include "../io3DEngine/ioOrientBox.h"
#include "../io3DEngine/ioAxisAlignBox.h"

class ioEntityGroup;

#define MAX_ALPHA_RATE	255


const float frm_max = 60;									// ������ 60������ ��������..
const float check_frm = 500.f;								// 500ms ������ üũ��..
const float sec_frm = 1000.f / frm_max;
const float fcheck_cnt = check_frm / sec_frm;				// üũ�� ����

const int	ICHECK_CNT		= (int)fcheck_cnt;	//üũ����
const int	ICHECK_TIME		= (int)sec_frm;		//üũ����

class ioGameEntity : public ioEntityEvent
{
public:
	enum GameEntityType
	{
		GET_PLAY_ENTITY = 1,
		GET_WORLD_DECO  = 2,
	};

protected:
	ioEntityGroup *m_pGroup;

	int		m_iCurAlphaRate;
	int		m_iTargetAlphaRate;

	mutable ioOrientBox m_WorldColBox;
	mutable ioAxisAlignBox m_WorldAxisBox;
	mutable float	m_fCurBottomHeight;

	mutable bool m_bNeedUpdateBound;
	mutable bool m_bNeedUpdateBottomHeight;
	bool m_bShadowCastEnable;
#ifdef USE_LAGTEST
	struct sDelayBox
	{
		DWORD dwLastTime;
		D3DXVECTOR3 WorldPosition;
		ioOrientBox WorldColBox;
		sDelayBox() : dwLastTime(0){}
	};

	bool m_bUseLatency;		// �� ĳ�� �������
	bool m_bCanLatency;		// ��� ������ ��������
	bool m_bIsLatency;		// �� ��������

	DWORD m_dwLatency;					// �ش������� delay��
	DWORD m_dwLatencyDelay;				// �� ���� ��ġ �������� ���Ѱ�

	std::list<sDelayBox*> m_listDelayBox;
#endif

public:
	virtual void Update( float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime ) = 0;
	virtual void UpdateAfter( float fFrameGap );
	virtual int GetTypeID() const = 0;

#ifdef USE_LAGTEST
	void UseLatencyDelay();
	void UpdateDelayBox();
	void UpdateDelayBound();

	inline bool IsUseLatency() { return m_bUseLatency; }
	inline bool IsCanLatency() { return m_bCanLatency; }
	inline bool IsCanLatency() const { return m_bCanLatency; }

	inline DWORD GetLatency() const { return m_dwLatency; }	
	inline void  OffLatencyDelay() { m_bIsLatency = false; }

	virtual void InitLatencyDelay();		
	virtual void SetLatencyDelay( DWORD dwLatencyDelay );

	sDelayBox* GetDelayBox() const;
#endif

public:
	virtual bool IsNeedCheckMapCollision() const;
	virtual bool IsMountAvailable() const;		// �ڽ��� ���� �ٸ� ������Ʈ�� Mount �Ҽ� �ִ� ��ü�ΰ�?
	virtual bool IsMountingAvailable() const;	// �ٸ� ������Ʈ�� �׿����� �ִ� ��ü�ΰ�? Mounted�� �ɼ� �ֳ�?
	virtual bool IsNowMountEnable() const;		// ���� Mount�� ������ �����ΰ�?

public:
	virtual bool IsAllWeaponDefence() const;	// ��� ���⸦ ���� �� �ִ°�?

public:
	void SetShadowCastEnable( bool bEnable );
	void ApplyShadowCast( bool bApply );

	inline bool IsShadowCastEnable() const { return m_bShadowCastEnable; }

public:
	void SetAlphaRateDirect( int iRate );
	void SetAttachEffectAlphaRateDirect( int iRate, bool bMinZero = false );
	void SetTargetAlphaRate( int iTarget );

	inline int  GetCurrentAlphaRate() const { return m_iCurAlphaRate; }
	inline bool IsNowAlphaChanging() const
	{
		if( m_iCurAlphaRate == m_iTargetAlphaRate )
			return false;

		return true;
	}

public:
	virtual bool HasEntity( const ioHashString &szMeshName );
	virtual ioEntity *AddEntity( const ioHashString &zMeshName );
	virtual bool DestroyEntity( const ioHashString &szMeshName );
	virtual ioEntity *GetEntity( const ioHashString &szMeshName );

protected:
	void ProcessAlphaRate( float fTimePerSec );

	virtual void UpdateBound() const;
	virtual void UpdateBottomHeight() const = 0;

public:
	inline void NeedUpdateBound() { m_bNeedUpdateBound = true; }
	virtual void NeedUpdateBottomHeight() { m_bNeedUpdateBottomHeight = true; }

public:
	virtual void SetWorldPosition( const D3DXVECTOR3 &vPos );
	virtual void SetWorldOrientation( const D3DXQUATERNION &qtRot );
	virtual void SetWorldScale( const D3DXVECTOR3 &vScale );

	virtual void Translate( const D3DXVECTOR3 &vMove );
	virtual void TranslateByConveyer( const D3DXVECTOR3 &vMove );
	virtual void TranslateByWorldEntity( const D3DXVECTOR3 &vMove );
	virtual void MapCollisionPushingTranslate( const D3DXVECTOR3 &vMoveAmt );
	virtual void DontMoveCollisionPushingTranslate( ioGameEntity *pPusher, const D3DXVECTOR3 &vMoveAmt );
	virtual void Rotate( const D3DXVECTOR3 &vAxis, float fDegrees, ioNode::TransformSpace eRelativeTo = ioNode::TS_PARENT );

	virtual void Scale( const D3DXVECTOR3 &vScale );
	virtual void Scale( float fScaleX, float fScaleY, float fScaleZ );

	const D3DXMATRIX* GetFullTransform() const;

	virtual const D3DXVECTOR3& GetWorldPosition() const;
	virtual const D3DXQUATERNION& GetWorldOrientation() const;
	virtual const D3DXVECTOR3& GetWorldScale() const;

public:
	const ioOrientBox& GetWorldCollisionBox() const;
	const ioOrientBox& GetCollisionBox() const;
	const ioAxisAlignBox& GetWorldAxisBox() const;

	D3DXVECTOR3 GetMidPositionByRate( float fRate = FLOAT05 ) const;
	float GetMidHeightByRate( float fRate = FLOAT05 ) const;
	float GetBottomHeight() const;

public:
	inline ioEntityGroup* GetGroup() const { return m_pGroup; }
	UniqueObjID PlaySound( const ioHashString &rkName, bool bLoop = false );

public:
	ioGameEntity( ioEntityGroup *pGrp );
	virtual ~ioGameEntity();
};


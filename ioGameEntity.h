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


const float frm_max = 60;									// 적당히 60프레임 기준으로..
const float check_frm = 500.f;								// 500ms 정도만 체크함..
const float sec_frm = 1000.f / frm_max;
const float fcheck_cnt = check_frm / sec_frm;				// 체크할 갯수

const int	ICHECK_CNT		= (int)fcheck_cnt;	//체크갯수
const int	ICHECK_TIME		= (int)sec_frm;		//체크간격

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

	bool m_bUseLatency;		// 이 캐릭 사용할지
	bool m_bCanLatency;		// 사용 가능한 상태인지
	bool m_bIsLatency;		// 렉 상태인지

	DWORD m_dwLatency;					// 해당유저의 delay값
	DWORD m_dwLatencyDelay;				// 내 이전 위치 가져오기 위한값

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
	virtual bool IsMountAvailable() const;		// 자신의 위에 다른 오브젝트를 Mount 할수 있는 객체인가?
	virtual bool IsMountingAvailable() const;	// 다른 오브젝트에 쌓여질수 있는 객체인가? Mounted가 될수 있나?
	virtual bool IsNowMountEnable() const;		// 현재 Mount가 가능한 상태인가?

public:
	virtual bool IsAllWeaponDefence() const;	// 모든 무기를 막을 수 있는가?

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


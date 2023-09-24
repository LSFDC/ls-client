#pragma once


#include "../io3DEngine/ioSphere.h"
#include "../io3DEngine/ioOrientBox.h"

class ioSkeleton;
class ioCylinder;

struct WoundedCollisionBox
{
	ioOrientBox m_LocalBox;
	ioOrientBox m_WorldBox;

	ioHashString m_LinkBone;
	int m_iLinkBipedIndex;
};

class ioWoundedCollisionBoxGrp
{
private:
	typedef std::vector< WoundedCollisionBox* > WoundedCollisionBoxList;
	WoundedCollisionBoxList m_WoundedCollisionBoxList;
	ioSphere m_WorldBoundingSphere;
#ifdef USE_LAGTEST
	struct sDelayBox
	{
		DWORD						dwUpdateTime;
		ioSphere					WorldBoundingSphere;
		WoundedCollisionBoxList		collBoxList;
		sDelayBox() : dwUpdateTime(0){}
	};

	bool		m_bBaseChar;
	DWORD		m_dwdLastTime;

	std::list<sDelayBox*> m_listDelayBox;

public:
	void SetBaseChar();
	void UpdateDelayBox();
#endif	

public:
	bool InitBoxList( const char *szFileName );

private:
	bool IsWoundedCollisionFile( const char *szToken, int iVersion );
	WoundedCollisionBox* CreateNewBox();
	void ClearBoxList();

public:
	void UpdateWorldBoxList( const ioSkeleton *pSkeleton, const D3DXMATRIX *pWorldXform );
	void UpdateBipedIndex( const ioSkeleton *pSkeleton );

public:
	bool IsCollision( const ioOrientBox &rkAttackBox, D3DXVECTOR3 *pColPoint = NULL ) const;

	bool CheckSphereDistance( const D3DXVECTOR3 &vStart, float fRange, float &fDistance ) const;
	bool CheckSpherePoint( const D3DXVECTOR3 &vStart, float fRange, D3DXVECTOR3 &vPoint ) const;
	bool CheckCylinderPoint( const ioCylinder &rkCylinder,
							 const D3DXVECTOR3 &vStart,
							 D3DXVECTOR3 &vPoint ) const;

#ifdef USE_LAGTEST
	bool IsCollisionLatency( const ioOrientBox &rkAttackBox, DWORD dwLatency, D3DXVECTOR3 *pColPoint = NULL ) const;
	bool CheckSphereDistanceLatency( const D3DXVECTOR3 &vStart, float fRange, float &fDistance, DWORD dwLatency ) const;
	bool CheckSpherePointLatency( const D3DXVECTOR3 &vStart, float fRange, D3DXVECTOR3 &vPoint, DWORD dwLatency ) const;
#endif


public:
	bool GetLocalBox( const ioHashString &rkBiped, ioOrientBox &rkLocalBox ) const;

	bool GetBoxVertexArray( int iIndex, D3DXVECTOR3 pArray[8] ) const;
	int GetBoxCount() const;

public:
	ioWoundedCollisionBoxGrp* Clone();

public:
	ioWoundedCollisionBoxGrp();
	ioWoundedCollisionBoxGrp( const ioWoundedCollisionBoxGrp &rhs );
	~ioWoundedCollisionBoxGrp();
};


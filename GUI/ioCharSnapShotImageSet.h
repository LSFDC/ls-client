#pragma once

class ioRenderSystem;
class ioRenderTexture;

class ioUI3DRender;
class ioUIRenderImage;
class ioBaseChar;

class ioCharSnapShotImageSet
{
protected:
	ioRenderSystem  *m_pSystem;
	ioRenderTexture *m_pRenderTexture;
	ioUI3DRender	*m_pUI3DRender;

	struct SnapShot
	{
		ioHashString m_Name;
		ioUIRenderImage *m_pImg;
	};
	typedef std::vector< SnapShot > SnapShotPool;
	SnapShotPool m_SnapShotPool;

public:
	bool Initialize( ioRenderSystem *pSystem );
	void AddCharSnapShot( ioBaseChar *pChar );
	void AddDummyCharSnapShot( const ioHashString &szName );
	void ClearSnapShotNames();

protected:
	void DestorySnapShotPool();
	void UpdateSnapShot( SnapShot *pSnapShot );	

	SnapShot* FindEnableSnapShot( const ioHashString &rkName );	// ���� �̸��� ������ ������, ������ ���, 	
	ioUIRenderImage* GetSnapShotImg( const ioHashString &rkName );

public:
	void RenderCharSnapShot( const ioHashString &rkName, int iXPos, int iYPos );

public:
	ioCharSnapShotImageSet();
	virtual ~ioCharSnapShotImageSet();
};


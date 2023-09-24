//----------------------------------------------------------------------------------
/*
Filename	: ioPaperDollWnd
Author		: sumin
Date		: 113.11.5_14:41:3
*/
//----------------------------------------------------------------------------------

#ifndef _IOPAPERDOLLWND_
#define _IOPAPERDOLLWND_

#include "ioWndEX.h"
#include "../CharacterInfo.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioPaperDoll
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioUI3DRender;
class ioCreateChar;
class ioItem;
class ioPaperDoll
{
public:

	enum EQUIP_TYPE
	{
		ET_WEAPON,
		ET_ARMOR,
		ET_HELMET,
		ET_CAPE,
	};

public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ioPaperDoll();
	~ioPaperDoll();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	void					Init( const char* INIFilename, const char* caminfo );
	const CHARACTER&		GetCharacterInfo() const { return m_charinfo; }
	// character 积己
	//void					SetMyChar( const char* instancename, int classtype, int sextype );
	void					SetChar( const char* instancename, const CHARACTER& charinfo );
	// 厘厚
	void					SetEquip();
	void					SetEquip( const ITEM_DATA& item );
	void					SetEquip( const ioItem* item );
	void					UnEquip( EQUIP_TYPE type );
	// 摹厘
	void					SetDeco( const CHARACTER& info );		
	// character
	void					SetPosition( const D3DXVECTOR3& pos );
	const D3DXVECTOR3&		GetPosition() const;
	void					SetScale( const D3DXVECTOR3& scale );
	void					SetYaw( float yaw );
	void					SetLocation( const D3DXVECTOR3& pos, float yaw ) { SetPosition( pos ); SetYaw( yaw ); }
	void					AttachEffect( const char* filename );
	// animation
	void					SetWeaponShopAnimation();
	void					SetLoopAnimation( int aniidx, float fadein = 0.0f );
	void					SetLoopAnimation( const ioHashString& aniname, float fadein = 0.0f );
	void					SetActionAnimation( const ioHashString& aniname, float fadein = 0.0f, float fadeout = 0.0f );
	void					SetItemAnimation( DWORD item );
	void					SetItemAnimation( const ioHashString& aniname );
	// camera
	void					LookAt( const D3DXVECTOR3& pos );

	//----------------------------------------------------------------------------------
	/*
	render
	*/
	void					Update( float dt );
	void					Render( int x, int y, float width, float height );

protected:

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	// character 积己
	//void					SetMyCharInfo( int classtype, int sextype );
	//void					SetMyDefaultCharInfo( int classtype, int sextype );

protected:

	std::string				m_instancename;

	ioUI3DRender*			m_3drenderer;
	ioCreateChar*			m_char;
	CHARACTER				m_charinfo;
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioPaperDollWnd
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioPaperDollWnd : public ioWndEX
{
public:

	//----------------------------------------------------------------------------------
	/*
	积己 / 家戈
	*/
	ioPaperDollWnd();
	virtual ~ioPaperDollWnd();

	//----------------------------------------------------------------------------------
	/*
	ioWnd Override
	*/
	virtual void		OnProcess( float dt );
	virtual void		OnRender();

	//----------------------------------------------------------------------------------
	/*
	control
	*/
	ioPaperDoll&		GetPaperDoll() { return m_doll; }
	const CHARACTER&	GetCharacterInfo() const { return m_doll.GetCharacterInfo(); }
	void				SetDollOffset( int x, int y ) { SetDollOffsetX( x ); SetDollOffsetY( y ); }
	void				SetDollOffsetX( int x ) { m_dolloffsetx = x; }
	void				SetDollOffsetY( int y ) { m_dolloffsety = y; }
	int					GetDollOffsetX() const { return m_dolloffsetx; }
	int					GetDollOffsetY() const { return m_dolloffsety; }

protected:

	ioPaperDoll			m_doll;

	int					m_dolloffsetx;
	int					m_dolloffsety;
};


#endif // _IOPAPERDOLLWND_




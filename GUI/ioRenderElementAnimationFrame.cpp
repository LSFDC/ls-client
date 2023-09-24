
//----------------------------------------------------------------------------------
/*
Filename	: ioRenderElementAnimationFrame.cpp
Author		: megamegy
Date		: 2013.10.31
Desc		: 
*/
//----------------------------------------------------------------------------------

#include "stdafx.h"

#include "ioRenderElementAnimationFrame.h"
#include "../io3DEngine/ioUIRenderElement.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrame
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
���� / �Ҹ�
*/
ioRenderElementAnimationFrame::ioRenderElementAnimationFrame( ioUIRenderElement* element, DWORD targettime )
	: m_lastupdatetime( 0 )
	, m_targettime( targettime )
{
	assert( element != 0 );

	m_elements.push_back( ElementInfo( element ) );
}

ioRenderElementAnimationFrame::ioRenderElementAnimationFrame( const RenderElementVec& elements, DWORD targettime )
	: m_lastupdatetime( 0 )
	, m_targettime( targettime )
{
	int num = static_cast<int>( elements.size() );
	for ( int i = 0; i < num; ++i )
	{
		assert( elements[ i ] != 0 );
		m_elements.push_back( ElementInfo( elements[ i ] ) );
	}
}

ioRenderElementAnimationFrame::~ioRenderElementAnimationFrame()
{
}

//----------------------------------------------------------------------------------
/*
Update : ó���� �Ϸ�Ǿ��ٸ� true�� �����Ѵ�.
*/
bool	ioRenderElementAnimationFrame::Update()
{
	if ( m_elements.empty() ) {
		return true;
	}

	if ( m_targettime == 0 ) 
	{
		SetTargetValue();
		return true;
	}

	DWORD dt = FRAMEGETTIME() - m_lastupdatetime;

	bool ret = true;
	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( int i = 0; iter != eiter; ++iter, ++i )
	{	
		if ( !UpdateElement( dt, *iter, i ) ) {
			ret = false;
		}
	}

	m_lastupdatetime = FRAMEGETTIME();
	return ret;
}

//----------------------------------------------------------------------------------
/*
�������� ������ ��
*/
void	ioRenderElementAnimationFrame::OnBegin()
{
	m_lastupdatetime = FRAMEGETTIME();
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameEmpty
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
���� / �Ҹ�
*/
ioRenderElementAnimationFrameEmpty::ioRenderElementAnimationFrameEmpty( ioUIRenderElement* element, DWORD targettime )
	: ioRenderElementAnimationFrame( element, targettime )
	, m_acctime( 0 )
{
}

ioRenderElementAnimationFrameEmpty::ioRenderElementAnimationFrameEmpty( const RenderElementVec& elements, DWORD targettime )
	: ioRenderElementAnimationFrame( elements, targettime )
	, m_acctime( 0 )
{
}

ioRenderElementAnimationFrameEmpty::~ioRenderElementAnimationFrameEmpty()
{
}

//----------------------------------------------------------------------------------
/*
Update : ó���� �Ϸ�Ǿ��ٸ� true�� �����Ѵ�.
*/
bool	ioRenderElementAnimationFrameEmpty::Update()
{
	DWORD dt = FRAMEGETTIME() - m_lastupdatetime;
	m_acctime += dt;
	return ( m_acctime >= m_targettime );
}

//----------------------------------------------------------------------------------
/*
�������� ������ ��
*/
void	ioRenderElementAnimationFrameEmpty::OnBegin()
{
	ioRenderElementAnimationFrame::OnBegin();

	m_acctime = 0;
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameScale
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
���� / �Ҹ�
*/
ioRenderElementAnimationFrameScale::ioRenderElementAnimationFrameScale( ioUIRenderElement* element, const D3DXVECTOR2& targetscale, DWORD targettime )
	: ioRenderElementAnimationFrame( element, targettime )
	, m_targetscale( targetscale )
{
}

ioRenderElementAnimationFrameScale::ioRenderElementAnimationFrameScale( const RenderElementVec& elements, const D3DXVECTOR2& targetscale, DWORD targettime )
	: ioRenderElementAnimationFrame( elements, targettime )
	, m_targetscale( targetscale )
{
}

ioRenderElementAnimationFrameScale::~ioRenderElementAnimationFrameScale()
{
}

//----------------------------------------------------------------------------------
/*
������ ��Ҹ� update�ϴ� �Լ�
*/
bool	ioRenderElementAnimationFrameScale::UpdateElement( DWORD dt, ioRenderElementAnimationFrame::ElementInfo& info, int index )
{	
	if ( info.m_speed == 0.0f ) 
	{
		info.m_element->SetXScale( m_targetscale.x );
		info.m_element->SetYScale( m_targetscale.y );
		return true;
	}

	D3DXVECTOR2 curscale = info.m_element->GetScale();
	D3DXVECTOR2 vec = m_targetscale - curscale;
	if ( ioMath::IsEqual( vec, D3DXVECTOR2( 0.0f, 0.0f ) ) ) {			
		return true;
	}	

	float tickspeed = info.m_speed * ( dt * FLOAT0001 );
	float distance = std::min<float>( D3DXVec2Length( &vec ), tickspeed );
	D3DXVec2Normalize( &vec, &vec );
	vec = vec * distance;

	info.m_element->SetXScale( curscale.x + vec.x );
	info.m_element->SetYScale( curscale.y + vec.y );
	return false;
}

//----------------------------------------------------------------------------------
/*
�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
*/
void	ioRenderElementAnimationFrameScale::SetTargetValue()
{
	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter )
	{
		(*iter).m_element->SetXScale( m_targetscale.x );
		(*iter).m_element->SetYScale( m_targetscale.y );
	}
}

//----------------------------------------------------------------------------------
/*
�������� ������ ��
*/
void	ioRenderElementAnimationFrameScale::OnBegin()
{
	ioRenderElementAnimationFrame::OnBegin();

	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter )
	{	
		D3DXVECTOR2 curscale = iter->m_element->GetScale();
		D3DXVECTOR2 vec = m_targetscale - curscale;
		if ( m_targettime == 0 || ioMath::IsEqual( vec, D3DXVECTOR2( 0.0f, 0.0f ) ) ) 
		{
			iter->m_speed = 0.0f;
		}
		else 
		{
			float distance = D3DXVec2Length( &vec );
			iter->m_speed = distance / ( m_targettime * FLOAT0001 );
		}
	}
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameAlpha
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
���� / �Ҹ�
*/
ioRenderElementAnimationFrameAlpha::ioRenderElementAnimationFrameAlpha( ioUIRenderElement* element, float targetalpha, DWORD targettime )
	: ioRenderElementAnimationFrame( element, targettime )
	, m_targetalpha( targetalpha )
{
}

ioRenderElementAnimationFrameAlpha::ioRenderElementAnimationFrameAlpha( const RenderElementVec& elements, float targetalpha, DWORD targettime )
	: ioRenderElementAnimationFrame( elements, targettime )
	, m_targetalpha( targetalpha )
{
}

ioRenderElementAnimationFrameAlpha::~ioRenderElementAnimationFrameAlpha()
{
}

//----------------------------------------------------------------------------------
/*
������ ��Ҹ� update�ϴ� �Լ�
*/
bool	ioRenderElementAnimationFrameAlpha::UpdateElement( DWORD dt, ioRenderElementAnimationFrame::ElementInfo& info, int index )
{	
	if ( info.m_speed == 0.0f ) 
	{
		info.m_element->SetAlpha( m_targetalpha );
		return true;
	}

	float curalpha = info.m_element->GetAlpha();
	float dist = m_targetalpha - curalpha;
	if ( fabsf( dist ) < ALMOST_ZERO ) {
		return true;
	}

	float speed = info.m_speed * ( dt * FLOAT0001 );
	float distance = std::min<float>( fabsf( dist ), fabsf( speed ) );
	info.m_element->SetAlpha( curalpha + ( distance * ( ( speed < 0.0f ) ? -FLOAT1 : FLOAT1 ) ) );
	return false;
}

//----------------------------------------------------------------------------------
/*
�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
*/
void	ioRenderElementAnimationFrameAlpha::SetTargetValue()
{
	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter ) {
		(*iter).m_element->SetAlpha( m_targetalpha );
	}
}

//----------------------------------------------------------------------------------
/*
�������� ������ ��
*/
void	ioRenderElementAnimationFrameAlpha::OnBegin()
{
	ioRenderElementAnimationFrame::OnBegin();

	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter )
	{	
		float curalpha = iter->m_element->GetAlpha();
		float dist = m_targetalpha - curalpha;
		if ( m_targettime == 0 || fabsf( dist ) < ALMOST_ZERO ) {
			iter->m_speed = 0.0f;
		} else {
			iter->m_speed = dist / ( m_targettime * FLOAT0001 );
		}
	}
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameSize
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/*
���� / �Ҹ�
*/
ioRenderElementAnimationFrameSize::ioRenderElementAnimationFrameSize( ioUIRenderElement* element, const D3DXVECTOR2& targetsize, DWORD targettime )
	: ioRenderElementAnimationFrame( element, targettime )	
	, m_targetsize( targetsize )
{
	m_cursizes.resize( 1, D3DXVECTOR2( 0.0f, 0.0f ) );
}

ioRenderElementAnimationFrameSize::ioRenderElementAnimationFrameSize( const RenderElementVec& elements, const D3DXVECTOR2& targetsize, DWORD targettime )
	: ioRenderElementAnimationFrame( elements, targettime )
	, m_targetsize( targetsize )
{
	m_cursizes.resize( elements.size(), D3DXVECTOR2( 0.0f, 0.0f ) );
}

ioRenderElementAnimationFrameSize::~ioRenderElementAnimationFrameSize()
{
}

//----------------------------------------------------------------------------------
/*
������ ��Ҹ� update�ϴ� �Լ�
*/
bool	ioRenderElementAnimationFrameSize::UpdateElement( DWORD dt, ioRenderElementAnimationFrame::ElementInfo& info, int index )
{	
	if ( info.m_speed == 0.0f ) 
	{
		info.m_element->SetSize( m_targetsize.x, m_targetsize.y );
		return true;
	}

	D3DXVECTOR2 cursize = m_cursizes[ index ];
	D3DXVECTOR2 vec = m_targetsize - cursize;
	if ( ioMath::IsEqual( vec, D3DXVECTOR2( 0.0f, 0.0f ) ) ) {
		return true;
	}	

	float tickspeed = info.m_speed * ( dt * FLOAT0001 );
	float distance = std::min<float>( D3DXVec2Length( &vec ), tickspeed );
	D3DXVec2Normalize( &vec, &vec );
	vec = vec * distance;

	m_cursizes[ index ].x = cursize.x + vec.x;
	m_cursizes[ index ].y = cursize.y + vec.y;
	info.m_element->SetSize( m_cursizes[ index ].x, m_cursizes[ index ].y );
	return false;
}

//----------------------------------------------------------------------------------
/*
�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
*/
void	ioRenderElementAnimationFrameSize::SetTargetValue()
{
	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter ) {
		(*iter).m_element->SetSize( m_targetsize.x, m_targetsize.y );
	}
}

//----------------------------------------------------------------------------------
/*
�������� ������ ��
*/
void	ioRenderElementAnimationFrameSize::OnBegin()
{
	ioRenderElementAnimationFrame::OnBegin();

	int cnt = 0;
	ElementInfos::iterator iter = m_elements.begin();
	ElementInfos::iterator eiter = m_elements.end();
	for ( ; iter != eiter; ++iter, ++cnt )
	{	
		m_cursizes[ cnt ] = iter->m_element->GetSize();
		D3DXVECTOR2 vec = m_targetsize - m_cursizes[ cnt ];
		if ( m_targettime == 0 || ioMath::IsEqual( vec, D3DXVECTOR2( 0.0f, 0.0f ) ) ) 
		{
			iter->m_speed = 0.0f;
		}
		else 
		{
			float distance = D3DXVec2Length( &vec );
			iter->m_speed = distance / ( m_targettime * FLOAT0001 );
		}
	}
}
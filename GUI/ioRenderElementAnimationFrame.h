
//----------------------------------------------------------------------------------
/*
Filename	: ioRenderElementAnimationFrame.h
Author		: megamegy
Date		: 2013.10.31
Desc		: 
*/
//----------------------------------------------------------------------------------
#ifndef _IORENDERELEMENTANIMATIONFRAME_H_
#define _IORENDERELEMENTANIMATIONFRAME_H_

class ioUIRenderElement;
typedef std::vector< ioUIRenderElement* > RenderElementVec;

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioUIAnimationFrame
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioRenderElementAnimationFrame
{
public:

	//----------------------------------------------------------------------------------
	/*
	ElementInfo : ����� ���� �ʿ��Ѱ͵�
	*/
	struct ElementInfo
	{
		ElementInfo( ioUIRenderElement* element ) 
			: m_element( element ), m_speed( 0.0f ) {}

		ioUIRenderElement*		m_element;
		float					m_speed;
	};
	typedef std::vector< ElementInfo > ElementInfos;

public:

	//----------------------------------------------------------------------------------
	/*
	���� / �Ҹ�
	*/
	ioRenderElementAnimationFrame( ioUIRenderElement* element, DWORD targettime );
	ioRenderElementAnimationFrame( const RenderElementVec& elements, DWORD targettime );
	virtual ~ioRenderElementAnimationFrame();
	
	//----------------------------------------------------------------------------------
	/*
	Update : ó���� �Ϸ�Ǿ��ٸ� true�� �����Ѵ�.
	*/
	virtual bool				Update();

	//----------------------------------------------------------------------------------
	/*
	�������� ������ ��
	*/
	virtual void				OnBegin();
	virtual void				OnEnd() {}
	
protected:

	//----------------------------------------------------------------------------------
	/*
	������ ��Ҹ� update�ϴ� �Լ�
	*/
	virtual bool				UpdateElement( DWORD dt, ElementInfo& info, int index ) = 0;

	//----------------------------------------------------------------------------------
	/*
	�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
	*/
	virtual void				SetTargetValue() {}

protected:

	ElementInfos				m_elements;
	
	DWORD						m_lastupdatetime;
	DWORD						m_targettime;
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameEmpty
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioRenderElementAnimationFrameEmpty : public ioRenderElementAnimationFrame
{
public:

	//----------------------------------------------------------------------------------
	/*
	���� / �Ҹ�
	*/
	ioRenderElementAnimationFrameEmpty( ioUIRenderElement* element, DWORD targettime );
	ioRenderElementAnimationFrameEmpty( const RenderElementVec& elements, DWORD targettime );
	virtual ~ioRenderElementAnimationFrameEmpty();
	
	//----------------------------------------------------------------------------------
	/*
	Update : ó���� �Ϸ�Ǿ��ٸ� true�� �����Ѵ�.
	*/
	virtual bool	Update();

	//----------------------------------------------------------------------------------
	/*
	�������� ������ ��
	*/
	virtual void	OnBegin();

protected:

	//----------------------------------------------------------------------------------
	/*
	������ ��Ҹ� update
	*/
	virtual bool	UpdateElement( DWORD dt, ElementInfo& info, int index ) { return false; }

protected:

	DWORD			m_acctime;
};


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameScale
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioRenderElementAnimationFrameScale : public ioRenderElementAnimationFrame
{
public:

	//----------------------------------------------------------------------------------
	/*
	���� / �Ҹ�
	*/
	ioRenderElementAnimationFrameScale( ioUIRenderElement* element, const D3DXVECTOR2& targetscale, DWORD targettime );
	ioRenderElementAnimationFrameScale( const RenderElementVec& elements, const D3DXVECTOR2& targetscale, DWORD targettime );
	virtual ~ioRenderElementAnimationFrameScale();
	
	//----------------------------------------------------------------------------------
	/*
	�������� ������ ��
	*/
	virtual void	OnBegin();

protected:

	//----------------------------------------------------------------------------------
	/*
	������ ��Ҹ� update
	*/
	virtual bool	UpdateElement( DWORD dt, ElementInfo& info, int index );

	//----------------------------------------------------------------------------------
	/*
	�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
	*/
	virtual void	SetTargetValue();

protected:

	D3DXVECTOR2		m_targetscale;
};


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameAlpha
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioRenderElementAnimationFrameAlpha : public ioRenderElementAnimationFrame
{
public:

	//----------------------------------------------------------------------------------
	/*
	���� / �Ҹ�
	*/
	ioRenderElementAnimationFrameAlpha( ioUIRenderElement* element, float targetalpha, DWORD targettime );
	ioRenderElementAnimationFrameAlpha( const RenderElementVec& elements, float targetalpha, DWORD targettime );
	virtual ~ioRenderElementAnimationFrameAlpha();
	
	//----------------------------------------------------------------------------------
	/*
	�������� ������ ��
	*/
	virtual void	OnBegin();

protected:

	//----------------------------------------------------------------------------------
	/*
	������ ��Ҹ� update�ϴ� �Լ�
	*/
	virtual bool	UpdateElement( DWORD dt, ElementInfo& info, int index );

	//----------------------------------------------------------------------------------
	/*
	�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
	*/
	virtual void	SetTargetValue();

protected:

	float			m_targetalpha;
};

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
ioRenderElementAnimationFrameSize
*/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
class ioRenderElementAnimationFrameSize : public ioRenderElementAnimationFrame
{
	// �̰� size�� int������ ������ �Ǽ� ���� �̷��� ����ؾ��Ѵ�...;;;
	typedef std::vector< D3DXVECTOR2 > ElementSubInfos;

public:

	//----------------------------------------------------------------------------------
	/*
	���� / �Ҹ�
	*/
	ioRenderElementAnimationFrameSize( ioUIRenderElement* element, const D3DXVECTOR2& targetsize, DWORD targettime );
	ioRenderElementAnimationFrameSize( const RenderElementVec& elements, const D3DXVECTOR2& targetsize, DWORD targettime );
	virtual ~ioRenderElementAnimationFrameSize();
	
	//----------------------------------------------------------------------------------
	/*
	�������� ������ ��
	*/
	virtual void		OnBegin();

protected:

	//----------------------------------------------------------------------------------
	/*
	������ ��Ҹ� update�ϴ� �Լ�
	*/
	virtual bool		UpdateElement( DWORD dt, ElementInfo& info, int index );

	//----------------------------------------------------------------------------------
	/*
	�ð��� 0�ϰ�� target������ ��� �����ϴ� �Լ�
	*/
	virtual void		SetTargetValue();

protected:
		
	D3DXVECTOR2			m_targetsize;

	ElementSubInfos		m_cursizes;
};

#endif // _IORENDERELEMENTANIMATIONFRAME_H_
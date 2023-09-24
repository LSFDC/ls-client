#pragma once

/*
	Ǯ�ٿ� �޴��� ���콺 ����, Ŭ�� ���� ���콺 �� Ű���� � ���� �̺�Ʈ ��ü�� ����
*/

class ioPullDownList;
class PullDownEvent
{
public:
	// ���콺 �̺�Ʈ Ÿ�� �߰�
	enum PullDownEventType
	{		
		PULLDOWNEVENT_NONE = 0,
		PULLDOWNEVENT_DOWN,
		PULLDOWNEVENT_OVER,
		PULLDOWNEVENT_LEAVE 
	};

private:
	friend class ioPullDownList;
	ioPullDownList* pEventCaller;
	PullDownEventType m_pdeType;

public:
	enum
	{		
		NONE_SELECT  = -1,
	};

	//�̺�Ʈ�� �߻���Ų Ǯ �ٿ� ����Ʈ ����
	const ioPullDownList* GetEventCaller() const { return pEventCaller; }

	const PullDownEventType GetEventType() const { return m_pdeType; }
	void SetEventType( const PullDownEventType newType ) { m_pdeType = newType; }

	PullDownEvent( ioPullDownList* pCaller )
	{
		m_pdeType		= PULLDOWNEVENT_NONE;
		pEventCaller	= pCaller;
	}
	virtual ~ PullDownEvent(){};
};

/*
	Ǯ�ٿ� �޴� ������ ���콺 ���� �̺�Ʈ ��ü
*/

class OnOverPullDownEvent : public PullDownEvent
{
public:
	int iOverIdx;
	int iOverImageIdx;

	OnOverPullDownEvent()	: PullDownEvent( NULL )
	{
		iOverIdx		= NONE_SELECT;
		iOverImageIdx	= NONE_SELECT;
	}

	OnOverPullDownEvent( ioPullDownList* pCaller ) : PullDownEvent( pCaller )
	{
		iOverIdx		= NONE_SELECT;
		iOverImageIdx	= NONE_SELECT;
	}
	virtual ~ OnOverPullDownEvent(){};

	bool IsPulldownMouseOver() const
	{
		if( NONE_SELECT != iOverIdx && NONE_SELECT == iOverImageIdx )
			return true;

		return false;
	}

	bool IsImageMouseOver() const
	{
		if( NONE_SELECT != iOverIdx && NONE_SELECT != iOverImageIdx )
			return true;

		return false;
	}
};

/*
	Ǯ�ٿ� �޴� ������ ���콺 ��Ŭ�� �̺�Ʈ ��ü
*/

class ClickPullDownEvent : public PullDownEvent
{
public:
	int iClickIdx;
	int iClickImageIdx;
	int iSelectChar;

	ClickPullDownEvent()	: PullDownEvent( NULL )
	{
		iClickIdx		= NONE_SELECT;
		iClickImageIdx	= NONE_SELECT;
		iSelectChar		= -1;
	}

	ClickPullDownEvent( ioPullDownList* pCaller ) : PullDownEvent( pCaller )
	{
		iClickIdx		= NONE_SELECT;
		iClickImageIdx	= NONE_SELECT;
		iSelectChar		= -1;
	}
	virtual ~ ClickPullDownEvent(){};

	bool IsClickNone()
	{
		if( NONE_SELECT == iClickIdx && NONE_SELECT == iClickImageIdx )
			return true;

		return false;
	}

	bool IsPulldownClick() const
	{
		if( NONE_SELECT != iClickIdx && NONE_SELECT == iClickImageIdx )
			return true;

		return false;
	}

	bool IsImageClick() const
	{
		if( NONE_SELECT != iClickIdx && NONE_SELECT != iClickImageIdx )
			return true;

		return false;
	}
};

/*
	Ǯ�ٿ� �޴� ������ ���콺 ���� �̺�Ʈ ��ü
*/

class OnLeavePullDownEvent : public PullDownEvent
{
public:
	int iLeaveIdx;
	int iLeaveImageIdx;

	OnLeavePullDownEvent()	: PullDownEvent( NULL )
	{
		iLeaveIdx		= NONE_SELECT;
		iLeaveImageIdx	= NONE_SELECT;
	}

	OnLeavePullDownEvent( ioPullDownList* pCaller ) : PullDownEvent( pCaller )
	{
		iLeaveIdx		= NONE_SELECT;
		iLeaveImageIdx	= NONE_SELECT;
	}
	virtual ~ OnLeavePullDownEvent(){};

	bool IsPulldownMouseLeave() const
	{
		if( NONE_SELECT != iLeaveIdx )
			return true;

		return false;
	}
	
	bool IsImageMouseLeave() const
	{
		if( NONE_SELECT != iLeaveImageIdx )
			return true;

		return false;
	}
};

/*
	Ǯ�ٿ� �޴� ������ ���콺 �̺�Ʈ ó�� �������̽�
*/

class PullDownEventListener
{
public:
	virtual void iwm_pulldown_event( const PullDownEvent& Event ) PURE;
};
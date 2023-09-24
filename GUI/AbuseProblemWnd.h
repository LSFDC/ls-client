#pragma once

class SP2Packet;

class AbuseProblemWnd : public ioWnd
{
protected:
	enum AbuseType
	{
		AT_KEY,
		AT_DAMAGE,
	};

	enum QuizOperator
	{
		QP_ADD,
		QP_MINUS,
		QP_MULTIPLY,
		QP_DIVIDE,
	};

	enum
	{
		ID_OK = 1,
		ID_ANSWER_EDIT = 2,
	};

protected:
	AbuseType m_AbuseType;
	CEncrypt<DWORD> m_dwQuizStartTime;
	CEncrypt<DWORD> m_dwQuizLimitTime;

	CEncrypt<int> m_iFirstOperand;
	CEncrypt<int> m_iSecondOperand;
	QuizOperator m_Operator;

	CEncrypt<int> m_iCurQuizChance;
	CEncrypt<int> m_iMaxQuizChance;

	CEncrypt<int> m_iPreQuizFirst;
	CEncrypt<int> m_iPreQuizSecond;
	int m_iSendedAnwser;

	bool m_bNowOpenSpace;
	ioEdit *m_pPreEdit;

public:
	void SetAbuseAnnounce( SP2Packet &rkPacket, bool bOpenSpace );
	bool SetAbuseQuiz( SP2Packet &rkPacket );

protected:
	void SetAnswerEditFocus();
	void SendQuizAnswer();

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	AbuseProblemWnd();
	virtual ~AbuseProblemWnd();
};
//////////////////////////////////////////////////////////////////////////
// [1/12/2012 ��ȣ] - �κ񿡼� Ȱ��ȭ�Ǵ� ������� �κ��� ��� ����� ��Ȱ���ȴ�.
class LobbyLockProblemWnd : public ioWnd
{
public:
	enum
	{
		ID_OK = 1,
		ID_ANSWER_EDIT = 2,
	};

public:
	enum LockType
	{
		LT_NONE = 0,
		LT_MACRO,
		LT_HACK,
		LT_ABUSE,
		LT_SPEED,
	};

protected:
	CEncrypt<DWORD> m_dwLockType;

protected:
	CEncrypt<int> m_iFirstOperand;
	CEncrypt<int> m_iSecondOperand;
	
protected:
	ioEdit *m_pPreEdit;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	void CheckAnswer();

public:
	void SetLockType( DWORD dwLockType );

public:
	LobbyLockProblemWnd();
	virtual ~LobbyLockProblemWnd();
};
//////////////////////////////////////////////////////////////////////////
// [1/13/2012 ��ȣ] - ���� �÷����� ��ũ�� �Ű� & ��ũ�� ���ÿ� �ɸ��� ���� �÷��� ���� �� ���� ���濡�� Ȱ��ȭ
class BattleLockProblemWnd : public ioWnd
{
public:
	enum
	{
		ID_OK = 1,
		ID_ANSWER_EDIT = 2,
	};

public:
	enum LockType
	{
		LT_NONE = 0,
		LT_MACRO,
		LT_HACK,
		LT_ABUSE,
		LT_SPEED,
		LT_MACRO_EXTENSION,
	};

protected:
	CEncrypt<DWORD> m_dwLockType;

protected:
	CEncrypt<int> m_iFirstOperand;
	CEncrypt<int> m_iSecondOperand;

protected:
	CEncrypt<int> m_iCurQuizChance;
	CEncrypt<int> m_iMaxQuizChance;
	CEncrypt<DWORD> m_dwQuizStartTime;
	CEncrypt<DWORD> m_dwQuizLimitTime;

protected:
	ioEdit *m_pPreEdit;

public:
	virtual void iwm_show();
	virtual void iwm_hide();
	virtual void iwm_command( ioWnd *pWnd, int cmd, DWORD param );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

protected:
	void CheckAnswer();

public:
	void SetLockType( DWORD dwLockType );

public:
	BattleLockProblemWnd();
	virtual ~BattleLockProblemWnd();
};

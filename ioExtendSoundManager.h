#pragma once

class ExSound
{
public:
	enum ExSoundType
	{
		EST_NONE				= 0,	//���� ����
		EST_COMPANY				= 1,	//ȸ��ΰ� ����
		EST_LOGIN				= 2,	//���� �κ� ����� ����
		EST_LOGOUT				= 3,	//���� ���� �˾� �㋚ ����
		EST_SHOP_ENTER			= 4,	//���� �����
		EST_SHOP_BUY_SUCCESS	= 5,	//�������� ������ ���� ������
		EST_PRESENT_POPUP		= 6,	//���� �����찡 ������
		EST_REINFORCE_SUCCESS	= 7,	//��ȭ(����/����) ������
		EST_REINFORCE_FAIL		= 8,	//��ȭ(����/����) ���н�
		EST_PET_MIX_SUCESS		= 9,	//�� �ռ� ������
		EST_GASHAPON_GET		= 10,	//��í ȹ���
		EST_QUEST_COMPLETE		= 11,	//����Ʈ �Ϸ� �˾��� ������
		EST_MISSION_COMPLETE	= 12,	//�̼� �Ϸ� �˾��� ������
		EST_HQ_ENTER			= 13,	//�Ʒü� �����
		EST_GUILD_HQ_ENTER		= 14,	//��庻�� �����
		EST_HOME_ENTER			= 15,	//���κ��� �����
		EST_WIN					= 16,	//�¸�
		EST_LOSE				= 17,	//�й�
		EST_AWARD				= 18,	//�û��
		EST_PRESENT_BOX_RESULT	= 19,	//������ �ޱ� �� ���â
	};

	ExSoundType		m_SoundType;
	ioHashString	m_SoundName;
	bool			m_bThread;
	bool			m_bIgnoreNextSound;	//��� ���� �� ���� ���尡 ��� ���� ���ϵ��� ��(�� �켱 ����)
	bool			m_bCutIgnore;		//��� ���� �� �Ʒ� m_bCutPrevSound�� �� �ڸ��� ��(�� �켱 ����)
	bool			m_bCutPrevSound;	//������ ������� ���尡 �ִٸ� �ڸ��� ���(�� ����)
	

	ExSound()
	{
		m_SoundType			= EST_NONE; 
		m_bThread			= false;
		m_bIgnoreNextSound	= false;
		m_bCutIgnore		= false;
		m_bCutPrevSound		= false;
	}
	virtual ~ExSound(){};
};
typedef std::vector<ExSound> ExSoundList;

class ioExtendSoundManager : public Singleton< ioExtendSoundManager >
{
protected:
	ExSoundList		m_ExSoundList;
	UniqueObjID		m_CurrSoundID;
	ioHashString	m_CurrSoundName;
	bool			m_bIgnoreNextSound;
	bool			m_bCutIgnore;

public:
	static ioExtendSoundManager& GetSingleton();

public:
	virtual void Initialize();

	bool IsSound( ExSound::ExSoundType eSound );
	void PlaySound( ExSound::ExSoundType eSound );

public:
	ioExtendSoundManager();
	virtual ~ioExtendSoundManager();
};

#define g_ExSoundMgr ioExtendSoundManager::GetSingleton()
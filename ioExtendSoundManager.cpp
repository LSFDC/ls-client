#include "stdafx.h"
#include "ioExtendSoundManager.h"

template<> ioExtendSoundManager* Singleton< ioExtendSoundManager >::ms_Singleton = 0;
ioExtendSoundManager& ioExtendSoundManager::GetSingleton()
{
	return Singleton< ioExtendSoundManager >::GetSingleton();
}

ioExtendSoundManager::ioExtendSoundManager() : m_CurrSoundID( -1 ), m_bCutIgnore( false ), m_bIgnoreNextSound( false )
{
}

ioExtendSoundManager::~ioExtendSoundManager()
{
}

void ioExtendSoundManager::Initialize()
{
	m_ExSoundList.clear();

	ioINILoader_e kLoader( "config/sp2_ex_sound.ini" );
	kLoader.SetTitle_e( "common" );
	int nCount = kLoader.LoadInt_e("max_sound", 0 );

	char szBuf[MAX_PATH];
	for( int i = 0; i < nCount; ++i )
	{
		wsprintf_e( szBuf, "sound%d", i+1 );
		kLoader.SetTitle( szBuf );

		ExSound Sound;
		kLoader.LoadString_e( "sound_name", "", szBuf, MAX_PATH );
		Sound.m_SoundName			= szBuf;
		Sound.m_SoundType			= static_cast<ExSound::ExSoundType>( kLoader.LoadInt_e( "sound_type", 0 ) );
		Sound.m_bThread				= kLoader.LoadBool_e( "thread_sound", false );
		Sound.m_bIgnoreNextSound	= kLoader.LoadBool_e( "ignore_next_sound", false );
		Sound.m_bCutPrevSound		= kLoader.LoadBool_e( "cut_prev_sound", true );
		Sound.m_bCutIgnore			= kLoader.LoadBool_e( "cut_ignore", false );

		m_ExSoundList.push_back( Sound );
	}
}

bool ioExtendSoundManager::IsSound( ExSound::ExSoundType eSound )
{
	if( !Setting::IsPlayExtendSound() )
		return false;

	for( auto iter = m_ExSoundList.begin(); iter != m_ExSoundList.end(); ++iter )
	{
		const ExSound& Sound = *iter;
		if( Sound.m_SoundType == eSound )
			return true;
	}

	return false;
}

void ioExtendSoundManager::PlaySound( ExSound::ExSoundType eSound )
{
	//����� ���� �Ŵ����� ���ٸ� �÷��� ���� ����
	if( ioSoundManager::GetSingletonPtr() == NULL )
		return;

	//���� ��� �ɼ��� ���������� �÷��� ���� ����
	if( !Setting::IsPlayExtendSound() )
		return;

	//����� ���� ����Ʈ�� ����
	ExSoundList Buffer;
	for( auto iter = m_ExSoundList.begin(); iter != m_ExSoundList.end(); ++iter )
	{
		const ExSound& Sound = *iter;
		if( Sound.m_SoundType == eSound )
		{
			Buffer.push_back( Sound );
		}
	}

	//��� �� ���尡 ������ �н�~
	if( Buffer.empty() )
		return;

	//�������� ����� ���� �� �ϳ��� ���
	srand( time( NULL ) );
	int Index = rand() % (int)Buffer.size();
	if( !COMPARE( Index, 0, (int)Buffer.size() ) )
		Index = 0;

	//��� ���� ���尡 ���� ��� �� ���带 �����ϴ� ����� �ִٸ� ����
	if( !m_CurrSoundName.IsEmpty() )
	{	
		if( g_SoundMgr.IsPlayingSound( m_CurrSoundName, m_CurrSoundID ) && m_bIgnoreNextSound )
			return;
	}

	//������ ������� ���尡 �ִٸ� �ڸ��� ���
	if( Buffer[Index].m_bCutPrevSound && !m_CurrSoundName.IsEmpty() && !m_bCutIgnore )
	{
		g_SoundMgr.StopSound( m_CurrSoundName, m_CurrSoundID );
	}

	if( Buffer[Index].m_bThread )
		m_CurrSoundID	= g_SoundMgr.PlaySound( Buffer[Index].m_SoundName, DSBVOLUME_MAX, PSM_THREAD );
	else
		m_CurrSoundID	= g_SoundMgr.PlaySound( Buffer[Index].m_SoundName, DSBVOLUME_MAX, PSM_NONE );

	m_CurrSoundName		= Buffer[Index].m_SoundName;
	m_bCutIgnore		= Buffer[Index].m_bCutIgnore;
	m_bIgnoreNextSound	= Buffer[Index].m_bIgnoreNextSound;
}
#include "stdafx.h"

#include "BlockDefine.h"
#include "ioBlockSetupManager.h"
#include "ioBlockWorld.h"
#include "ioBlockEventParameter.h"
#include "ioUserBlockStorage.h"

#include "../ioPlayStage.h"
#include "../ioBaseChar.h"
#include "../StateClass/ioConstructHouseSpecialState.h"
#include "../ioEtcItem.h"
#include "../ioUserEtcItem.h"

#include "../GUI/ioHousingBlockSetupWnd.h"

template<> ioBlockSetupManager* Singleton< ioBlockSetupManager >::ms_Singleton = 0;

ioBlockSetupManager::ioBlockSetupManager()
{
}

ioBlockSetupManager::~ioBlockSetupManager()
{
}

ioBlockSetupManager& ioBlockSetupManager::GetSingleton()
{
	return Singleton< ioBlockSetupManager >::GetSingleton();
}

void ioBlockSetupManager::Initialize( ioPlayStage* pStage, BlockModeType eType )
{
	m_pStage				= pStage;
	m_BlockModeType			= eType;
	m_nSetUpItemListIndex	= 0;

	m_SetUpAdminUser.Clear();

	ioINILoader_e kLoader( "config/housing/sp2_block.ini" );
	kLoader.SetTitle_e( "common" );

	m_nBlockLimitCount = kLoader.LoadInt( "block_limit", 3000 );
}

bool ioBlockSetupManager::IsSetUpState()
{
	ioBaseChar* pOwner = m_pStage->GetBaseChar( g_MyInfo.GetPublicID() );
	if( pOwner && pOwner->GetState() == CS_CONSTRUCT_HOUSE )
	{
		return true;
	}

	return false;
}

ioConstructHouseSpecialState* ioBlockSetupManager::GetAdminState()
{
	ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
	if( pOwner )
	{
		ioConstructHouseSpecialState* pState = dynamic_cast<ioConstructHouseSpecialState*>( pOwner->GetSpecialState() );
		if( pState )
		{
			return pState;
		}
	}

	return NULL;
}

void ioBlockSetupManager::HousingSetUpProcess()
{
	if( IsSetUpState() )
		EndSetUpState();
	else
		BeginSetUpState();
}

void ioBlockSetupManager::BeginSetUpState()
{
	if( g_GUIMgr.IsShow( BRIEFING_WND ) ) 
		return;

	if( !m_pStage )
		return;

	ioBaseChar* pOwner = m_pStage->GetOwnerChar();
	if( !pOwner )
	{
		LOG.PrintTimeAndLog( 0, "%s pOwner == NULL.", __FUNCTION__ );
		return;
	}

	if( !pOwner->IsNeedProcess() )
	{
		LOG.PrintTimeAndLog( 0, "%s pOwner Not NeedProess", __FUNCTION__ );
		return;
	}

	if( pOwner->GetState() == CS_FISHING || pOwner->GetState() == CS_EXCAVATING )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
		return;
	}

	// ĳ���� ���� üũ
	if( pOwner->GetState() != CS_DELAY || pOwner->IsCatchMode() || pOwner->IsPrisonerMode() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
		return;
	}
	
	// Buff üũ
	if( pOwner->HasBuff( BT_LIMIT_MOVE ) || pOwner->HasBuff( BT_LIMIT_ACTION ) )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
		return;
	}

	// ������ üũ
	if( pOwner->IsHasCrownByAllMode() )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
		return;
	}

	// ���� üũ
	if( pOwner->GetCreator() == NULL || pOwner->GetCreator()->GetBlockWorld() == NULL )
	{
		g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
		return;
	}

	pOwner->SetState( CS_CONSTRUCT_HOUSE );

	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_CONSTRUCT_MODE );
	kPacket << (byte)m_BlockModeType;
	kPacket << (byte)CONSTRUCT_ON;
	TCPNetwork::SendToServer( kPacket );
}

void ioBlockSetupManager::ApplySetUpState()
{
	TCPNetwork::MouseBusy( false );

	ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
	if( pOwner )
	{
		if( pOwner->IsNeedProcess() )
		{
			ioConstructHouseSpecialState* pState = dynamic_cast<ioConstructHouseSpecialState*>( pOwner->GetSpecialState() );
			if( pState )
			{
				pState->ChangeToSetUp( pOwner, pOwner->GetCreator() );
				if( m_SetUpAdminUser == g_MyInfo.GetPublicID() )
				{
					ioHousingBlockSetupWnd* pWnd = dynamic_cast<ioHousingBlockSetupWnd*>( g_GUIMgr.FindWnd( HOUSING_BLOCK_SETUP_WND ) );
					if( pWnd )
					{
						pWnd->SetBlockModeType( m_BlockModeType );
						pWnd->ShowWnd();
					}

					g_GUIMgr.HideWnd( HELP_WND );
					g_GUIMgr.HideWnd( SKILL_LIST_WND );
					g_GUIMgr.HideWnd( ACCESSORY_SKILL_LIST_WND );
					g_GUIMgr.HideWnd( MINI_SOLDIER_SELECT_WND );					
					g_GUIMgr.HideWnd( MESSENGER_WND );
				}
				return;				
			}
		}
		else
		{
			pOwner->SetState( CS_CONSTRUCT_HOUSE );
			return;
		}
	}

	//��ġ���� ��ȯ�� �ȉ�ٸ� ���� ��ȯ
	g_BlockSetupManager.EndSetUpState();
}

void ioBlockSetupManager::EndSetUpState()
{
	if( m_pStage->HasCameraFixedStyle( CAMERA_STYLE_HOUSING ) )
		m_pStage->ClearCameraModeFixed( CAMERA_STYLE_HOUSING );

	ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
	if( pOwner && pOwner->IsNeedProcess() && pOwner->GetState() == CS_CONSTRUCT_HOUSE )
	{
		TCPNetwork::MouseBusy( true );
		SP2Packet kPacket( CTPK_CONSTRUCT_MODE );
		kPacket << (byte)m_BlockModeType;
		kPacket << (byte)CONSTRUCT_OFF;
		TCPNetwork::SendToServer( kPacket );
	}
}

void ioBlockSetupManager::ApplyEndSetUpState()
{
	TCPNetwork::MouseBusy( false );

	if( m_SetUpAdminUser == g_MyInfo.GetPublicID() )
	{
		ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
		if( pOwner )
		{
			pOwner->SetState( CS_DELAY );
		}

		g_GUIMgr.ShowWnd( HELP_WND );
		g_GUIMgr.ShowWnd( SKILL_LIST_WND );
		g_GUIMgr.ShowWnd( ACCESSORY_SKILL_LIST_WND );
		g_GUIMgr.ShowWnd( MINI_SOLDIER_SELECT_WND );
	}
	else
	{
		ioBaseChar* pOwner = m_pStage->GetBaseChar( g_MyInfo.GetPublicID() );
		if( pOwner && pOwner->GetState() == CS_CONSTRUCT_HOUSE )
		{
			pOwner->SetState( CS_DELAY );
		}
	}
}

void ioBlockSetupManager::SendSetUpBlock( int nItemCode, int x, int y, int z, byte rotate )
{
	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_CONSTRUCT_BLOCK );
	kPacket << (byte)m_BlockModeType;
	kPacket << nItemCode;
	kPacket << x + (z * g_BlockManager.GetTileMax() );
	kPacket << y;
	kPacket << rotate;
	TCPNetwork::SendToServer( kPacket );
}

void ioBlockSetupManager::SendRemoveBlock( __int64 nUniqueIndex )
{
	__int64 nUniqIndex64 = nUniqueIndex;
	TCPNetwork::MouseBusy( true );
	SP2Packet kPacket( CTPK_RETRIEVE_BLOCK );
	kPacket << (byte)m_BlockModeType;
	kPacket << nUniqIndex64;
	TCPNetwork::SendToServer( kPacket );
}

void ioBlockSetupManager::ChangeToSetUp()
{
	ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
	if( pOwner )
	{
		ioConstructHouseSpecialState* pState = dynamic_cast<ioConstructHouseSpecialState*>( pOwner->GetSpecialState() );
		if( pState )
			pState->ChangeToSetUp( pOwner, m_pStage );
	}
}

void ioBlockSetupManager::ChangeToRemove()
{
	ioBaseChar* pOwner = m_pStage->GetBaseChar( m_SetUpAdminUser );
	if( pOwner )
	{
		ioConstructHouseSpecialState* pState = dynamic_cast<ioConstructHouseSpecialState*>( pOwner->GetSpecialState() );
		if( pState )
			pState->ChangeToRemove( pOwner, m_pStage );
	}
}

bool ioBlockSetupManager::ProcessTCPPacket( SP2Packet &rkPacket )
{
	switch( rkPacket.GetPacketID() )
	{
	case STPK_CONSTRUCT_MODE:
		OnSetUpState( rkPacket );
		return true;
	case STPK_CONSTRUCT_BLOCK:
		OnSetUpBlock( rkPacket );
		return true;
	case STPK_RETRIEVE_BLOCK:
		OnRemoveBlock( rkPacket );
		return true;
	}

	return false;
}

bool ioBlockSetupManager::ProcessUDPPacket( SP2Packet &rkPacket )
{
	return false;
}

void ioBlockSetupManager::Process()
{
}

void ioBlockSetupManager::OnSetUpState( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eModeType;
	rkPacket >> eModeType;

	byte eResult;
	rkPacket >> eResult;

	switch( eResult )
	{
	case CONSTRUCT_MODE_CHANGE_SUCCESS:
		{
			byte eType;
			rkPacket >> eType;

			ioHashString szName;
			rkPacket >> szName;

			ioBaseChar* pChar = NULL;
			if( m_pStage )
				pChar = m_pStage->GetBaseChar( szName );

			m_SetUpAdminUser = szName;

			if( pChar )
			{
				switch( eType )
				{
				case CONSTRUCT_ON:
					{
						ApplySetUpState();
					}
					break;
				case CONSTRUCT_OFF:
					{
						ApplyEndSetUpState();
						m_SetUpAdminUser.Clear();
					}
					break;
				}
			}
		}
		break;
	case CONSTRUCT_PROGRESSING:
		{
			switch( eModeType )
			{
			case BMT_GUILD:
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				break;
			default:
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
				break;
			}
			ApplyEndSetUpState();
		}
		break;
	case CONSTRUCT_AUTH_NONE:
		{
			switch( eModeType )
			{
			case BMT_GUILD:
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
				break;
			case BMT_PERSONAL:
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
				break;
			default:
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
				break;
			}
			ApplyEndSetUpState();
		}
		break;
	case CONSTRUCT_MODE_EXCEPTION:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(6) );
			ApplyEndSetUpState();
		}
		break;
	}
}

void ioBlockSetupManager::OnSetUpBlock( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eModeType;
	rkPacket >> eModeType;
	
	byte eRes;
	rkPacket >> eRes;

	switch( eRes )
	{
	case CONSTRUCT_BLOCK_SUCCESS:
		{
			ioHashString szName;
			rkPacket >> szName;

			__int64 nUniqueID;
			rkPacket >> nUniqueID;

			DWORD dwCode;
			rkPacket >> dwCode;

			int nPos;
			rkPacket >> nPos;

			int nLevel;
			rkPacket >> nLevel;

			byte Rotate;
			rkPacket >> Rotate;	

			if( nUniqueID != 0 )
			{				
				if( m_pStage )
				{
					//���忡 �� ��ġ
					ioBlockWorld* pWorld = m_pStage->GetBlockWorld();
					if( pWorld )
					{
						TilePos Pos( nPos % g_BlockManager.GetTileMax(), nLevel, nPos / g_BlockManager.GetTileMax() );
						float fRotate = Rotate * ROTATE_VALUE;

						pWorld->CreateBlock( dwCode, Pos, fRotate, nUniqueID );
					}
					
					//�κ��丮���� ���� ����
					ioUserBlockStorage* pStorage = NULL;
					switch( eModeType )
					{
					case BMT_GUILD:
						pStorage = g_App.GetGuildStorage();
						break;
					case BMT_PERSONAL:
						pStorage = g_App.GetHomeStorage();
						break;
					}

					if( pStorage )					
						pStorage->DecreaseItem( dwCode );

					//��ġ��� ���� ����
					ioBaseChar* pChar = m_pStage->GetBaseChar( szName );
					if( pChar )
					{
						ioConstructHouseSpecialState* pState = ToConstructHouseSpecialState( pChar->GetSpecialState() );
						if( pState )
							pState->ChangeToSetUp( pChar, m_pStage );
					}
				}
			}
			else
			{
				g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
				EndSetUpState();
			}
		}
		break;
	case CONSTRUCT_POS_INVALID:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			EndSetUpState();
		}
		break;
	case CONSTRUCT_BLOCK_SHORTAGE:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(3) );
			EndSetUpState();
		}
		break;
	case CONSTRUCT_BLOCK_EXCEPTION:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(4) );
			EndSetUpState();
		}
		break;
	case CONSTRUCT_BLOCK_MAX_COUNT:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(5) );
			EndSetUpState();
		}
		break;
	}
}

void ioBlockSetupManager::OnRemoveBlock( SP2Packet &rkPacket )
{
	TCPNetwork::MouseBusy( false );

	byte eModeType;
	rkPacket >> eModeType;

	byte eRes;
	rkPacket >> eRes;

	switch( eRes )
	{
	case RETRIEVE_BLOCK_SUCCESS:
		{
			ioHashString szName;
			rkPacket>> szName;

			__int64 nUniqueIndex;
			rkPacket >> nUniqueIndex;

			ioBaseChar* pChar = NULL;
			if( m_pStage )
			{
				ioBlock* pBlock = g_BlockManager.GetBlock( nUniqueIndex );
				ioBlockWorld* pWorld = m_pStage->GetBlockWorld();
				if( pWorld && pBlock )
				{					
					//�κ��丮���� ���� ����
					if( pBlock->GetProperty() )
					{
						ioUserBlockStorage* pStorage = NULL;
						switch( eModeType )
						{
						case BMT_GUILD:
							pStorage = g_App.GetGuildStorage();
							break;
						case BMT_PERSONAL:
							pStorage = g_App.GetHomeStorage();
							break;
						}

						if( pStorage )
						{
							pStorage->IncreaseItem( pBlock->GetProperty()->GetCode() );
							pStorage->UpdateInvenBlockItem();
						}
					}

					pWorld->DestroyBlock( pBlock->GetTilePos() );
				}

				pChar = m_pStage->GetBaseChar( szName );
				if( pChar )
				{
					ioConstructHouseSpecialState* pState = ToConstructHouseSpecialState( pChar->GetSpecialState() );
					if( pState )
						pState->ChangeToRemove( pChar, m_pStage );
				}
			}
		}
		break;
	case RETRIEVE_BLOCK_NONE_INFO:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(1) );
			EndSetUpState();
		}
		break;
	case RETRIEVE_BLOCK_EXCEPTION:
		{
			g_GUIMgr.SetMsgBox( MB_OK, NULL, STR(2) );
			EndSetUpState();
		}
		break;
	}
}
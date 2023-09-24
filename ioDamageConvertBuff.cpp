

#include "stdafx.h"

#include "ioDamageConvertBuff.h"

#include "ioBaseChar.h"
#include "ioPlayStage.h"

#include "ioGrowthUpInfo.h"

ioDamageConvertBuff::ioDamageConvertBuff()
{
}

ioDamageConvertBuff::ioDamageConvertBuff( const ioDamageConvertBuff &rhs )
	: ioBuff( rhs ),
	m_fWoundedRecoveryGauge( rhs.m_fWoundedRecoveryGauge ),
	m_fWoundedRecoveryRate( rhs.m_fWoundedRecoveryRate ),
	m_fAttackRecoveryGauge( rhs.m_fAttackRecoveryGauge ),
	m_fAttackRecoveryRate( rhs.m_fAttackRecoveryRate ),
	m_fHPStealRate( rhs.m_fHPStealRate ),
	m_HPStealEffect( rhs.m_HPStealEffect ),
	m_DamageConvertEffect( rhs.m_DamageConvertEffect ),
	m_vAttackTypeList( rhs.m_vAttackTypeList ),
	m_type( rhs.m_type ),
	m_nGaugeAttackCnt( rhs.m_nGaugeAttackCnt),
	m_fAbsoluteRecoveryGague( rhs.m_fAbsoluteRecoveryGague ),
	m_bCheckElementCheckType( rhs.m_bCheckElementCheckType )
{
	m_nCurAttackCnt = 0;
}

ioDamageConvertBuff::~ioDamageConvertBuff()
{
}

void ioDamageConvertBuff::LoadProperty( ioINILoader &rkLoader )
{
	ioBuff::LoadProperty( rkLoader );

	m_fWoundedRecoveryGauge = rkLoader.LoadFloat_e( "wounded_recovery_gauge", 0.0f );
	m_fWoundedRecoveryRate = rkLoader.LoadFloat_e( "wounded_recovery_rate", 0.0f );

	m_fAttackRecoveryGauge = rkLoader.LoadFloat_e( "attack_recovery_gauge", 0.0f );
	m_fAttackRecoveryRate = rkLoader.LoadFloat_e( "attack_recovery_rate", FLOAT1 );

	m_vAttackTypeList.clear();

	int iCnt = rkLoader.LoadInt_e( "attack_cnt", 0 );
	if( iCnt > 0 )
		m_vAttackTypeList.reserve( iCnt );

	char szBuf[MAX_PATH] = "";
	for( int i=0; i < iCnt; ++i )
	{
		wsprintf_e( szBuf, "attack_type%d", i+1 );

		int iType = rkLoader.LoadInt( szBuf, 0 );
		m_vAttackTypeList.push_back( iType );
	}

	m_fHPStealRate = rkLoader.LoadFloat_e( "hp_steal_rate", 0.0f );

	rkLoader.LoadString_e( "damage_convert_hp_effect", "", szBuf, MAX_PATH );
	m_HPStealEffect = szBuf;

	rkLoader.LoadString_e( "damage_convert_effect", "", szBuf, MAX_PATH );
	m_DamageConvertEffect = szBuf;

	//////////////////////////////////////////////////////////////////////////

	m_type = (DamageConvertType)rkLoader.LoadInt_e( "DamageConvertType", DCVT_NORMAL );
	m_nGaugeAttackCnt = rkLoader.LoadInt_e( "GaugeAttackCnt", 1000 );
	m_fAbsoluteRecoveryGague = rkLoader.LoadFloat_e("AbsoluteRecoveryGague", 0.f );

	//////////////////////////////////////////////////////////////////////////

	m_bCheckElementCheckType = rkLoader.LoadBool_e( "check_element_type", true );
}

ioBuff* ioDamageConvertBuff::Clone()
{
	return new ioDamageConvertBuff( *this );
}

void ioDamageConvertBuff::StartBuff( ioBaseChar *pOwner )
{
	ioBuff::StartBuff( pOwner );

	m_nCurAttackCnt = 0;

	pOwner->CheckDamageInfo( true );
}

void ioDamageConvertBuff::StartBuffWithAniTime( ioBaseChar *pOwner, DWORD dwAniTime )
{
	ioBuff::StartBuff( pOwner );

	pOwner->CheckDamageInfo( true );
	m_dwBuffEndTime = FRAMEGETTIME() + GetBuffDuration();
#ifdef ANTIHACK
	SetRecoverBuff();
#endif
}

bool ioDamageConvertBuff::ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime )
{
	if( !ioBuff::ApplyStartBuff( pOwner, dwGlobalTime, dwGapTime ) )
		return false;

	pOwner->CheckDamageInfo( true );
	return true;
}

void ioDamageConvertBuff::ProcessBuff( float fTimePerSec )
{
	switch( m_iOperationType )
	{
	case OT_SWITCH:
		break;
	case OT_DELAY:
	case OT_ACTIVE:
		ioBuff::ProcessBuff( fTimePerSec );
		break;
	case OT_PASSIVE:
		CheckCallingAreaWeapon();
		break;
	}

	if( !IsLive() )
		return;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
	{
		SetReserveEndBuff();
		return;
	}

	if( m_type == DCVT_NORMAL )
	{
		if( pOwner->IsOwnerChar() )
		{
			//Attack
			float fAttackGauge = GetRecoveryGaugeByAttack();
			float fAttackHP = GetRecoveryHPByAttack();

			//Wounded
			float fWoundedGauge = GetRecoveryGaugeByWounded();

			// HP
			if( fAttackHP > 0.0f )
				m_pOwner->RecoveryHP( fAttackHP );

			if( fAttackHP > 0.0f && !m_HPStealEffect.IsEmpty() )
			{
				pOwner->AttachEffect( m_HPStealEffect, NULL );
			}

			//Gauge
			float fRecoveryGauge = fAttackGauge + fWoundedGauge;
			if( fRecoveryGauge > 0.0f )
			{
				ioItem *pOwnerItem = m_pOwner->GetEquipedItem( m_ItemName );

				for( int iSlot=0; iSlot < 4; ++iSlot )
				{
					ioItem *pItem = pOwner->GetEquipedItem( iSlot );
					ioSkill *pSkill = pOwner->GetEquipedSkill( iSlot );

					if( !pItem || !pSkill ) continue;
					//if( pItem == m_pOwnerItem ) continue;
					if( pItem == pOwnerItem ) continue;

					float fCurGauge = pItem->GetCurSkillGuage();
					fCurGauge += fRecoveryGauge;
					pItem->SetCurSkillGauge( fCurGauge );

#ifdef ANTIHACK
					AddRecoverSkillInfo( iSlot, pItem );
#endif
					
				}
			}

			pOwner->InitAttackDamageInfoList();
			pOwner->InitWoundDamageInfoList();
		}
	}
	else if( m_type == DCVT_TYPE1 )
	{
		//Attack
		float fAttackGauge = GetRecoveryGaugeByAttack();
		float fAttackHP = GetRecoveryHPByAttack();

		//Wounded
		float fWoundedGauge = GetRecoveryGaugeByWounded();

		// HP
		if( fAttackHP > 0.0f )
			m_pOwner->RecoveryHP( fAttackHP );

		if( fAttackHP > 0.0f && !m_HPStealEffect.IsEmpty() )
		{
			pOwner->AttachEffect( m_HPStealEffect, NULL );
		}

		ioPlayStage *pStage = m_pOwner->GetCreator();
		ioGrowthNormalUpInfo *pUpInfo = NULL;

		float fGrowth = 0.f;
		pUpInfo = ToNormalUpInfo( GetGrowthUpInfoByType(GT_SKILL_COOL_TIME, m_CreateChar) );
		if( pUpInfo )
			fGrowth = pUpInfo->GetValue(pStage);

		fGrowth = FLOAT1 + (fGrowth/FLOAT100);

		//Gauge
		float fRecoveryGauge = fAttackGauge + fWoundedGauge;
		if( fRecoveryGauge > 0.0f && m_nCurAttackCnt < m_nGaugeAttackCnt )
		{
			for( int iSlot=0; iSlot < 4; ++iSlot )
			{
				ioItem *pItem = pOwner->GetEquipedItem( iSlot );
				ioSkill *pSkill = pOwner->GetEquipedSkill( iSlot );

				if( !pItem || !pSkill ) continue;
				//if( pItem != m_pOwnerItem ) continue;
				if( pItem->GetName() != m_ItemName  )
					continue;

				float fCurGauge = pItem->GetCurSkillGuage();
				fCurGauge += (m_fAbsoluteRecoveryGague/fGrowth);

				pItem->SetCurSkillGauge( fCurGauge );

				m_nCurAttackCnt++;
			}
		}

		pOwner->InitAttackDamageInfoList();
		pOwner->InitWoundDamageInfoList();
	}
}

float ioDamageConvertBuff::GetRecoveryGaugeByAttack()
{
	if( m_fAttackRecoveryGauge <= 0.0f )
		return 0.0f;

	ioBaseChar *pOwner = GetOwner();
	if( !pOwner )
		return 0.0f;

	const DamageInfoList &rkInfoList = pOwner->GetAttackDamageInfoList();

	int iAttackCnt = rkInfoList.size();
	if( iAttackCnt == 0 )
		return 0.0f;

	float fAttackGauge = 0.0f;
	for( int i=0; i < iAttackCnt; ++i )
	{
		float fDamage = rkInfoList[i].m_fDamage;
		int iAttackType = (int)rkInfoList[i].m_AttackType;

		if( CheckElementType( iAttackType ) )
		{
			if( m_fAttackRecoveryRate <= 0.0f )
				fAttackGauge += fDamage * m_fAttackRecoveryGauge;
			else
				fAttackGauge += fDamage * m_fAttackRecoveryRate * m_fAttackRecoveryGauge;
		}
	}

	return fAttackGauge;
}

float ioDamageConvertBuff::GetRecoveryHPByAttack()
{
	if( m_fHPStealRate <= 0.0f )
		return 0.0f;

	const DamageInfoList &rkInfoList = m_pOwner->GetAttackDamageInfoList();

	int iAttackCnt = rkInfoList.size();
	if( iAttackCnt == 0 )
		return 0.0f;

	float fAttackHP = 0.0f;
	for( int i=0; i < iAttackCnt; ++i )
	{
		float fDamage = rkInfoList[i].m_fDamage;
		int iAttackType = (int)rkInfoList[i].m_AttackType;

		if( CheckElementType( iAttackType ) )
			fAttackHP += (fDamage * m_fHPStealRate);
	}

	return fAttackHP;
}

float ioDamageConvertBuff::GetRecoveryGaugeByWounded()
{
	if( m_fWoundedRecoveryGauge <= 0.0f )
		return 0.0f;

	const DamageInfoList &rkInfoList = m_pOwner->GetWoundDamageInfoList();

	int iWoundCnt = rkInfoList.size();
	if( iWoundCnt == 0 )
		return 0.0f;

	if( m_fWoundedRecoveryRate <= 0.0f )
		return m_fWoundedRecoveryGauge * iWoundCnt;

	float fWoundGauge = 0.0f;
	for( int i=0; i < iWoundCnt; ++i )
	{
		float fDamage = rkInfoList[i].m_fDamage;
		int iAttackType = (int)rkInfoList[i].m_AttackType;

		if( CheckElementType( iAttackType ) )
			fWoundGauge += (fDamage / m_fWoundedRecoveryRate) * m_fWoundedRecoveryGauge;
	}

	return fWoundGauge;
}

bool ioDamageConvertBuff::CheckElementType( int iType )
{
	if( !m_bCheckElementCheckType )
		return true;

	int iCnt = m_vAttackTypeList.size();

	// ���������� ��� ����
	if( iCnt == 0 ) return true;

	for( int i=0; i < iCnt; ++i )
	{
		if( iType == m_vAttackTypeList[i] )
			return true;
	}

	return false;
}

void ioDamageConvertBuff::EndBuff()
{
	m_pOwner->CheckDamageInfo( false );

	if( !m_DamageConvertEffect.IsEmpty() )
	{
		m_pOwner->EndEffect( m_DamageConvertEffect, false );
	}

	if( !m_HPStealEffect.IsEmpty() )
	{
		m_pOwner->EndEffect( m_HPStealEffect, false );
	}

	ioBuff::EndBuff();
}


ioGrowthUpInfo* ioDamageConvertBuff::GetGrowthUpInfoByType( int iType, const ioHashString &szName )
{
	int iLevel = 0;
	int iInfoNum = 0;
	int iClassType = 0;

	ioGrowthLevel* pLevel = NULL;
	ioBaseChar *pChar = NULL;
	ioItem *pItem = GetOwnerItem();
	if( pItem )
	{
		ioPlayStage *pStage = m_pOwner->GetCreator();
		if( pStage )
		{
			pLevel = pStage->GetGrowthLevel( szName );

			pChar = pStage->GetBaseChar( szName );
			if( pChar )
			{
				iClassType = pChar->GetCharacterInfo().m_class_type;
			}
		}

		if( pLevel )
		{
			int iGrowthNum = 0;
			iInfoNum = pItem->GetGrowthInfoNumByType( iType, iGrowthNum );

			int iSlot = pItem->GetType() - 1;
			int nReinforce = pItem->GetItemReinforce();
			iLevel = pLevel->GetItemGrowthLevel( iClassType, iSlot, false );

			//��ȭ������ ���� �߰� ����
			if ( nReinforce <= g_CompoundMaterialMgr.GetFixedFailReinforce() )
				iLevel += nReinforce;
			else
				iLevel += g_GrowthInfoMgr.GetReinforceAddStat( nReinforce );

			if( pChar )
			{
				pChar->GetMedalItemGrowth( iLevel, false, iSlot );

				//�� ����
				pChar->GetPetGrowth( iLevel, false, iSlot );

				//����
				pChar->GetAwakeCharGrowth( iLevel, iSlot );
			}

			return g_GrowthInfoMgr.GetGrowthUpInfo( iInfoNum, iLevel );
		}
	}

	return NULL;
}
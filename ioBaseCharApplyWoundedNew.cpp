
#include "stdafx.h"

#include "ioBaseChar.h"

#include "WeaponDefine.h"
#include "ItemDefine.h"
#include "BuffDefine.h"
#include "SkillDefine.h"
#include "DummyCharDefine.h"

#include "ioPlayStage.h"

#include "ioNpcChar.h"

#include "ioGrowthUpInfo.h"
#include "ioShuffleBonusMode.h"

#include "ioRsoldierInfoManager.h"

#ifdef ANTIHACK
#ifdef _ITEST
extern bool g_bDetailLog;
#else
extern bool g_bDetailLog;
#endif
#endif

void ioBaseChar::ApplyWeaponWoundedEnableChangeWoundState( ioWeapon *pWeapon, bool &bEnableCancel, bool &bNoCancel, DefenseBreakType eBreakType )
{
	if ( !pWeapon )
		return;

	//---���� ���� �������� Ȯ��

	bEnableCancel = true;
	bNoCancel = false;

	if( m_CharState == CS_FLOAT_STATE )
	{
		if( pWeapon->CheckWeaponByFloatBreak() )
		{
			bEnableCancel = true;
		}
		else if( m_FloatBuffInfo.m_iCancelType == ioFloatBuff::FCT_NONE )
		{
			bEnableCancel = false;
		}
		else if( m_FloatBuffInfo.m_iCancelType == ioFloatBuff::FCT_SEMI_CANCEL )
		{
			if( eBreakType == DBT_NONE && !pWeapon->CheckWeaponBySkill() )
				bEnableCancel = false;
		}
		else if ( m_FloatBuffInfo.m_iCancelType == ioFloatBuff::FCT_NO_CANCEL )
		{
			bNoCancel = true;
		}
	}

	//-----------------------------------------------------------------------------

	ioFloatBuff2 *pFloatBuff2 = ToFloatBuff2( GetBuff( BT_FLOAT2 ) );
	if ( pFloatBuff2 && pFloatBuff2->IsNoCancel() )
		bNoCancel = true;

	ioThunderBirdHoldBuff *pThunderBirdHoldBuff = ToThunderBirdHoldBuff( GetBuff( BT_THUNDER_BIRD_HOLD ) );
	if ( pThunderBirdHoldBuff && pThunderBirdHoldBuff->IsNoCancel() )
		bNoCancel = true;
	
	ioStateProtectionBuff *StateProtectionBuff = ToStateProtectBuff( GetBuff( BT_STATE_PROTECTION) );
	if ( StateProtectionBuff && StateProtectionBuff->IsNoCancel() )
		bNoCancel = true;
}

void ioBaseChar::ApplyWeaponWoundedCheckFrozenState( ioWeapon *pWeapon, ioBaseChar *pAttacker, bool &bFrozenBuff, bool &bFrozenState, int &iFrozenType )
{
	//---������ ���� �ʱ�ȭ

	if ( !pWeapon || !pAttacker )
		return;

	const BuffInfoList& kBuffList = pWeapon->GetTargetBuffList();
	int iTargetBuffCnt = kBuffList.size();

	bFrozenBuff = false;
	iFrozenType = ioFrozenBuff::FZT_NONE;
	for( int i=0; i<iTargetBuffCnt; ++i )
	{
		const ioFrozenBuff *pFrozen = ToFrozenConst( g_BuffMaker.GetBuffConst( kBuffList[i].m_BuffName ) );
		if( pFrozen )
		{
			bFrozenBuff = true;
			iFrozenType = pFrozen->GetFrozenType();
			break;
		}
	}

	bool bChangeDefenseBreak = false;
	if( pWeapon->IsDefenseBreakBuff() && pAttacker->HasBuff(BT_DEFENSE_BREAK_BUFF) )
		bChangeDefenseBreak = true;

	bFrozenState = false;
	if( m_CharState == CS_FROZEN )
		bFrozenState = true;

	if( pWeapon->HasFeature( WF_FROZEN_BREAK ) || bChangeDefenseBreak )
	{
		if( bFrozenState && !bFrozenBuff )
			bFrozenState = false;
	}
}

void ioBaseChar::ApplyWeaponWoundedBasePushBlowCalc( ioWeapon *pWeapon, float fDamage, float &fPushPower, float &fBlowPower, bool &bForceLinear )
{
	//---�⺻ ��ο�, Ǫ���Ŀ� ����

	if ( !pWeapon )
		return;

	if( IsFloatingState() )
	{
		float fEotAirPush = GetWoundEnhanceRate( EOT_AIR_PUSH );
		float fEotAirBlow = GetWoundEnhanceRate( EOT_AIR_BLOW );

		if( IsGangsi() )
		{
			fPushPower = pWeapon->GetGangsiAirPushPower( fDamage );
			fBlowPower = pWeapon->GetGangsiAirBlowPower( fDamage );
		}
		else
		{
			fPushPower = pWeapon->GetAirPushPower( fDamage );
			fBlowPower = pWeapon->GetAirBlowPower( fDamage );
		}

		fPushPower *= fEotAirPush;
		fBlowPower *= fEotAirBlow;
		bForceLinear = pWeapon->HasFeature( WF_AIR_PUSH_POWER_LINEAR );
	}
	else
	{
		float fEotPush = GetWoundEnhanceRate( EOT_PUSH );
		float fEotBlow = GetWoundEnhanceRate( EOT_BLOW );

		if( IsGangsi() )
		{
			fPushPower = pWeapon->GetGangsiPushPower( fDamage );
			fBlowPower = pWeapon->GetGangsiBlowPower( fDamage );
		}
		else
		{
			fPushPower = pWeapon->GetPushPower( fDamage );
			fBlowPower = pWeapon->GetBlowPower( fDamage );
		}

		fPushPower *= fEotPush;
		fBlowPower *= fEotBlow;
		bForceLinear = pWeapon->HasFeature( WF_PUSH_POWER_LINEAR );
	}
}

void ioBaseChar::ApplyWeaponWoundedExtraPushBlowCalc( ioWeapon *pWeapon, float &fPushPower, float &fBlowPower )
{
	if ( !pWeapon )
		return;

	//---�߰� ��ο� Ǫ�� ���
	fPushPower *= pWeapon->CheckPushIncreaseBuff( this );
	fBlowPower *= pWeapon->CheckBlowIncreaseBuff( this );

	//---���¿� ���� ���
	if( m_CharState == CS_BLOW_WOUNDED ||
		m_CharState == CS_BOUND_BLOW_WOUNDED ||
		m_CharState == CS_BLOW_EXTEND_WOUND )
	{
		m_iFloatingWoundedCnt++;

		//����ȣ�� �߰� by yunga
		float fBlowRate = 0.0f;
		const WeaponAttribute *pAttr = g_WeaponMgr.FindAttribute( pWeapon->GetAttributeIndex() );
		if( pAttr && pAttr->m_bNoCalcFloatingCnt )
			fBlowRate = m_fFloatingBlowDownRate;
		else
			fBlowRate = m_fFloatingBlowDownRate * (float)m_iFloatingWoundedCnt;

		fBlowRate = max( 0.0f, ( 1.0f - fBlowRate ) );
		fBlowPower *= fBlowRate;

		if( !IsFloatingState() )
		{
			fBlowPower *= pWeapon->GetDownStateBlowPower();
			fPushPower *= pWeapon->GetDownStatePushPower();
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedBaseDamageCalc( ioWeapon *pWeapon, AttackSide eSide, float &fDamage, float &fBoundDamage, bool bTeamAttack )
{
	if ( !pWeapon )
		return;

	//---���� ���⿡ ���� ó��
	UpdateBackAttackCount( eSide );
	m_AttackedSide = eSide;
	
	// �и�, ��� �� ����� ���忡 ���� ������ ����
	fDamage *= pWeapon->GetGrowthAttackRate();
	fBoundDamage *= pWeapon->GetGrowthAttackRate();

	//---������ ����
	// weapon increase buff üũ
	fDamage *= pWeapon->CheckDamageIncreaseBuff( this );

	// Block Damage
	float fBlockRate = (float)pWeapon->GetNpcBlockingRate( this ) / 100.0f;
	fDamage *= fBlockRate;
	fBoundDamage *= fBlockRate;

	// ���� Block Damage
	fBlockRate = (float)pWeapon->GetGangsiBlockingRate( this ) / 100.0f;
	fDamage *= fBlockRate;
	fBoundDamage *= fBlockRate;

	// FC Block Damage
	if( ToFightClubMode(m_pModeParent) )
	{
		fBlockRate = (float)pWeapon->GetFCBlockingRate( this ) / 100.0f;
		fDamage *= fBlockRate;
	}

	// ���� ���°� �ƴϸ鼭 bTeamAttack�� �����ϸ� ������ 0
	if( !m_bPrisonerMode && bTeamAttack && !IsEnableTeamDamage() )
	{
		fDamage = 0.0f;
		fBoundDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}
}

void ioBaseChar::ApplyWeaponWoundedCalcByBuff( ioBaseChar *pAttacker, float &fPushPower, float &fBlowPower, float &fBoundDamage, float &fBoundJumpAmt, float &fBoundForceAmt, float &fBoundGrvityRate )
{
	if ( !pAttacker )
		return;

	// BT_AFFECT_WOUNDED_VALUE������ ���� Push, Blow, Damage ����.
	ioAffectWoundedValueBuff *pAffectWoundedValueBuff = ToAffectWoundedValue( GetAffectWoundedValueBuff() );
	if( pAffectWoundedValueBuff )
	{
		bool bAffect = false;
		if( pAffectWoundedValueBuff->CheckTeamAttack() )
		{
			if( GetTeam() != pAttacker->GetTeam() )
				bAffect = true;
		}
		else
			bAffect = true;

		if( bAffect )
		{
			// Push, Blow Value
			if( IsFloatingState() )
			{
				fPushPower *= pAffectWoundedValueBuff->GetAirPushPowerRate();
				fBlowPower *= pAffectWoundedValueBuff->GetAirBlowPowerRate();
			}
			else
			{
				fPushPower *= pAffectWoundedValueBuff->GetPushPowerRate();
				fBlowPower *= pAffectWoundedValueBuff->GetBlowPowerRate();

				if( m_CharState == CS_BLOW_WOUNDED ||
					m_CharState == CS_BOUND_BLOW_WOUNDED ||
					m_CharState == CS_BLOW_EXTEND_WOUND )
				{
					fPushPower *= pAffectWoundedValueBuff->GetDownPushPowerRate();
					fBlowPower *= pAffectWoundedValueBuff->GetDownBlowPowerRate();
				}
			}

			// Bound Value
			fBoundDamage	*= pAffectWoundedValueBuff->GetBoundDamageRate();
			fBoundJumpAmt	*= pAffectWoundedValueBuff->GetBoundJumpPowerRate();
			fBoundForceAmt	*= pAffectWoundedValueBuff->GetBoundForcePowerRate();
			fBoundGrvityRate*= pAffectWoundedValueBuff->GetBoundGravityRate();
		}
	}
	else if( HasBuff( BT_CONCENTRATE_TARGET ) )
	{
		fPushPower *= GetConcentratePushPowerByTarget( pAttacker );
		fBlowPower *= GetConcentrateBlowPowerByTarget( pAttacker );
	}
}

void ioBaseChar::ApplyWeaponWoundedProtectDamageCalc( ioBaseChar *pAttacker, bool &bProtectDamage, float &fPreCheckDamage, float &fDamage, 
														AttackMethodType eMethodType, int iIgnoreProtect, DefenseBreakType eBreakType, AttackSide eSide, bool bCheckStopMotion )
{
	if ( !pAttacker )
		return;

	bProtectDamage = true;
	fPreCheckDamage = fDamage;

	// �⺻ ���°��� ������ ó��
	if( m_pModeParent->GetModeState() == ioPlayMode::MS_RESULT )
	{
		bProtectDamage = false;
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}

	ioHashString szConcentrateTargetName = pAttacker->GetConcentrateTargetName();
	if( szConcentrateTargetName == GetCharName() )
	{
		fDamage *= pAttacker->GetConcentrateTargetDamageRate();
		//fBaseDamage *= pAttacker->GetConcentrateTargetDamageRate();
	}
	else
	{
		fDamage *= pAttacker->GetConcentrateNoTargetDamageRate();
		//fBaseDamage *= pAttacker->GetConcentrateNoTargetDamageRate();
	}	

	//---
	if( IsRangeProtect(eMethodType) && iIgnoreProtect == IPT_NONE && eBreakType == DBT_NONE )
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}

	if( IsDamageProtect() && iIgnoreProtect == IPT_NONE )		// ������ ����
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}

	if( IsDefenseEnable( eSide, eBreakType, false ) )
		fDamage *= m_fDefenseBrokenDamageRate;

	if( IsFloatingState() )
		fDamage *= m_fFloatStateDamageRate;

	if( m_CharState == CS_FLOAT_STATE && !m_FloatBuffInfo.m_bEnableDamage )
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}
	else if( m_CharState == CS_DRINK )
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}
	else if( bCheckStopMotion && eBreakType == DBT_NONE && iIgnoreProtect == IPT_NONE )
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}
	else if( (m_CharState == CS_ETC_STATE && m_bBlowEtcState) || IsBlowDownState() )
		fDamage *= m_fBlowStateDamageRate;

	if( IsStateProtect( eSide, eBreakType ) && iIgnoreProtect == IPT_NONE )
	{
		fDamage *= GetCurStateProtectRate( eSide );
	}

	if( HasBuff(BT_DEFENSE_DAMAGE) )
	{
		fDamage *= GetCurDefenseDamageBuffRate();
	}
}

void ioBaseChar::ApplyWeaponWoundedCheckBlock( ioWeapon *pWeapon, int iIgnoreProtect, float &fDamage, bool &bProtectDamage, bool bContactType, 
												bool &bOnlyEmoticon, AttackBlockType &eBlockType, ProtectionMode &eProtectionType, AttackSide eAttackSide, DefenseBreakType eBreakType,
												AttackMethodType eMethodType, bool bFrozenBuff, bool bFrozenState, int iFrozenType, bool bCheckStopMotion, bool &bProtectionMagic, bool &bProtectShakeCamera, bool bAirTargetBuff )
{
	if ( !pWeapon )
		return;

	bOnlyEmoticon = false;
	bProtectionMagic = false;
	eBlockType = ATB_NONE;
	eProtectionType = PM_NONE;
	bProtectShakeCamera = false;

	//---�� üũ
	// 1, 2, 3�� ���ΰ��� �ߺ� ����. ���� �������¿��� ��ų �����.
	// 1 < 2 < 3 �� �߿䵵�� ����. ���� ������ �̸�Ƽ���� ��������� ��µ�

	// 1. �ý��� ������ ���� �� üũ
	if( m_ProtectionMode != PM_NONE && iIgnoreProtect != IPT_ALL )	// �ý��� ����
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
		bProtectDamage = false;

		if( !bContactType || pWeapon->CheckWeaponBySkill() )
			bOnlyEmoticon = true;

		eBlockType = ATB_PROTECTION;
		eProtectionType = m_ProtectionMode;
	}

	// 2. �������� ���¿� ���� �� üũ
	if( HasBuff(BT_ABSOLUTE_PROTECTION) && iIgnoreProtect == IPT_NONE )		// ���̳� ����
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;

		if( !bContactType || pWeapon->CheckWeaponBySkill() )
			bOnlyEmoticon = true;

		eBlockType = ATB_MINER;
		eProtectionType = PM_NONE;
	}
	else if( HasBuff(BT_NOT_MOVE_PROTECT) && iIgnoreProtect == IPT_NONE )	// �ݰ��ұ�
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;

		if( !bContactType || pWeapon->CheckWeaponBySkill() )
			bOnlyEmoticon = true;

		eBlockType = ATB_BUFF;
		eProtectionType = PM_NONE;
	}
	else if( IsStateProtect( eAttackSide, eBreakType ) && iIgnoreProtect == IPT_NONE )		// ���� ����
	{
		if( !bContactType || pWeapon->CheckWeaponBySkill() || IsNoBlockStateProtect( eAttackSide ) )
		{
			bOnlyEmoticon = true;
		}

		ioBuff *pBuff = GetBuff( BT_STATE_PROTECTION );
		ioStateProtectionBuff *pStateProtectionBuff = ToStateProtectBuff( pBuff );
		if ( pStateProtectionBuff && pStateProtectionBuff->IsNoEmoticon() )
			eBlockType = ATB_NONE;
		else
			eBlockType = ATB_BUFF;

		eProtectionType = PM_NONE;
	}
	else if( IsRangeProtect(eMethodType) && iIgnoreProtect == IPT_NONE && eBreakType == DBT_NONE )	// ���Ÿ�/��ź ����
	{
		bOnlyEmoticon = true;

		eBlockType = ATB_BUFF;
		eProtectionType = PM_NONE;
	}
	else if( bFrozenState && !bFrozenBuff )							// ������� ����
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;

		if( !bContactType || pWeapon->CheckWeaponBySkill() || pWeapon->IsChangeJumpping( false, bAirTargetBuff ) )
			bOnlyEmoticon = true;

		eBlockType = ATB_ICE;
		eProtectionType = PM_NONE;
	}
	else if( bFrozenState && bFrozenBuff )
	{
		if( iFrozenType == ioFrozenBuff::FZT_EXCEPT && eBreakType == DBT_NONE )
		{
			fDamage = 0.0f;
			//fBaseDamage = 0.0f;

			if( !bContactType || pWeapon->CheckWeaponBySkill() || pWeapon->IsChangeJumpping( false, bAirTargetBuff ) )
				bOnlyEmoticon = true;

			eBlockType = ATB_ICE;
			eProtectionType = PM_NONE;
		}
		else
			fDamage *= m_fFrozenDamageRate;
	}
	else if( bCheckStopMotion && eBreakType == DBT_NONE && iIgnoreProtect == IPT_NONE )
	{
		if( !bContactType || pWeapon->CheckWeaponBySkill() || pWeapon->IsChangeJumpping( false, bAirTargetBuff ) )
			bOnlyEmoticon = true;

		eBlockType = ATB_BUFF;
		eProtectionType = PM_NONE;
	}
	else if( IsBuffProtect() && iIgnoreProtect == IPT_NONE )
	{
		fDamage = 0.0f;

		if( !bContactType || pWeapon->CheckWeaponBySkill() )
			bOnlyEmoticon = true;

		eBlockType = ATB_SKILL;
		eProtectionType = PM_NONE;
	}
	else if( IsProtectionMagicType( pWeapon->GetElementType() ) )		// �Ӽ� ����.
	{
		bOnlyEmoticon = true;
		eBlockType = ATB_BUFF;
		eProtectionType = PM_NONE;
		bProtectionMagic = true;
	}

	// 3. ��ų ������ ���� �� üũ
	if( m_pActiveSkill && m_pActiveSkill->IsProtected( pWeapon->GetDefenseBreakType() ) && iIgnoreProtect == IPT_NONE )		// ��ų ����
	{
		fDamage *= m_fSkillProtectDamageRate;

		if( !bContactType || pWeapon->CheckWeaponBySkill() )
			bOnlyEmoticon = true;

		bProtectShakeCamera = true;

		eBlockType = ATB_SKILL;
		eProtectionType = PM_NONE;
	}

	// 4. ĳ���� ���¿� ���� �� üũ
	if( m_CharState == CS_DEFENSE_ATTACK && eBreakType == DBT_NONE && iIgnoreProtect == IPT_NONE )
	{
		fDamage = 0.0f;
		//fBaseDamage = 0.0f;
	}

	// 5. ������Ʈ �����ۿ� ���� �� üũ
	if( GetObject() )
	{	
		ioObjectItem *pObject = GetObject();
		if( pObject && pObject->IsProtectState() && iIgnoreProtect == IPT_NONE )
		{
			fDamage = 0.0f;
			//fBaseDamage = 0.0f;

			eBlockType = ATB_SKILL;
			eProtectionType = PM_NONE;
		}
	}

	// Enable Side Check Damage
	EnableSideType eSideType = pWeapon->GetEnableSideType();
	switch( eSideType )
	{
	case EST_FRONT:
		if( m_AttackedSide == AS_BACK )
		{
			fDamage = 0.0f;
			//fBaseDamage = 0.0f;
		}
		break;
	case EST_BACK:
		if( m_AttackedSide == AS_FRONT )
		{
			fDamage = 0.0f;
			//fBaseDamage = 0.0f;
		}
		break;
	}
}

void ioBaseChar::ApplyWeaponWoundedReflect( ioWeapon *pWeapon, ioBaseChar *pAttacker, 
												bool &bStateProtectIgnoreReflect, bool &bApplyStateProtect, ioHashString &szReflectAni, float &fReflectAniRate, 
												bool &bPushAttacker, float &fAttackerPushPower, float &fWeakAttackPushPower,
												AttackSide eSide, bool bOnlyEmoticon, const D3DXVECTOR3 &vAttackDir, AttackBlockType eBlockType, ProtectionMode eProtectionType, 
												float &fDamage, bool bContactType, EscapeCatchType eEscapeCatchType, DefenseBreakType eBreakType, bool bStructWeapon )
{
	if ( !pWeapon || !pAttacker )
		return;

	bStateProtectIgnoreReflect = false;
	bApplyStateProtect = false;

	szReflectAni.Clear();
	fReflectAniRate = 1.0f;

	bPushAttacker = false;
	fAttackerPushPower = 0.0f;
	fWeakAttackPushPower = pWeapon->GetDefensePushPower( pWeapon->GetDamage(this) );

	float fAirReflectRate = 1.0f;
	bool bAirReflect = false;	
	AttackSide eCheckReflectSide =  AS_NONE;
	bool bEnableEscapeCatchAttackReflect = false;

	int iReflectIndex = GetReflectInfoIndex( eSide );
	if( iReflectIndex != -1 )
	{
		fAirReflectRate =  m_vReflectInfoList[iReflectIndex]->m_fAirReflectRate;
		bAirReflect = m_vReflectInfoList[iReflectIndex]->m_bAirReflect;
		bStateProtectIgnoreReflect = m_vReflectInfoList[iReflectIndex]->m_bStateProtectIgnoreReflect;
		bEnableEscapeCatchAttackReflect = m_vReflectInfoList[iReflectIndex]->m_bEnableEscapeCatchAttackReflect;
		eCheckReflectSide = m_vReflectInfoList[iReflectIndex]->m_ReflectSideType;
	}

	//Reflect Buff�� StateProtect Buff�� �����ϵ��� ���� �Ǿ����� ó��
	if ( !pAttacker->IgnoreBlock() )
	{
		if( bStateProtectIgnoreReflect )
		{
			if( eCheckReflectSide != AS_NONE && eCheckReflectSide != m_AttackedSide )
			{
				if( !bOnlyEmoticon )
					pAttacker->WeakAttackPush( vAttackDir, fWeakAttackPushPower );

				CheckBlockState( eBlockType, eProtectionType );	
				bApplyStateProtect = true;
			}
		}
		else
		{
			// ���� ���� �и��� �� ���� ó��
			if( eBlockType != ATB_NONE )
			{
				if( !bOnlyEmoticon )
					pAttacker->WeakAttackPush( vAttackDir, fWeakAttackPushPower );

				CheckBlockState( eBlockType, eProtectionType );
			}
		}	
	}

	//---Refelct Start
	if( iReflectIndex != -1 )
	{
		bool bEnableReflect = false;
		if( bEnableEscapeCatchAttackReflect && eEscapeCatchType )
			bEnableReflect = true;
		else if( bContactType )
			bEnableReflect = true;

		if( HasBuff( BT_HIT_COUNT_HOLD ) )
			bEnableReflect = false;

		bool bReflect = false;		
		if( m_CharState != CS_BLOW_WOUNDED &&
			m_CharState != CS_BOUND_BLOW_WOUNDED &&
			m_CharState != CS_BLOW_EXTEND_WOUND &&
			bEnableReflect && !bStructWeapon )
		{
			if ( bAirReflect )
				bReflect = true;
			else
			{
				if ( !IsFloating() )
					bReflect = true;
			}
		}

		if( bReflect )
		{
			if( pAttacker->IsEnableAttackCancelState() )
			{
				// �и��� ���
				fAttackerPushPower = m_vReflectInfoList[iReflectIndex]->GetReflectPower( fDamage );

				// ���� ĵ�� ���� ������ ĵ��ó��
				szReflectAni = m_vReflectInfoList[iReflectIndex]->m_ReflectAni;
				fReflectAniRate = m_vReflectInfoList[iReflectIndex]->m_fReflectAniRate;

				if( !szReflectAni.IsEmpty() && fReflectAniRate > 0.0f && !pAttacker->IgnoreBlock() )
				{
					pAttacker->SetEtcState( szReflectAni, fReflectAniRate );
				}

				// �и��� ����
				// �����ҿ� ���� �и��� ����
				int iSlot = m_vReflectInfoList[iReflectIndex]->m_iSlot;
				ioItem *pItem = NULL;
				if( iSlot >= 0 )
				{
					pItem = GetEquipedItem( iSlot );
					if( pItem )
					{
						float fGrowthValue = 0.0f;
						ioGrowthNormalUpInfo *pUpInfo = ToNormalUpInfo( pItem->GetGrowthUpInfoByType(GT_REFLECT_PUSH, GetCharName()) );
						if( pUpInfo )
							fGrowthValue = pUpInfo->GetValue(m_pCreator);

						fAttackerPushPower += fGrowthValue;
					}
				}

				if( bAirReflect && pAttacker->IsFloating() )
				{
					fAttackerPushPower *= fAirReflectRate;
				}

				pAttacker->SetLastAttackerName( GetCharName().c_str(), pWeapon->GetWeaponBySkillName() );
				pAttacker->ReverseReflectPush( vAttackDir, fAttackerPushPower );

				// ����Ʈ ����
				m_pCreator->CreateMapEffect( m_vReflectInfoList[iReflectIndex]->m_ReflectEffect, GetMidPositionByRate(), GetWorldScale() );

				bPushAttacker = true;

				//SkillEvent
				if( IsOwnerChar() )
					CheckSkillEvent( BT_REFLECT );
			}
		}

		// ������ ���� ����
		if( eBreakType == DBT_NONE )
		{
			int iSlot = m_vReflectInfoList[iReflectIndex]->m_iSlot;
			float fGrowthValue = 0.0f;
			ioItem *pItem = NULL;
			if( iSlot >= 0 )
			{
				pItem = GetEquipedItem( iSlot );
				if( pItem )
				{
					ioGrowthNormalUpInfo *pUpInfo = ToNormalUpInfo( pItem->GetGrowthUpInfoByType(GT_REFLECT_DAMAGE, GetCharName()) );
					if( pUpInfo )
						fGrowthValue = pUpInfo->GetValue(m_pCreator);
				}
			}

			fDamage *= (m_vReflectInfoList[iReflectIndex]->m_fDecreaseDamageRate - fGrowthValue);
		}
	}
	// Reflect End

	// Reflect Demeage
	CheckReflectDameage( bContactType, bStructWeapon, pAttacker, pWeapon );
}

void ioBaseChar::ApplyWeaponWoundedFrontBackBuff( ioBaseChar *pAttacker, AttackSide eAttackSide, bool bAirTargetBuff, bool bTargetBuff, bool bDownTargetBuff,
													ioHashString &szFrontBackWoundBuff, ioHashString  &szFrontBackAttackerBuff, float &fDamage )
{
	if ( !pAttacker )
		return;

	FrontBackWoundInfo kInfo;
	if( pAttacker->GetFrontBackWoundInfo( kInfo ) && bTargetBuff )
	{
		if( eAttackSide == AS_FRONT )
		{
			if( bAirTargetBuff )
			{
				fDamage *= kInfo.m_fAirFrontDamageRate;
				szFrontBackWoundBuff = kInfo.m_AirFrontBuff;
				szFrontBackAttackerBuff = kInfo.m_AirFrontAttackerBuff;
			}
			else if( bTargetBuff )
			{
				fDamage *= kInfo.m_fFrontDamageRate;
				szFrontBackWoundBuff = kInfo.m_FrontBuff;
				szFrontBackAttackerBuff = kInfo.m_FrontAttackerBuff;
			}
			else if( bDownTargetBuff )
			{
				fDamage *= kInfo.m_fDownFrontDamageRate;
				szFrontBackWoundBuff = kInfo.m_DownFrontBuff;
				szFrontBackAttackerBuff = kInfo.m_DownFrontAttackerBuff;
			}
		}
		else
		{
			if( bAirTargetBuff )
			{
				fDamage *= kInfo.m_fAirBackDamageRate;
				szFrontBackWoundBuff = kInfo.m_AirBackBuff;
				szFrontBackAttackerBuff = kInfo.m_AirBackAttackerBuff;
			}
			else if( bTargetBuff )
			{
				fDamage *= kInfo.m_fBackDamageRate;
				szFrontBackWoundBuff = kInfo.m_BackBuff;
				szFrontBackAttackerBuff = kInfo.m_BackAttackerBuff;
			}
			else if( bDownTargetBuff )
			{
				fDamage *= kInfo.m_fDownBackDamageRate;
				szFrontBackWoundBuff = kInfo.m_DownBackBuff;
				szFrontBackAttackerBuff = kInfo.m_DownBackAttackerBuff;
			}
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedExtraDamageCalc( ioWeapon *pWeapon, ioBaseChar *pAttacker, bool bProtectDamage, float fPreCheckDamage, float &fDamage )
{
	if ( !pWeapon || !pAttacker )
		return;

	//---������ 2�� ����
	if( bProtectDamage && fDamage <= 0.0f )
		fDamage = GetProtectStateApplyDamage( pWeapon, fPreCheckDamage );

	// BT_AFFECT_WOUNDED_VALUE ������ ���� ������ ����.
	ioAffectWoundedValueBuff *pAffectWoundedValueBuff = ToAffectWoundedValue( GetAffectWoundedValueBuff() );
	if( pAffectWoundedValueBuff )
	{
		if( pAffectWoundedValueBuff->CheckTeamAttack() )
		{
			if( GetTeam() != pAttacker->GetTeam())
				fDamage *= pAffectWoundedValueBuff->GetDamageRate();
		}
		else
			fDamage *= pAffectWoundedValueBuff->GetDamageRate();
	}
}

void ioBaseChar::ApplyWeaponWoundedDamageFunc( ioWeapon *pWeapon, float fDamage )
{
	if ( !pWeapon )
		return;

	//---�������� ���� �߰� ���
	// check extra gauge
	const WeaponAttribute *pAttr = g_WeaponMgr.FindAttribute( pWeapon->GetAttributeIndex() );
	float fExtraGauge = 0.0f;
	if( pAttr )
		fExtraGauge = pAttr->m_fDamageRate;	

	if( fDamage > 0.0f && fExtraGauge > 0.0f && pWeapon->GetTeam() != GetTeam() )
		SetExtraGaugeTypeByWeapon( EGT_WOUNDED, pWeapon->GetExtraGaugeType(), fExtraGauge );

	// �ǰݽ� ȿ�� �߻��ϴ� ����
	if( fDamage > 0.0f )
		CheckWoundEffectBuff();

	// �ǰݽ� ȿ�� �߻��ϴ� ���� : ������/���¿� ������� �ߵ�...
	CheckExtraCallBuff();

	//�ǰݽ� ĳ������ ���¸� üũ�ؾ� ������ Add Buff ���� üũ
	if( fDamage > 0.0f )
	{
		ioShuffleBonusMode* pShuffleMode = ToShuffleBonusMode( m_pModeParent );
		if( pShuffleMode )			
			pShuffleMode->DropShuffleStarItem( pWeapon, this );
	}
	
	if( fDamage > 0.0f )
	{
		// Ÿ�ݿ� ���� ���� �߻� ó��
		SetWoundedStateApplyBuff( (int)pWeapon->GetType(), pWeapon->GetTeam() );

/*		// ���� ��� �Ϲ��� - > ���� ��ȯ
		ioGangsiMode *pGangsiMode = ToGangsiMode( m_pModeParent );
		if( pGangsiMode )
		{
			// Ư�� ���� Ÿ�� ������Ʈ�κ��� ���� ���ش� ���÷� ��ȯ���� �ʴ´�.
			if( pWeapon->GetTeam() == TEAM_BLUE && GetTeam() == TEAM_RED )
			{
				// ���� ��忡�� �Ϲ����� ������ ���õ�
				SetTeam( TEAM_BLUE );
				pGangsiMode->SendChangeGangsi( this );
			}
		}
*/	
	}
}

void ioBaseChar::ApplyWeaponWoundedApplyPushPower( ioWeapon *pWeapon, bool &bApplyPushPower, bool &bCharDie, 
													bool bFrozenState, bool bFrozenBuff, int iFrozenType, DefenseBreakType eBreakType, 
													AttackSide eAttackSide, AttackMethodType eMethodType, int iIgnoreProtect, bool bProtectionMagic,
													bool bTeamAttack, bool bContactType, bool bCheckStopMotion, const D3DXVECTOR3 &vAttackDir, float fDamage )
{
	bApplyPushPower = true;
	bCharDie = false;

	bool bNoDmgAniNPC = false;
	ioNpcChar *pNpc = ToNpcChar( this );
	if( pNpc )
	{
		if(COMPARE(pNpc->GetNpcType(), NT_BIGTOWER, END_FIXING_NPC))
			bNoDmgAniNPC = true;

		if( pNpc->GetNpcProtectType() == ioNpcChar::NPT_NODMGANI)
			bNoDmgAniNPC = true;
	}

	if( bFrozenState && !bFrozenBuff )
		bApplyPushPower = false;
	else if( bFrozenState && bFrozenBuff )
		if( iFrozenType == ioFrozenBuff::FZT_EXCEPT && eBreakType == DBT_NONE )
			bApplyPushPower = false;

	if( (m_ProtectionMode != PM_NONE && iIgnoreProtect != IPT_ALL) ||
		(HasBuff(BT_ABSOLUTE_PROTECTION) && iIgnoreProtect == IPT_NONE) ||
		(HasBuff(BT_NOT_MOVE_PROTECT) && iIgnoreProtect == IPT_NONE) ||
		(IsStateProtect( eAttackSide, eBreakType ) && iIgnoreProtect == IPT_NONE) ||
		(IsRangeProtect(eMethodType) && iIgnoreProtect == IPT_NONE && eBreakType == DBT_NONE) ||
		bProtectionMagic )
	{
		bApplyPushPower = false;
	}
	else if( m_pActiveSkill )
	{
		if( (m_pActiveSkill->IsProtected( pWeapon->GetDefenseBreakType() ) && iIgnoreProtect == IPT_NONE) ||
			HasBuff(BT_ABSOLUTE_PROTECTION) && iIgnoreProtect == IPT_NONE )
		{
			bApplyPushPower = false;
		}
		else if( GetObject() && GetObject()->IsProtectState() && iIgnoreProtect == IPT_NONE )
		{
			bApplyPushPower = false;
		}
		else if( (!m_pActiveSkill->IsProtected( pWeapon->GetDefenseBreakType() ) && iIgnoreProtect == IPT_NONE) && m_pActiveSkill->IsJumpState() )
		{
			bApplyPushPower = false;
		}
		else
		{
			if( !bNoDmgAniNPC )
			{
				ClearAttackFireTimeAndSkill( CAFT_APPLY_WOUNDED );
				ClearReservedSliding();
			}
		}		
	}
	else if( m_CharState == CS_DRINK )
	{
		bApplyPushPower = false;
	}
	else if( bCheckStopMotion && eBreakType == DBT_NONE && iIgnoreProtect == IPT_NONE )
	{
		bApplyPushPower = false;
	}
	else if( m_CharState == CS_DEFENSE_ATTACK && eBreakType == DBT_NONE && iIgnoreProtect == IPT_NONE )
	{
		bApplyPushPower = false;
	}
	else if( IsBuffProtect() && iIgnoreProtect == IPT_NONE )
	{
		bApplyPushPower = false;
	}
	else if( GetObject() )
	{
		ioObjectItem *pObject = GetObject();
		if( pObject && pObject->IsProtectState() && iIgnoreProtect == IPT_NONE )
			bApplyPushPower = false;
	}
	//
	if( m_bPrisonerMode && bTeamAttack )
	{
		if( bContactType )
			bApplyPushPower = false;
	}
	else if( m_bCatchState && bTeamAttack )
	{
		if( bContactType )
			bApplyPushPower = false;
	}

	// Enable Side Check Applypuspower
	EnableSideType eSideType = pWeapon->GetEnableSideType();
	switch( eSideType )
	{
	case EST_FRONT:
		if( m_AttackedSide == AS_BACK )
			bApplyPushPower = false;
		break;
	case EST_BACK:
		if( m_AttackedSide == AS_FRONT )
			bApplyPushPower = false;
		break;
	}

	if( bApplyPushPower )
	{
		CheckExplosionItemWoundedDrop( m_vMoveDir, false );
		CheckSpecialObjectItemDrop();
	}

	// ���� ���� ���¿��� HP�� Zero�� �Ǹ� �׾���Ѵ�.
	if( IsStateProtect( eAttackSide, eBreakType ) && iIgnoreProtect == IPT_NONE ) // ���� ����
	{
		int iHPCancelType = GetStateProtectHPCancelType();
		if( iHPCancelType == ioStateProtectionBuff::HPT_ZERO_CANCEL &&
			(m_HP.m_fCurValue <= 0.0f && fDamage > 0.0f) )
		{
			bApplyPushPower = true;
		}
	}

	if( IsRangeProtect(eMethodType) && iIgnoreProtect == IPT_NONE && eBreakType == DBT_NONE )
	{
		if( m_HP.m_fCurValue <= 0.0f && fDamage > 0.0f )
			bApplyPushPower = true;
	}

	// ���Ŀ� �����Ǵ� ����
	if( bApplyPushPower && HasBuff(BT_RANGE_PROTECT_BUFF) && eBreakType != DBT_NONE )
		RemoveBuffType( BT_RANGE_PROTECT_BUFF );

	//---Buff ó��
	for( int i=0; i<m_aBuffArray->MaxCount(); i++ )
	{
		ioBuff *pBuff = m_aBuffArray->GetAt(i);
		if( pBuff && pBuff->UseActiveCount() && (pBuff->GetTypeID() == BT_STATE_PROTECTION || pBuff->GetTypeID() == BT_STATE_PROTECTION_BY_PASSIVE || pBuff->GetTypeID() == BT_WINDLORD) )
			pBuff->CheckActiveCount();

		ioHitCountHoldBuff *pHitCountHoldBuff = ToHitCountHoldBuff( pBuff );
		ioHitCountHoldBuff2 *pHitCountHoldBuff2 = ToHitCountHoldBuff2( pBuff );
		ioDummyBuff *pDummyBuff = ToDummyBuff( pBuff );
		if( pHitCountHoldBuff && bApplyPushPower && fDamage > 0.0f )
			pHitCountHoldBuff->IncreaseHitCount();
		else if( pHitCountHoldBuff2 && bApplyPushPower && fDamage > 0.0f )
			pHitCountHoldBuff2->IncreaseHitCount();
		else if( pWeapon && pDummyBuff && pDummyBuff->GetCreatorName() != pWeapon->GetOwnerName() && bApplyPushPower && fDamage > 0.0f )
			pDummyBuff->IncreaseHitCount();
	}	

	// last applyweapon check
	if( bApplyPushPower || fDamage > 0.0f )
		SetLastApplyWeapon( pWeapon->GetAttributeIndex(), pWeapon->GetIgnoreWeaponTime() );

	if( bNoDmgAniNPC )
	{
		if( m_HP.m_fCurValue <= 0.0f && fDamage > 0.0f)
			bCharDie = true;

		bApplyPushPower = false;
	}

	//---Ǫ���Ŀ� ��� ����
	if( bApplyPushPower )
		RotateByWeaponCollision( pWeapon );
}

void ioBaseChar::ApplyWeaponWoundedChangeState( ioWeapon *pWeapon, ioBaseChar *pAttacker, 
													bool &bAllWeakWounded, ioHashString &szWoundAni, DWORD &dwWoundDuration, bool &bWoundedLoopAni, bool &bIgnorCatch, float &fDefenseEnableRate, BlowTargetType &eNewBlowType,
													bool &bCharDie, float &fPushPower, float &fBlowPower, bool bApplyPushPower, bool bTeamAttack, 
													float fDamage, float fBoundJumpAmt, float fBoundForceAmt, float fBoundGrvityRate, float fBoundDamage,
													bool &bPrisoner, bool bEnableCancel, bool bNoCancel,
													ioHashString &szExtendWoundAni, DWORD &dwExtendWoundDuration, float fExtendWoundJumpAmt, float fExtendWoundForceAmt )
{													
	if ( !pWeapon || !pAttacker )					
		return;										

	bAllWeakWounded = false;
	szWoundAni = pWeapon->GetWoundedAnimation();
	dwWoundDuration = pWeapon->GetWoundedDuration();
	bWoundedLoopAni = pWeapon->IsLoopWoundedAnimation();
	bIgnorCatch = false;

	fDefenseEnableRate = pWeapon->GetDefenseEnableRate();
	eNewBlowType = pWeapon->GetBlowType();

	szExtendWoundAni = pWeapon->GetExtendWoundAni();
	dwExtendWoundDuration = pWeapon->GetExtendWoundDuration();

	BlowTargetType ePreBlowType = m_BlowTargetType;
	m_BlowTargetType = pWeapon->GetBlowType();

	BlowWoundAniInfo kBlowWoundAniInfo = pWeapon->GetBlowWoundAniInfo();

	// MustBlowWound Buff Check
	bool bEnableTeamAttack = IsEnableTeamAttack();

	bool bHPZeroKo = false;
	int iPrevEquipItemCnt = GetEquipItemCnt();
	if( !IsItemDrop() )
		iPrevEquipItemCnt = 0;
	else if( IsGangsi() )    // ���ô� �������� ������� �ʴ´�.
		iPrevEquipItemCnt = 0;

	if( m_pCreator )
	{
		if( m_pCreator->CheckRoomZeroKO() )
		{
			bHPZeroKo = true;
			iPrevEquipItemCnt = 0;
		}

		if( m_pCreator->CheckRoomWeakWounded( m_TeamType ) )
		{
			bAllWeakWounded = true;
			fBlowPower = 1.0f;
			//fPushPower = 1.0f;
		}

		ioPlayMode* pMode = m_pCreator->GetPlayMode();
		if( pMode )
			pMode->NottifyWeaponWounded( this );
	}

	bool bEnableTeamDamage = IsEnableTeamDamage();
	if( bApplyPushPower )
	{
		if( m_HP.m_fCurValue <= 0.0f )
		{
			if( m_pModeParent->GetModeState() == ioPlayMode::MS_PLAY )
			{
				if( (!bTeamAttack || bEnableTeamDamage ) &&
					( bHPZeroKo || pWeapon->IsEnableItemDrop( this ) ) && 
					(!IsDamageProtect() || pWeapon->IsIgnoreProtect()) && 
					(!m_bEnableManaShield || pWeapon->IsIgnoreProtect()) )
				{
					if( bHPZeroKo && !ToNpcChar(this))
					{
						AllItemDrop();
					}
					else
					{
						ioHashString szConcentrateTargetName = pAttacker->GetConcentrateTargetName();
						if( szConcentrateTargetName.IsEmpty() )
							CheckItemDrop( false );
						else if( szConcentrateTargetName == pAttacker->GetCharName() )
							CheckItemDrop( false );
					}
				}
			}

			if( !pAttacker->GetCharName().IsEmpty() && !m_bPrisonerMode )
			{
				if( iPrevEquipItemCnt == 0 &&
					(!IsDamageProtect() || pWeapon->IsIgnoreProtect()) &&
					(!m_bEnableManaShield || pWeapon->IsIgnoreProtect()) &&
					(!bTeamAttack || bEnableTeamAttack ) )
				{
					if( !CheckPrisonerMode( fDamage ) && fDamage > 0.0f )
					{
						bCharDie = true;
					}
					else if( fDamage > 0.0f )
					{
						if( bAllWeakWounded )
						{
							if( m_BlowTargetType == BTT_WEAK )
							{
								SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
							}
							else
							{
								m_BlowTargetType = BTT_WEAK;

								SetWoundedState( "", 0,	0.6f, bWoundedLoopAni, bIgnorCatch );
							}
						}
						else
						{
							if( m_BlowTargetType == BTT_BOUND_BLOW )
							{
								bool bGetUpState = false;
								if( (m_CharState == CS_ETC_STATE && m_bBlowEtcState) ||
									(m_CharState == CS_BLOW_WOUNDED && m_BlowState == BW_GETUP) ||
									(m_CharState == CS_BOUND_BLOW_WOUNDED && m_BlowState == BW_GETUP) )
								{
									bGetUpState = true;
								}

								SetBoundWoundedState( fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate, fBoundDamage, bGetUpState );
							}
							else
							{
								if( fBlowPower > m_fKOActionBlowPower )
									SetBlowWoundedState( true, kBlowWoundAniInfo );
								else
									SetBlowWoundedState( false, kBlowWoundAniInfo );
							}
						}

						bPrisoner = true;
					}
				}
			}
		}

		GrapplingType eWeaponGrappling = pWeapon->CheckGrapplingState();
		if( !bCharDie && bEnableCancel && !bNoCancel )
		{
			if( m_bCatchState )
			{
				if( eWeaponGrappling == GPT_NO_CHANGE || eWeaponGrappling == GPT_NO_CHANGE2 || eWeaponGrappling == GPT_NO_CHANGE3 )
				{
					bIgnorCatch = true;
					SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
				}
				else
				{
					if( m_BlowTargetType != BTT_BLOW &&
						m_BlowTargetType != BTT_BLOW2 &&
						m_BlowTargetType != BTT_BLOW3 &&
						m_BlowTargetType != BTT_BOUND_BLOW &&
						m_BlowTargetType != BTT_BLOW_EXTEND_WOUND )
					{
						fPushPower = m_fPrisonerPushPower;
						fBlowPower = m_fPrisonerBlowPower;
					}

					if( m_BlowTargetType == BTT_BOUND_BLOW )
					{
						SetBoundWoundedState( fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate, fBoundDamage, false );
					}
					else
					{
						if( fBlowPower > m_fKOActionBlowPower )
							SetBlowWoundedState( true, kBlowWoundAniInfo );
						else
							SetBlowWoundedState( false, kBlowWoundAniInfo );
					}
				}
			}
			else if( m_bPrisonerMode )
			{
				if( eWeaponGrappling == GPT_NO_CHANGE || eWeaponGrappling == GPT_NO_CHANGE2 || eWeaponGrappling == GPT_NO_CHANGE3  )
				{
					bIgnorCatch = true;
					SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
				}
				else
				{
					if( m_BlowTargetType != BTT_BLOW &&
						m_BlowTargetType != BTT_BLOW2 &&
						m_BlowTargetType != BTT_BLOW3 &&
						m_BlowTargetType != BTT_BOUND_BLOW &&
						m_BlowTargetType != BTT_BLOW_EXTEND_WOUND )
					{
						fPushPower = m_fPrisonerPushPower;
						fBlowPower = m_fPrisonerBlowPower;
					}

					if( m_BlowTargetType == BTT_BOUND_BLOW )
					{
						SetBoundWoundedState( fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate, fBoundDamage, false );
					}
					else
					{
						if( fBlowPower > m_fKOActionBlowPower )
							SetBlowWoundedState( true, kBlowWoundAniInfo );
						else
							SetBlowWoundedState( false, kBlowWoundAniInfo );
					}
				}
			}
			else if( !bPrisoner )
			{
				bool bMustBlowWoundBuff = CheckMustBlowWoundBuff();

				if( bAllWeakWounded )
				{
					if( m_BlowTargetType == BTT_WEAK )
					{
						SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
					}
					else
					{
						m_BlowTargetType = BTT_WEAK;

						SetWoundedState( "", 0,	0.6f, bWoundedLoopAni, bIgnorCatch );
					}
				}
				else if( m_BlowTargetType == BTT_BOUND_BLOW )
				{
					bool bGetUpState = false;
					if( (m_CharState == CS_ETC_STATE && m_bBlowEtcState) ||
						(m_CharState == CS_BLOW_WOUNDED && m_BlowState == BW_GETUP) ||
						(m_CharState == CS_BOUND_BLOW_WOUNDED && m_BlowState == BW_GETUP) )
					{
						bGetUpState = true;
					}

					SetBoundWoundedState( fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate, fBoundDamage, bGetUpState );
				}
				else if( m_BlowTargetType == BTT_BLOW_EXTEND_WOUND )
				{
					SetBlowExtendWoundedState( szExtendWoundAni, dwExtendWoundDuration, fExtendWoundJumpAmt, fExtendWoundForceAmt,
						fDefenseEnableRate, bWoundedLoopAni );
				}
				else if( m_BlowTargetType == BTT_GETUP )
				{
					SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
				}
				else if( m_BlowTargetType == BTT_GETUP_STAND_KO )
				{
					m_BlowTargetType = BTT_STAND_KO2;
					eNewBlowType = BTT_STAND_KO2;

					SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
				}
				else if( IsJumppingState() ||
					m_CharState == CS_BLOW_WOUNDED ||
					m_CharState == CS_BOUND_BLOW_WOUNDED ||
					m_CharState == CS_BLOW_EXTEND_WOUND ||
					m_CharState == CS_FLOAT_STATE ||
					m_CharState == CS_CREEPING ||
					m_CharState == CS_FALL ||
					m_CharState == CS_CHARGE_FLY ||
					m_CharState == CS_BUFF_ACTION ||	
					(m_CharState == CS_USING_SKILL && m_pActiveSkill && m_pActiveSkill->IsDownState()) ||
					(m_CharState == CS_USING_SKILL && m_pActiveSkill && m_pActiveSkill->IsJumpState()) ||
					(m_CharState == CS_ETC_STATE && m_bBlowEtcState ) ||
					(m_CharState == CS_STOP_MOTION && GetBlowStopMotionState() ) ||
					(m_CharState == CS_ICE_STATE && GetBlowStopMotionState() ) ||
					(m_CharState == CS_EXTEND_DASH && IsCanPlayWoundedAniFromExtendDash() ) ||
					m_BlowTargetType == BTT_BLOW ||
					m_BlowTargetType == BTT_BLOW2 ||
					m_BlowTargetType == BTT_BLOW3 ||
					m_BlowTargetType == BTT_FLATTEN ||
					bMustBlowWoundBuff ||
					( m_pCharSpeicalState && m_pCharSpeicalState->IsEnableBlowWounded( this ) ) )
				{
					bool bGetUpState = false;
					if( (m_CharState == CS_ETC_STATE && m_bBlowEtcState) ||
						(m_CharState == CS_BLOW_WOUNDED && m_BlowState == BW_GETUP) ||
						(m_CharState == CS_BOUND_BLOW_WOUNDED && m_BlowState == BW_GETUP) )
					{
						bGetUpState = true;
					}

					if( fBlowPower > m_fKOActionBlowPower )
						SetBlowWoundedState( true, kBlowWoundAniInfo, bGetUpState );
					else
						SetBlowWoundedState( false, kBlowWoundAniInfo, bGetUpState );
				}
				else if( m_CharState == CS_WOUNDED )
				{
					if( ePreBlowType == BTT_M_STIFF )
					{
						if( m_BlowTargetType == BTT_M_STIFF )
						{
							m_BlowTargetType = BTT_STAND_KO;
							eNewBlowType = BTT_STAND_KO;
						}

						SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
					}
					else if( ePreBlowType == BTT_M_BLOW && m_BlowTargetType == BTT_M_BLOW )
					{
						m_BlowTargetType = BTT_BLOW3;
						eNewBlowType = BTT_BLOW3;

						if( fBlowPower > m_fKOActionBlowPower )
							SetBlowWoundedState( true, kBlowWoundAniInfo );
						else
							SetBlowWoundedState( false, kBlowWoundAniInfo );
					}
					else if( ePreBlowType == BTT_M_BLOW && m_BlowTargetType == BTT_M_STIFF )
					{
						m_BlowTargetType = BTT_STAND_KO;
						eNewBlowType = BTT_STAND_KO;

						SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
					}
					else if( ePreBlowType == BTT_STAND_KO || IsFloatingState() )
					{
						if( fBlowPower > m_fKOActionBlowPower )
							SetBlowWoundedState( true, kBlowWoundAniInfo );
						else
							SetBlowWoundedState( false, kBlowWoundAniInfo );
					}
					else if( ePreBlowType == BTT_STAND_KO2 || IsFloatingState() )
					{
						if( fBlowPower > m_fKOActionBlowPower )
							SetBlowWoundedState( true, kBlowWoundAniInfo );
						else
							SetBlowWoundedState( false, kBlowWoundAniInfo );
					}
					else if( ePreBlowType == BTT_GETUP && m_BlowTargetType == BTT_M_STIFF )
					{
						m_BlowTargetType = BTT_STAND_KO;
						eNewBlowType = BTT_STAND_KO;

						SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
					}
					else
					{
						SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
					}
				}
				else if( m_CharState == CS_ATTACK &&
					ToLinearChargeItem2( GetPriorityItem(SPT_EXTEND_ATTACK) ) &&
					ToLinearChargeItem2( GetPriorityItem(SPT_EXTEND_ATTACK) )->IsMustBlowWoundState() )
				{
					if( fBlowPower > m_fKOActionBlowPower )
						SetBlowWoundedState( true, kBlowWoundAniInfo );
					else
						SetBlowWoundedState( false, kBlowWoundAniInfo );
				}
				else
				{
					SetWoundedState( szWoundAni, dwWoundDuration, fDefenseEnableRate, bWoundedLoopAni, bIgnorCatch );
				}
			}
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedChangePrisonerState( bool bTeamAttack, bool bFrozenState, bool bContactType, EscapeCatchType eEscapeCatchType )
{
	if( m_bPrisonerMode && bTeamAttack && !bFrozenState )
	{
		if( bContactType || eEscapeCatchType != ECT_NONE )
		{
			if( P2PNetwork::IsNetworkPlaying() && m_pCreator->IsNetworkMode() )
			{
				SP2Packet kPacket( CTPK_PRISONER_ESCAPE );
				kPacket << GetCharName() << GetLastAttackerName() << GetLastAttackerSkillName();
				TCPNetwork::SendToServer( kPacket );
			}
			else
			{
				ioCatchMode *pMode = ToCatchMode( m_pModeParent );
				ioUnderwearMode *pUnderwear = ToUnderWearMode(m_pModeParent);
				ioCBTMode *pCBT = ToCBTMode(m_pModeParent);

				if( pMode || pUnderwear || pCBT)
					pMode->EscapePrisonerChar( this );
				else
				{
					ioCatchRunningManMode *pRunningManMode = ToCatchRunningManMode( m_pModeParent );
					if( pRunningManMode )
						pRunningManMode->EscapePrisonerChar( this );					
				}
			}
		}
	}
	else if( m_bCatchState && bTeamAttack && !bFrozenState )
	{
		if( bContactType || eEscapeCatchType != ECT_NONE )
			SetEscapeCatchState();
	}
}

void ioBaseChar::ApplyWeaponWoundedPushBlowCalcByBuff( ioWeapon *pWeapon, ioBaseChar *pAttacker, float &fPushPower, float &fBlowPower, bool bTeamAttack )
{
	if ( !pWeapon || !pAttacker )
		return;

	//---Ǫ�� ��ο찪 2�� ����
	if( HasBuff( BT_HIT_COUNT_HOLD ) )
	{
		fPushPower = 0.0f;
		fBlowPower = 0.0f;
	}

	//������� ����� ���� ����
	if ( m_CharState == CS_THUNDER_BIRD_HOLD )
	{
		//�ǰ��ڰ� Ư�� ������ ���� ������ �´� ������ �Ӽ��� �⺻ �Ӽ��̸� ���� �ʴ´�.
		if ( !pWeapon->IsPush() )
		{
			fPushPower = 0.0f;
			fBlowPower = 0.0f;
		}
		else
		{
			fPushPower *= pWeapon->GetEnablePushPowerRate();
			fBlowPower = 0.0f;
		}

		if ( HasBuff( BT_THUNDER_BIRD_HOLD ) )
		{
			//���� �� �ϰ��
			if ( bTeamAttack )
				CheckThunderBirdSpecialBuff( true );
			else
			{
				//������� �������� �ƴ϶�� �������� Ư�� ���� �ڵ� �߻�
				ioThunderBirdItem *pThunderBirdItem = ToThunderBirdItem( pAttacker->GetWeapon() );
				if ( !pThunderBirdItem )
					CheckThunderBirdSpecialBuff( false );
			}
		}
		else
			SetState( CS_DELAY );
	}
}

void ioBaseChar::ApplyWeaponWoundedPushBlowPower( ioWeapon *pWeapon, bool bApplyPushPower, const D3DXVECTOR3 &vAttackDir, float fPushPower, float &fBlowPower, bool bForceLinear )
{
	if ( !pWeapon )
		return;

	if( bApplyPushPower )
	{
		SetCurSpeedToForcePower();

		float fPushAngle = pWeapon->GetPushAngle();
		D3DXVECTOR3 vPushDir = vAttackDir;

		if( vPushDir != ioMath::UNIT_Y )
		{
			D3DXVECTOR3 vAxis1, vAxis2;
			D3DXVec3Cross( &vAxis1, &ioMath::UNIT_Y, &vPushDir );
			D3DXVec3Normalize(&vAxis1,&vAxis1);

			D3DXVec3Cross( &vAxis2, &vPushDir, &vAxis1 );
			D3DXVec3Normalize(&vAxis2,&vAxis2);

			D3DXQUATERNION qtRot;
			if( vAxis2 == ioMath::VEC3_ZERO )
				D3DXQuaternionRotationAxis( &qtRot, &ioMath::UNIT_Y, DEGtoRAD(fPushAngle) );
			else
				D3DXQuaternionRotationAxis( &qtRot, &vAxis2, DEGtoRAD(fPushAngle) );

			vPushDir = qtRot * vPushDir;
			D3DXVec3Normalize(&vPushDir,&vPushDir);

			SetForcePower( vPushDir, fPushPower, 1.0f, bForceLinear );
		}

		if( HasBuff( BT_FLOAT_BUFF ) || HasBuff( BT_FLOAT_MODE_EVENT_BUFF ) )
			fBlowPower *= m_FloatBuffInfo.m_fFloatBlowRate;

		SetJumpPower( fBlowPower );

		// ������ �ʿ䰡 �ִ�.
		m_fCurGravityGapAmt = m_fGravityAmt - m_fCurJumpPowerAmt;
	}
	else
		fBlowPower = 0.0f;
}

void ioBaseChar::ApplyWeaponWoundedAddBuff( ioWeapon *pWeapon, ioBaseChar *pAttacker, 
												bool bApplyPushPower, bool bEnableCancel, bool bTeamAttack, bool &bEnableTeamAttack, bool &bTeamAttackNoDamage,
												bool bPrisoner, ioHashString &szFrontBackWoundBuff, ioHashString &szFrontBackAttackerBuff, ioHashString &szDefenseBreakBuff,
												bool bAirTargetBuff, bool bTargetBuff, bool bDownTargetBuff,
												DefenseBreakType eBreakType, bool &bLandStunBuff, BuffInfoList &kAddBuffList )
{
	if ( !pWeapon || !pAttacker )
		return;

	bTeamAttackNoDamage = pWeapon->HasFeature(WF_TEAM_ATTACK_NO_DAMAGE);
	bEnableTeamAttack = IsEnableTeamAttack();
	bLandStunBuff = false;

	D3DXVECTOR3 vRelativePos = GetWorldPosition() - pAttacker->GetWorldPosition();

	if( bApplyPushPower && bEnableCancel )
	{
		if( szFrontBackWoundBuff.IsEmpty() && ((!bTeamAttack || bEnableTeamAttack) || (bTeamAttack && bTeamAttackNoDamage)) )
		{
			const BuffInfoList& kAirBuffList = pWeapon->GetTargetAirBuffList();
			int iTargetAirBuffCnt = kAirBuffList.size();

			for( int i=0; i<iTargetAirBuffCnt; ++i )
			{
				if( bAirTargetBuff && !kAirBuffList[i].m_BuffName.IsEmpty() )
				{
					if( kAirBuffList[i].m_iBuffType == BT_FROZEN_STATUE && bPrisoner )
						continue;

					if( kAirBuffList[i].m_iBuffType == BT_SKIP_STUN && HasBuff( BT_HIT_COUNT_HOLD ) )
						continue;

					ioBuff *pBuff = AddNewBuff( kAirBuffList[i].m_BuffName, pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
					if( pBuff )
					{
						float fValue = 0.0f;
						ioGrowthUpInfo *pUpInfo = NULL;
						ioGrowthNormalUpInfo *pNormalUpInfo = NULL;

						pBuff->SetItemCode( pAttacker->GetWeaponItemCode() );
						pBuff->SetWeaponIndex( pWeapon->GetWeaponIndex() );

						if( ToAdhesiveBuff(pBuff) )
						{
							ToAdhesiveBuff(pBuff)->SetRelativeVector( vRelativePos );
						}

						pUpInfo = GetGrowthUpInfoForItem(GT_BUFF_DURATION, pAttacker->GetCharName(), pWeapon->GetCreateItem());
						pNormalUpInfo = ToNormalUpInfo(pUpInfo);
						if( pNormalUpInfo )
							fValue = pNormalUpInfo->GetValue(m_pCreator);

						pBuff->SetExtraDurationByGrowth( fValue );
						pBuff->SetBuffDurationByRate( pWeapon->GetOwnerChargeBuffDurationRate() );

						kAddBuffList.push_back( kAirBuffList[i] );
					}
					else if( kAirBuffList[i].m_iBuffType == BT_ACCUMULATE || kAirBuffList[i].m_iBuffType == BT_SEAL_SKILL )
					{
						kAddBuffList.push_back( kAirBuffList[i] );
					}
				}
			}

			const BuffInfoList& kBuffList = pWeapon->GetTargetBuffList();
			int iTargetBuffCnt = kBuffList.size();
			for( int i=0; i<iTargetBuffCnt; ++i )
			{
				if( bTargetBuff && !kBuffList[i].m_BuffName.IsEmpty() )
				{
					if( kBuffList[i].m_iBuffType == BT_FROZEN_STATUE && bPrisoner )
						continue;

					if( kBuffList[i].m_iBuffType == BT_SKIP_STUN && HasBuff( BT_HIT_COUNT_HOLD ) )
						continue;

					ioBuff *pBuff = AddNewBuff( kBuffList[i].m_BuffName, pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
					if( pBuff )
					{
						float fValue = 0.0f;
						ioGrowthUpInfo *pUpInfo = NULL;
						ioGrowthNormalUpInfo *pNormalUpInfo = NULL;

						pBuff->SetItemCode( pAttacker->GetWeaponItemCode() );
						pBuff->SetWeaponIndex( pWeapon->GetWeaponIndex() );

						if( ToAdhesiveBuff(pBuff) )
						{
							ToAdhesiveBuff(pBuff)->SetRelativeVector( vRelativePos );
						}

						pUpInfo = GetGrowthUpInfoForItem(GT_BUFF_DURATION, pAttacker->GetCharName(), pWeapon->GetCreateItem());
						pNormalUpInfo = ToNormalUpInfo(pUpInfo);
						if( pNormalUpInfo )
							fValue = pNormalUpInfo->GetValue(m_pCreator);

						pBuff->SetExtraDurationByGrowth( fValue );
						pBuff->SetBuffDurationByRate( pWeapon->GetOwnerChargeBuffDurationRate() );

						kAddBuffList.push_back( kBuffList[i] );
					}
					else if( kBuffList[i].m_iBuffType == BT_ACCUMULATE || kBuffList[i].m_iBuffType == BT_SEAL_SKILL )
					{
						kAddBuffList.push_back( kBuffList[i] );
					}
				}
			}

			const BuffInfoList& kDownBuffList = pWeapon->GetTargetDownBuffList();
			int iTargetDownBuffCnt = kDownBuffList.size();

			for( int i=0; i<iTargetDownBuffCnt; ++i )
			{
				if( bDownTargetBuff && !kDownBuffList[i].m_BuffName.IsEmpty() )
				{
					if( kDownBuffList[i].m_iBuffType == BT_FROZEN_STATUE && bPrisoner )
						continue;

					if( kDownBuffList[i].m_iBuffType == BT_SKIP_STUN && HasBuff( BT_HIT_COUNT_HOLD ) )
						continue;

					ioBuff *pBuff = AddNewBuff( kDownBuffList[i].m_BuffName, pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
					if( pBuff )
					{
						float fValue = 0.0f;
						ioGrowthUpInfo *pUpInfo = NULL;
						ioGrowthNormalUpInfo *pNormalUpInfo = NULL;

						pBuff->SetItemCode( pAttacker->GetWeaponItemCode() );
						pBuff->SetWeaponIndex( pWeapon->GetWeaponIndex() );

						if( ToAdhesiveBuff(pBuff) )
						{
							ToAdhesiveBuff(pBuff)->SetRelativeVector( vRelativePos );
						}

						pUpInfo = GetGrowthUpInfoForItem(GT_BUFF_DURATION, pAttacker->GetCharName(), pWeapon->GetCreateItem());
						pNormalUpInfo = ToNormalUpInfo(pUpInfo);
						if( pNormalUpInfo )
							fValue = pNormalUpInfo->GetValue(m_pCreator);

						pBuff->SetExtraDurationByGrowth( fValue );
						pBuff->SetBuffDurationByRate( pWeapon->GetOwnerChargeBuffDurationRate() );

						kAddBuffList.push_back( kDownBuffList[i] );
					}
					else if( kDownBuffList[i].m_iBuffType == BT_ACCUMULATE || kDownBuffList[i].m_iBuffType == BT_SEAL_SKILL )
					{
						kAddBuffList.push_back( kDownBuffList[i] );
					}
				}
			}

			if( pAttacker->HasBuff(BT_DEFENSE_BREAK_BUFF) &&
				pWeapon->IsDefenseBreakBuff() &&
				!pWeapon->GetDefenseBreakBuff().IsEmpty() )				
			{
				ioBuff *pBreakBuff = AddNewBuff( pWeapon->GetDefenseBreakBuff(), pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
				if( pBreakBuff )
				{
					float fValue = 0.0f;
					ioGrowthUpInfo *pUpInfo = NULL;
					ioGrowthNormalUpInfo *pNormalUpInfo = NULL;

					pBreakBuff->SetItemCode( pAttacker->GetWeaponItemCode() );
					pBreakBuff->SetWeaponIndex( pWeapon->GetWeaponIndex() );

					if( ToAdhesiveBuff(pBreakBuff) )
					{
						ToAdhesiveBuff(pBreakBuff)->SetRelativeVector( vRelativePos );
					}

					pUpInfo = GetGrowthUpInfoForItem(GT_BUFF_DURATION, pAttacker->GetCharName(), pWeapon->GetCreateItem());
					pNormalUpInfo = ToNormalUpInfo(pUpInfo);
					if( pNormalUpInfo )
						fValue = pNormalUpInfo->GetValue(m_pCreator);

					pBreakBuff->SetExtraDurationByGrowth( fValue );

					szDefenseBreakBuff = pWeapon->GetDefenseBreakBuff();
				}
			}
		}

		bool bPreLandStunCheck = false;
		if( GetTeam() != pWeapon->GetTeam() && CheckLandStunBuff( pWeapon ) )
			bPreLandStunCheck = true;

		if( bPreLandStunCheck && eBreakType == DBT_NONE && !m_vLandStunInfoList.empty() )
		{
			SetCurJumpPowerAmt( 0.0f );
			SetGravityAmt( 0.0f );

			AddNewBuff( m_vLandStunInfoList[0].m_StunBuff, pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
			bLandStunBuff = true;
		}

		// FrontBack
		if( !szFrontBackWoundBuff.IsEmpty() )
		{
			ioBuff *pBuff = AddNewBuff( szFrontBackWoundBuff, pAttacker->GetCharName(), pWeapon->GetCreateItem(), NULL, bTeamAttack );
			if( pBuff )
			{
				float fValue = 0.0f;
				ioGrowthUpInfo *pUpInfo = NULL;
				ioGrowthNormalUpInfo *pNormalUpInfo = NULL;

				pBuff->SetItemCode( pAttacker->GetWeaponItemCode() );
				pBuff->SetWeaponIndex( pWeapon->GetWeaponIndex() );

				if( ToAdhesiveBuff(pBuff) )
				{
					ToAdhesiveBuff(pBuff)->SetRelativeVector( vRelativePos );
				}

				pUpInfo = GetGrowthUpInfoForItem(GT_BUFF_DURATION, pAttacker->GetCharName(), pWeapon->GetCreateItem());
				pNormalUpInfo = ToNormalUpInfo(pUpInfo);
				if( pNormalUpInfo )
					fValue = pNormalUpInfo->GetValue(m_pCreator);

				pBuff->SetExtraDurationByGrowth( fValue );
			}
		}
	}
	else
		szFrontBackWoundBuff.Clear();

	// FrontBack
	if( !szFrontBackAttackerBuff.IsEmpty() )
		pAttacker->AddNewBuff( szFrontBackAttackerBuff, "", "", NULL );
}

void ioBaseChar::ApplyWeaponWoundedActionStopDelay( ioWeapon *pWeapon, ioBaseChar *pAttacker, int &iShakeCamera, bool &bTargetDie, ActionStopType &eActionStype,
														bool bProtectShakeCamera, bool bCharDie, bool bStructWeapon, float fDamage, bool bApplyPushPower )
{
	if ( !pWeapon || !pAttacker )
		return;

	iShakeCamera = pWeapon->GetShakeCamera();
	bTargetDie = false;
	eActionStype = AST_NONE;

	int iTargetShackCamera = iShakeCamera;
	if( bProtectShakeCamera && m_iProtectShakeCamera > 0 )
		iTargetShackCamera = m_iProtectShakeCamera;

	if( bCharDie && m_pEquipSlot && ( !m_pEquipSlot->IsEquipWait() || ToNpcChar( this ) || IsGangsi() ) )
	{
		eActionStype = pWeapon->GetDieActionStopType();
		bTargetDie = true;
	}
	else
		eActionStype = pWeapon->GetActionStopType();

	if( bStructWeapon )
	{
		if( eActionStype != AST_NONE )
			eActionStype = AST_TARGET;
	}

	if( eActionStype != AST_NONE )
	{
		//yunga �߰�( bruce lee�� )
		if( !pWeapon->GetCalcActionDelayPassType() )
		{
			switch( eActionStype )
			{
			case AST_BOTH:
				pAttacker->SetActionStopDelay( fDamage, bTargetDie, iShakeCamera );

				if( bApplyPushPower )
					SetActionStopDelay( fDamage, bTargetDie, iTargetShackCamera );
				break;
			case AST_ATTACKER:
				pAttacker->SetActionStopDelay( fDamage, bTargetDie, iShakeCamera );
				break;
			case AST_TARGET:
				if( bApplyPushPower )
					SetActionStopDelay( fDamage, bTargetDie, iTargetShackCamera );
				break;
			}

			pWeapon->SetActionStopDuration( CalcActionStopDelay( fDamage, bTargetDie ), GetActionStopLooseRate( bTargetDie ) );
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedGrappling( ioWeapon *pWeapon, bool &bNoChangeGrappling, bool bApplyPushPower, bool bTargetDie, bool bPrisoner, CharState eCheckCharState )
{
	if ( !pWeapon )
		return;

	bNoChangeGrappling = false;

	m_GrapplingType = GPT_NONE;

	GrapplingType eWeaponGrappling = pWeapon->CheckGrapplingState();
	if( eWeaponGrappling == GPT_WRESTLING && IsWoundedGrapplingState( eWeaponGrappling ) )
	{
		bool bEnableFloat = true;
		if( IsFloatingState() )
		{
			bEnableFloat = false;

			if( m_CharState == CS_BLOW_WOUNDED && m_BlowState == BW_BLOWING )
			{
				if( !IsFloating() )
					bEnableFloat = true;
			}
			else if( m_CharState == CS_BLOW_EXTEND_WOUND && m_BlowState == BW_BLOWING )
			{
				if( !IsFloating() )
					bEnableFloat = true;
			}
			else if( m_CharState == CS_BOUND_BLOW_WOUNDED &&
				(m_BlowState == BW_BLOWING || m_BlowState == BW_BOUND_BLOWING) )
			{
				if( !IsFloating() )
					bEnableFloat = true;
			}
		}

		bool bCheckPreBlowState = false;
		if( (m_CharState == CS_BLOW_WOUNDED && m_BlowState == BW_GETUP) ||
			(m_CharState == CS_BOUND_BLOW_WOUNDED && m_BlowState == BW_GETUP) )
			bCheckPreBlowState = true;

		if( bEnableFloat && !bCheckPreBlowState )
			m_GrapplingType = GPT_WRESTLING;
	}
	else if( eWeaponGrappling == GPT_WRESTLING_ALL && IsWoundedGrapplingState( eWeaponGrappling ) )// && !HasBuff(BT_STATE_PROTECTION) )
		m_GrapplingType = GPT_WRESTLING_ALL;
	else if( eWeaponGrappling == GPT_PSYCHIC && IsWoundedState() )
		m_GrapplingType = GPT_PSYCHIC;
	else if( eWeaponGrappling == GPT_TOSS && bApplyPushPower )
		m_GrapplingType = GPT_TOSS;
	else if( eWeaponGrappling == GPT_LUCIFER_AIR && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_LUCIFER_AIR;
	else if( eWeaponGrappling == GPT_NAKORURU && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_NAKORURU;
	else if( eWeaponGrappling == GPT_GRAPPLER_JUMP && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_GRAPPLER_JUMP;
	else if( eWeaponGrappling == GPT_MICHAEL && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_MICHAEL;
	else if( eWeaponGrappling == GPT_SUPER_DIMENSION && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_SUPER_DIMENSION;
	else if( eWeaponGrappling == GPT_STRIKER_AIR && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_STRIKER_AIR;
	else if( eWeaponGrappling == GPT_JUMPER && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_JUMPER;
	else if( eWeaponGrappling == GPT_FLY_JUMP_EXTRA_ATTACK && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_FLY_JUMP_EXTRA_ATTACK;
	else if ( eWeaponGrappling == GPT_GALFORD && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_GALFORD;
	else if ( eWeaponGrappling == GPT_DRUID && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_DRUID;
	else if( eWeaponGrappling == GPT_GUNNER && IsWoundedGrapplingState( eWeaponGrappling ) )
		m_GrapplingType = GPT_GUNNER;
	else
	{
		switch( eWeaponGrappling )
		{
		case GPT_WRESTLING_ALL:
			if( IsWoundedState() && eCheckCharState != CS_GRAPPLING_WOUNDED && !HasBuff(BT_STATE_PROTECTION) )
				m_GrapplingType = eWeaponGrappling;
			break;
		case GPT_MULTI:
		case GPT_ATTACK:
		case GPT_NO_CHANGE:
		case GPT_NO_CHANGE2:
		case GPT_NO_CHANGE3:
		case GPT_NO_CHANGE4:
		case GPT_COMPETE:
			m_GrapplingType = eWeaponGrappling;
			break;
		}
	}

	if( eCheckCharState == CS_GRAPPLING_WOUNDED_SKILL &&
		(eWeaponGrappling == GPT_NO_CHANGE || eWeaponGrappling == GPT_NO_CHANGE2 || eWeaponGrappling == GPT_NO_CHANGE3 ) )
	{
		if( !bTargetDie && !bPrisoner )
			bNoChangeGrappling = true;
	}
	else if( eCheckCharState == CS_GRAPPLING_WOUNDED && eWeaponGrappling == GPT_NO_CHANGE4 )
	{
		if( !bTargetDie && !bPrisoner )
			bNoChangeGrappling = true;
	}

	if( bApplyPushPower && bNoChangeGrappling )
		SetNoChangeGrapplingState( m_GrapplingType );

}

void ioBaseChar::ApplyWeaponWoundedWeaponFunc( ioWeapon *pWeapon, ioBaseChar *pAttacker, bool bApplyPushPower )
{
	if ( !pWeapon || !pAttacker )
		return;

	SetIgnoreWeaponInfo( pWeapon );	
	CheckChainWeapon( pWeapon );
	CheckBallWeapon( pWeapon );

	ioAdhesiveMissileWeapon2 *pAdhesive2 = ToAdhesiveMissileWeapon2( pWeapon );
	if( pAdhesive2 )
		pAdhesive2->SetColMoveState( this, true );

	ioUroborusWeapon2 *pUroborus2 = ToUroborusWeapon2( pWeapon );
	if( pUroborus2 )
		pUroborus2->CheckTargetMoveState( this, bApplyPushPower );

	ioUroborusWeapon3 *pUroborus3 = ToUroborusWeapon3( pWeapon );
	if( pUroborus3 )
		pUroborus3->CheckTargetMoveState( this, bApplyPushPower );

	ioUroborusWeapon4 *pUroborus4 = ToUroborusWeapon4( pWeapon );
	if( pUroborus4 )
		pUroborus4->CheckTargetMoveState( this, bApplyPushPower );

	ioUroborusWeapon7 *pUroborus7 = ToUroborusWeapon7( pWeapon );
	if( pUroborus7 )
		pUroborus7->CheckTargetMoveState( this, bApplyPushPower );

	ioContactWeapon *pContact = ToContactWeapon( pWeapon );
	if ( pContact )
		pContact->CheckCollision( this, bApplyPushPower );

	ioGuide2Weapon *pGuide2 = ToGuide2Weapon( pWeapon );
	if( pGuide2 )
		pGuide2->CheckAfterCollisionState( this, bApplyPushPower );

	ioMissileWeapon2 *pMissile2 = ToMissileWeapon2( pWeapon );
	if( pMissile2 )
		pMissile2->CheckTargetColState( pAttacker );
}

void ioBaseChar::ApplyWeaponWoundedSkillFunc( ioWeapon *pWeapon, ioBaseChar *pAttacker, bool bApplyPushPower, float fDamage, DefenseBreakType eBreakType, AttackBlockType eBlockType, ProtectionMode eProtectionType )
{
	if ( !pWeapon || !pAttacker )
		return;

	if( pAttacker->GetTeam() != GetTeam() )
	{
		for( int i=0; i<4; i++ )
		{
			ioSkill *pSkill = GetEquipedSkill( i );
			ioPassiveSkill *pPassiveSkill = ToPassiveSkill( pSkill );
			if( pPassiveSkill )
				pPassiveSkill->CheckReducePassiveSkillGrade( this, pAttacker );					
		}
	}

	//Accessory
	if( pAttacker->GetTeam() != GetTeam() )
	{
		for( int i=ES_RING; i<MAX_EQUIP_SLOT; i++ )
		{
			ioSkill *pSkill = GetEquipedSkill( i );
			ioPassiveSkill *pPassiveSkill = ToPassiveSkill( pSkill );
			if( pPassiveSkill )
				pPassiveSkill->CheckReducePassiveSkillGrade( this, pAttacker );					
		}
	}

	bool bBlock = true;
	if( eBlockType == ATB_NONE && eProtectionType == PM_NONE )
		bBlock = false;

	ioSkill *pSkill = pAttacker->GetCurActiveSkill();
	if( pSkill )
		pSkill->SetInfoByApplyWeapon( pAttacker, this, pWeapon->GetWeaponIndex(), bBlock );

	if( !bBlock )
	{
		if( bApplyPushPower && pAttacker->GetTeam() != GetTeam() )
		{
			if( pWeapon->IsEnablePossession() && HasEnablePossessionBuff() && m_PossessionName.IsEmpty() )
				pAttacker->CheckEnablePossessionState( GetCharName() );
		}

		if( bApplyPushPower && fDamage > 0.0f && pAttacker->GetTeam() != GetTeam() )
		{
			CheckWoundPassiveSkill(	pAttacker );

			for( int i = 0; i < 4; i++ )
			{
				ioSkill *pSkill = NULL;
				pSkill = pAttacker->GetEquipedSkill( i );

				ioPassiveSkill *pPassiveSkill = ToPassiveSkill( pSkill );
				if( pPassiveSkill && pPassiveSkill->UsePassiveGauge() )
				{
					if( pAttacker->HasBuff( BT_CALL_BUFF_ON_ATTACK ) )
					{
						ioHashStringVec vGaugeBuffName = pPassiveSkill->GetOwnerGaugeBuffName( pAttacker );
						if( !vGaugeBuffName.empty() )
						{
							int iBuffCnt = vGaugeBuffName.size();
							for( int j=0; j<iBuffCnt; ++j )
							{								
								pPassiveSkill->CheckAttackProcess( this, pAttacker );
								AddNewBuff( vGaugeBuffName[j], pAttacker->GetCharName(), "", NULL );
							}
						}
					}

					if( eBreakType == DBT_CHANGE &&
						pPassiveSkill->CheckReducePassiveSkillGaugeBuff( pAttacker, BT_DEFENSE_BREAK_BUFF ) )
					{							
						pPassiveSkill->CheckAttackProcess( this, pAttacker );
					}
				}
			}

			//Accessory
			for( int i = ES_RING; i < MAX_EQUIP_SLOT; i++ )
			{
				ioSkill *pSkill = NULL;
				pSkill = pAttacker->GetEquipedSkill( i );

				ioPassiveSkill *pPassiveSkill = ToPassiveSkill( pSkill );
				if( pPassiveSkill && pPassiveSkill->UsePassiveGauge() )
				{
					if( pAttacker->HasBuff( BT_CALL_BUFF_ON_ATTACK ) )
					{
						ioHashStringVec vGaugeBuffName = pPassiveSkill->GetOwnerGaugeBuffName( pAttacker );
						if( !vGaugeBuffName.empty() )
						{
							int iBuffCnt = vGaugeBuffName.size();
							for( int j=0; j<iBuffCnt; ++j )
							{								
								pPassiveSkill->CheckAttackProcess( this, pAttacker );
								AddNewBuff( vGaugeBuffName[j], pAttacker->GetCharName(), "", NULL );
							}
						}
					}

					if( eBreakType == DBT_CHANGE &&
						pPassiveSkill->CheckReducePassiveSkillGaugeBuff( pAttacker, BT_DEFENSE_BREAK_BUFF ) )
					{							
						pPassiveSkill->CheckAttackProcess( this, pAttacker );
					}
				}
			}

			if( pAttacker->HasBuff( BT_CALL_BUFF_ON_ATTACK2 ) )
			{
				ioCallBuffOnAttackBuff2 *pCallBuff2 = ToCallBuffOnAttackBuff2( pAttacker->GetBuff( BT_CALL_BUFF_ON_ATTACK2 ) );
				if( pCallBuff2 )
				{
					ioHashString vCallBuffName = pCallBuff2->GetCallBuffName();
					if( !vCallBuffName.IsEmpty() )
						AddNewBuff( vCallBuffName, pAttacker->GetCharName(), "", NULL );
				}
			}
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedBuffFunc( ioBaseChar *pAttacker )
{
	if ( !pAttacker )
		return;

	if( HasBuff( BT_CREATE_DUMMY_BY_ATTACKED_BUFF ) )
	{
		ioBuff* pBuff = GetBuff( BT_CREATE_DUMMY_BY_ATTACKED_BUFF );
		ioCreateDummyByAttackedBuff* pCreateDummyByAttacedBuff = ToCreateDummyByAttackedBuff( pBuff );
		if ( pCreateDummyByAttacedBuff )
			pCreateDummyByAttacedBuff->OwnerCharApplyWeapon();
	}

	if( HasBuff( BT_CREATE_DUMMY_BY_ATTACKED_BUFF2 ) )
	{
		ioBuff* pBuff = GetBuff( BT_CREATE_DUMMY_BY_ATTACKED_BUFF2 );
		ioCreateDummyByAttackedBuff2* pCreateDummyByAttacedBuff2 = ToCreateDummyByAttackedBuff2( pBuff );
		if ( pCreateDummyByAttacedBuff2 )
			pCreateDummyByAttacedBuff2->OwnerCharApplyWeapon();
	}

	if ( HasBuff( BT_WOUNDED_ADD_BUFF ) )
	{
		ioWoundedAddBuff* pWoundedAddBuff = ToWoundedAddBuff( GetBuff( BT_WOUNDED_ADD_BUFF ) );
		if ( pWoundedAddBuff )
			pWoundedAddBuff->SetAddBuff( pAttacker );
	}

	if ( pAttacker->HasBuff( BT_COUNT_GATHER_ATTACK ) )
	{
		ioCountGatherAttackBuff* pCountGatherAttackBuff = ToCountGatherAttackBuff( pAttacker->GetBuff( BT_COUNT_GATHER_ATTACK ) );
		if ( pCountGatherAttackBuff )
			pCountGatherAttackBuff->CheckHitCount( pAttacker, this );
	}
}

void ioBaseChar::ApplyWeaponWoundedNextStateFunc( ioWeapon *pWeapon, ioBaseChar *pAttacker, float fDamage, BuffInfoList &kAddBuffList )
{
	if ( !pWeapon || !pAttacker )
		return;

	//�÷��� �ڷ���Ʈ ���� ���� (Ư�� ������ �ɼ��� �ִٸ� ��� ����)
	ioZoneEffectWeapon *pZoneEffectWeapon = ToZoneEffectWeapon( pWeapon );
	if ( pZoneEffectWeapon && pZoneEffectWeapon->IsCheckTarget() )
	{
		ioWeaponItem* pWeaponItem = pAttacker->GetWeapon();
		if( pWeaponItem )
		{
			ioFlash_Item *pFlashItem = ToFlashItem( pWeaponItem );
			if ( pFlashItem )
				pFlashItem->SetEnableTeleport( this, pAttacker );
		}
	}

	// �ʴɷ��� ������
	if ( fDamage > 0.0f && pAttacker->GetTeam() != GetTeam() && HasBuff( BT_STATE_PROTECTION ) )
	{
		for( int i = 0; i < 4; i++ )
		{
			ioSkill *pSkill = GetEquipedSkill( i );
			ioTargetMissile2Skill *pTargetMissile2Skill = ToTargetMissile2Skill( pSkill );
			if( pTargetMissile2Skill )
				pTargetMissile2Skill->SetDamage( true );			
		}
	}

	//yunga
	if( pWeapon->IsChangeNextItemType( false ) )
	{
		if( pWeapon->GetChangeNextItemType() == CNIT_TYPE1 || pWeapon->GetChangeNextItemType() == CNIT_TIGER1 || pWeapon->GetChangeNextItemType() == CNIT_TIGER2 )
		{
			ioAttackableItem* pItem = pAttacker->GetPriorityItem( SPT_ATTACK );
			if( pItem )
			{
				pItem->SetNextItemState( this, pAttacker, pWeapon, false, fDamage );

				//ioFlash_Item *pFlashItem = ToFlashItem( pWeaponItem );
				//if ( pFlashItem)
				//	pFlashItem->SetTeleportTarget( this, pAttacker );

				//ioPsychoMetry_Item *pPsychoItem = ToPsychoMetryItem( pWeaponItem );
				//if ( pPsychoItem && fDamage > 0.0f )
				//	pPsychoItem->SetTarget( this, pAttacker );

				//ioDragonNestAssassinItem *pDragonNestAssassinItem = ToDragonNestAssassinItem( pWeaponItem );
				//if ( pDragonNestAssassinItem && fDamage > 0.0f )
				//	pDragonNestAssassinItem->SetDashTeleportState( this, pAttacker );

				//ioSaberItem *pSaber = ToSaberItem( pWeaponItem );
				//if ( pSaber && fDamage > 0.0f )
				//	pSaber->SetChargeTarget( this, pAttacker );

				if( kAddBuffList.size() > 0 )
					pItem->WeaponWounded( pWeapon, this );
			}
		}
		else if( pWeapon->GetChangeNextItemType() == CNIT_TYPE2 )
		{
			ioAttackableItem *pItem = pAttacker->GetPriorityItem( SPT_JUMP );
			if( pItem )
			{
				ioExtendJump *pExtendJump = pItem->GetCurExtendJump();
				if( pExtendJump )
					pExtendJump->SetNextJumpItemState( pAttacker, this, false, CNIT_TYPE2, pWeapon, fDamage );
			}
		}
		else if( pWeapon->GetChangeNextItemType() == CNIT_TYPE3 )
		{
			ioBuff *pBuff = pAttacker->GetBuff( BT_TARGET_RUSH_ATTACK );
			if ( pBuff )
			{
				ioTargetRushAttackBuff* pTargetRushAttackBuff = ToTargetRushAttackBuff( pBuff );
				if ( pTargetRushAttackBuff && fDamage > 0.0f )
					pTargetRushAttackBuff->UseMoveEndAttack( this );
			}

			ioTargetMoveAttackBuff* pTargetMoveAttackBuff = ToTargetMoveAttackBuff( pAttacker->GetBuff( BT_TARGET_MOVE_ATTACK ) );
			if ( pTargetMoveAttackBuff )
				pTargetMoveAttackBuff->ApplyActionState( this );
		}
		else if( pWeapon->GetChangeNextItemType() == CNIT_TYPE4 )
		{
			ioBuff *pBuff = this->GetBuff( BT_ATTACK_COUNT_BUFF );
			if( pBuff )
			{
				ioAttackCountBuff* pAttackCountBuff = ToAttackCountBuff( pBuff );
				if( pAttackCountBuff && fDamage > 0.0f )
				{
					pAttackCountBuff->AttackWound( this, pWeapon );
				}
			}
		}
		else if( pWeapon->GetChangeNextItemType() == CNIT_TYPE5 )
		{
			ioAttackableItem *pItem = pAttacker->GetPriorityItem( SPT_DASH );
			if( pItem )
			{
				ioExtendDash *pExtendDash = pItem->GetCurExtendDash();
				if( pExtendDash )
					pExtendDash->SetNextDashState( this, pAttacker, pWeapon, false, fDamage );
			}
		}
	}

	if( pWeapon->IsChangeNextSkillType( false ) )
	{
		ioSkill *pCurSkill = pAttacker->GetCurActiveSkill();
		if( pCurSkill )
		{
			pCurSkill->SetNextSkillState( this, pWeapon );
		}
	}
}

void ioBaseChar::ApplyWeaponWoundedSingleFunc( ioWeapon *pWeapon, ioBaseChar *pAttacker, float fDamage, bool bAirTargetBuff, bool bApplyPushPower, AttackBlockType eBlockType, ProtectionMode eProtectionType )
{
	if ( !pWeapon || !pAttacker )
		return;

	// local or �Ʒü� or NPC
	bool bNPC = false;
	bool bAttackerNPC = false;
	if( ToNpcChar( this ) )
		bNPC = true;

	if( ToNpcChar( pAttacker ) )
		bAttackerNPC = true;

	if( !P2PNetwork::IsNetworkPlaying() || !m_pCreator->IsNetworkMode() || (bNPC || bAttackerNPC) )
	{		
		if( pWeapon->IsChangeJumpping( false, bAirTargetBuff ) )
		{
			int iLimitCnt = pWeapon->GetChangeJumppingLimitCnt();
			pAttacker->SetChangeJumppingState( pWeapon->GetChangeJumppingPowerRate(), true, iLimitCnt );
		}

		/*if( pWeapon->IsChangeNextItemType( false ) )
		{
			ioWeaponItem* pWeaponItem = pAttacker->GetWeapon();
			if( pWeaponItem )
				pWeaponItem->SetNextItemState( pAttacker );
		}*/

		switch( m_GrapplingType )
		{
		case GPT_WRESTLING:
			if( pAttacker->GetState() == CS_ATTACK )
				pAttacker->SetAttackGrapplingState( GetCharName() );
			else if( pAttacker->GetState() == CS_USING_SKILL )
				pAttacker->SetAttackSkillGrapplingState( GetCharName() );
			else if( pAttacker->GetState() == CS_JUMP )
				pAttacker->SetJumpGrapplingState( GetCharName() );
			break;
		case GPT_WRESTLING_ALL:
			if( pAttacker->GetState() == CS_ATTACK )
				pAttacker->SetAttackGrapplingState( GetCharName() );
			else if( pAttacker->GetState() == CS_USING_SKILL )
				pAttacker->SetAttackSkillGrapplingState( GetCharName() );
			else if( pAttacker->GetState() == CS_JUMP )
				pAttacker->SetJumpGrapplingState( GetCharName() );
			break;
		case GPT_PSYCHIC:
			if( pAttacker->GetState() == CS_USING_SKILL )
				pAttacker->SetPsycGrappling( GetCharName() );
			break;
		case GPT_TOSS:
		case GPT_ATTACK:
		case GPT_COMPETE:
			if( pAttacker->GetState() == CS_USING_SKILL )
				pAttacker->SetAttackSkillGrapplingState( GetCharName() );
			break;
		case GPT_MULTI:
			if( bApplyPushPower && eBlockType == ATB_NONE && eProtectionType == PM_NONE )
			{
				if( pAttacker->GetState() == CS_USING_SKILL )
					pAttacker->SetAttackSkillGrapplingState( GetCharName() );
			}
			break;
		case GPT_LUCIFER_AIR:
			if( pAttacker->GetState() == CS_LUCIFER_FLY )
				pAttacker->SetAttackItemGrapplingState( GetCharName() );
			else if( pAttacker->GetState() == CS_JUMP )
				pAttacker->SetJumpGrapplingState( GetCharName() );
			break;
		case GPT_NAKORURU:
			if( pAttacker->GetState() == CS_JUMP )
				pAttacker->SetJumpGrapplingState( GetCharName() );
			break;
		case GPT_GRAPPLER_JUMP:
			if( pAttacker->GetState() == CS_JUMP )
				pAttacker->SetJumpGrapplingState( GetCharName() );
				break;
		case GPT_MICHAEL:
			if( pAttacker->GetState() == CS_MICHAEL_FLY )
				pAttacker->SetAttackItemGrapplingState( GetCharName() );
			break;
		case GPT_SUPER_DIMENSION:
			if( pAttacker->GetState() == CS_SUPER_DIMENSION_DASH )
				pAttacker->SetAttackItemGrapplingState( GetCharName() );
			break;
		case GPT_JUMPER:				
			pAttacker->SetAttackItemGrapplingState( GetCharName() );
			break;
		case GPT_STRIKER_AIR:
			{
				ioAttackableItem *pItem = pAttacker->GetPriorityItem( SPT_JUMP );
				if( pItem )
				{
					ioExtendJump *pExtendJump = pItem->GetCurExtendJump();
					if( pExtendJump && ToChargeComboJump3( pExtendJump) && 
						ToChargeComboJump3( pExtendJump )->GetCurChargeComboState() == ioChargeComboJump3::CCS_STRIKER_MOVE_ADD_ATTACK )
					{
						pAttacker->SetAttackItemGrapplingState( GetCharName() );
					}
				}
			}
			break;
		case GPT_FLY_JUMP_EXTRA_ATTACK:				
			pAttacker->SetAttackBuffGrapplingState( GetCharName(), BT_FLY_JUMP_EXTRA_ATTACK );
			break;
		case GPT_GALFORD:
			if( pAttacker->GetState() == CS_GALFORD_CMD_STATE )
				pAttacker->SetAttackItemGrapplingState( GetCharName() );
			break;
		case GPT_DRUID:
			if( pAttacker->GetState() == CS_ATTACK )
				pAttacker->SetAttackGrapplingState( GetCharName() );
			break;
		case GPT_GUNNER:
			if( pAttacker->GetState() == CS_CMD_STATE )
				pAttacker->SetAttackItemGrapplingState( GetCharName() );
			break;
		}

		pAttacker->SetWarpTargetList( pAttacker->GetCharName(), GetCharName() );
						
		// check extra gauge
		const WeaponAttribute *pAttr = g_WeaponMgr.FindAttribute( pWeapon->GetAttributeIndex() );
		float fExtraGauge = 0.0f;
		if( pAttr )
			fExtraGauge = pAttr->m_fDamageRate;
			
		if( pWeapon->GetExtraGaugeType() != WGT_NONE && fDamage > 0.0f && fExtraGauge > 0.0f )
			pAttacker->SetExtraGaugeTypeByWeapon( EGT_ATTACK, pWeapon->GetExtraGaugeType(), fExtraGauge );
	}
}

void ioBaseChar::ApplyWeaponWounded( ioWeapon *pWeapon, AttackSide eSide, const D3DXVECTOR3 &vAttackDir )
{
	//---����ó��
	if( !pWeapon )
	{
		LOG.PrintTimeAndLog( 0, "ioBaseChar::ApplyWeaponWounded - Exception Error(1)" );
		return;
	}

	ioBaseChar *pAttacker = pWeapon->GetOwner();
	if( !pAttacker )
	{
		LOG.PrintTimeAndLog( 0, "ioBaseChar::ApplyWeaponWounded() - WeaponOwner(%s) is not exist.", pWeapon->GetOwnerName().c_str() );
		return;
	}

	//---���� �ʱ�ȭ ����
	m_bEnableWoundSkill = pWeapon->CheckWoundSkill();

	EscapeCatchType eEscapeCatchType = pWeapon->GetEnableEscapeCatchType();
	int iWeaponType = (int)pWeapon->GetType();
	DefenseBreakType eBreakType = pWeapon->GetDefenseBreakType();
	CharState eCheckCharState = m_CharState;

	// ���� ó��
	int iMagicResType = pWeapon->IsResistanceMagic( this );
	if( iMagicResType >= 0 )
		CheckNamedEmoticon( iMagicResType, false );

	int iBombResType = pWeapon->IsResistanceBomb( this );
	if( iBombResType >= 0 )
		CheckSkillEvent( iBombResType );

	//---�ǰ��� �ʱ�ȭ ����
	SetRandomSeed( 0 );

	ioAttackableItem *pItem = GetPriorityItem( SPT_WOUNDED );
	if( pItem )
		pItem->CheckWoundedState( this );

	bool bPrisoner = false;

	//---���� ���� �������� Ȯ��
	bool bEnableCancel, bNoCancel;
	ApplyWeaponWoundedEnableChangeWoundState( pWeapon, bEnableCancel, bNoCancel, eBreakType );

	//---������ �ʱ�ȭ ����
	DWORD dwAttackerWeaponItemCode = pAttacker->GetWeaponItemCode();

	// Check RunCharge
	pAttacker->CheckRunChargeCancel( pWeapon->IsCollisionCancel() );
	pAttacker->IncreaseChangeDamageByWeapon( pWeapon->GetWereWolfChangeDamage() );

	if( pWeapon->CheckWeaponByJump() )
		pAttacker->SetSuccessJumpAttack( true );

	// �� ����
	bool bTeamAttack = false;
	if( pWeapon->GetTeam() == GetTeam() && pAttacker != this )
		bTeamAttack = true;

	//---������ ������� ����
	m_iBoundBlowAttackType = pWeapon->GetType();						

	//---������ ���� �ʱ�ȭ
	bool bFrozenBuff, bFrozenState;
	int iFrozenType;
	ApplyWeaponWoundedCheckFrozenState( pWeapon, pAttacker, bFrozenBuff, bFrozenState, iFrozenType );

	//---������ �ʱ�ȭ
	//---������
	//float fBaseDamage = pWeapon->GetBaseDamage( this );
	float fDamage = GetWoundedStateApplyDamage( pWeapon, eSide );
	float fBoundDamage = GetBoundWoundedStateApplyDamage( pWeapon, eSide );
	float fBoundJumpAmt = pWeapon->GetBoundBlowPower( fBoundDamage );
	float fBoundForceAmt = pWeapon->GetBoundPushPower( fBoundDamage );
	float fBoundGrvityRate = pWeapon->GetBoundBlowGravityRate();

	float fExtendWoundJumpAmt = pWeapon->GetExtendWoundBlowPower( fDamage );
	float fExtendWoundForceAmt = pWeapon->GetExtendWoundPushPower( fDamage );

	//---�⺻ ��ο�, Ǫ���Ŀ� ����
	bool bForceLinear;
	float fPushPower, fBlowPower;
	ApplyWeaponWoundedBasePushBlowCalc( pWeapon, fDamage, fPushPower, fBlowPower, bForceLinear );
	ApplyWeaponWoundedExtraPushBlowCalc( pWeapon, fPushPower, fBlowPower );

	ApplyWeaponWoundedBaseDamageCalc( pWeapon, eSide, fDamage, fBoundDamage, bTeamAttack );

	//---������ ���� �� ����
	ApplyWeaponWoundedCalcByBuff( pAttacker, fPushPower, fBlowPower, fBoundDamage, fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate );

	//---������ ���� (���� ����X)
	bool bProtectDamage;
	float fPreCheckDamage;
	AttackMethodType eMethodType = pWeapon->GetAttackMethodType();
	int iIgnoreProtect = pWeapon->IsIgnoreProtect();

	//---��ž���, ����Ȯ��
	bool bCheckStopMotion = false;
	if( m_CharState == CS_STOP_MOTION &&
		(m_iStopMotionType == ioStopMotionBuff::SMT_NORMAL || m_iStopMotionType == ioStopMotionBuff::SMT_LOOP_NORMAL) )
		bCheckStopMotion = true;

	ApplyWeaponWoundedProtectDamageCalc( pAttacker, bProtectDamage, fPreCheckDamage, fDamage, eMethodType, iIgnoreProtect, eBreakType, eSide, bCheckStopMotion );

	bool bDefenseBroken = false;
	if( eBreakType != DBT_NONE )
	{
		if( IsDefenseEnable( eSide, eBreakType, false ) )
		{
			m_pCreator->CreateMapEffect( "sp2_disable_defense_explosion.txt", GetMidPositionByRate(), GetWorldScale() );
			bDefenseBroken = true;

			CheckNamedEmoticon( ioModeHelpManager::HT_DEFENSE_BREAK, false );
		}
	}

	//---�� üũ
	bool bAirTargetBuff = false;
	bool bTargetBuff = false;
	bool bDownTargetBuff = false;

	if( IsFloatingState() || (IsApplyDownState(true) && !IsApplyDownState(false)) )
		bAirTargetBuff = true;
	else if( !IsFloatingState() && !IsApplyDownState(true) )
		bTargetBuff = true;
	else if( IsApplyDownState(false) )
		bDownTargetBuff = true;

	bool bContactType = false;
	if( pWeapon->GetType() == ioWeapon::WT_CONTACT ||
		pWeapon->GetType() == ioWeapon::WT_RUN_CONTACT ||
		pWeapon->GetType() == ioWeapon::WT_CONTACT2 ||
		pWeapon->GetType() == ioWeapon::WT_CONTACT3 )
		bContactType = true;

	AttackSide eAttackSide = GetAttackedSide( vAttackDir );

	bool bOnlyEmoticon;
	bool bProtectionMagic;
	AttackBlockType eBlockType;
	ProtectionMode eProtectionType;
	bool bProtectShakeCamera;

	//---��üũ
	ApplyWeaponWoundedCheckBlock( pWeapon, iIgnoreProtect, fDamage, bProtectDamage, bContactType, 
									bOnlyEmoticon, eBlockType, eProtectionType, eAttackSide, eBreakType, 
									eMethodType, bFrozenBuff, bFrozenState, iFrozenType, bCheckStopMotion, bProtectionMagic, bProtectShakeCamera, bAirTargetBuff );

	//---Reflect
	bool bStructWeapon = false;
	if( pWeapon->CheckWeaponByStruct() || pWeapon->CheckWeaponByDummyChar() )
		bStructWeapon = true;

	if( bStructWeapon )
		bOnlyEmoticon = true;

	bool bStateProtectIgnoreReflect;
	bool bApplyStateProtect;

	ioHashString szReflectAni;
	float fReflectAniRate;

	bool bPushAttacker;
	float fAttackerPushPower;
	float fWeakAttackPushPower;

	ApplyWeaponWoundedReflect( pWeapon, pAttacker, bStateProtectIgnoreReflect, bApplyStateProtect, szReflectAni, fReflectAniRate, 
								bPushAttacker, fAttackerPushPower, fWeakAttackPushPower, eSide, bOnlyEmoticon, vAttackDir, 
								eBlockType, eProtectionType, fDamage, bContactType, eEscapeCatchType, eBreakType, bStructWeapon );

	// Check ReturnAttackBuff
	if( pAttacker && (bContactType || eEscapeCatchType == ECT_CATCH_RETURN) && !bStructWeapon )
		CheckReturnAttackBuff( pWeapon, pAttacker->GetMidPositionByRate() );

	//---��� ���� ����
	ioHashString szFrontBackWoundBuff, szFrontBackAttackerBuff;
	ApplyWeaponWoundedFrontBackBuff( pAttacker, eAttackSide, bAirTargetBuff, bTargetBuff, bDownTargetBuff, szFrontBackWoundBuff, szFrontBackAttackerBuff, fDamage );

	// �ǰݿ� �����Ǵ� ���� ó��
	if( fDamage > 0.0f )
	{
		CheckWoundCancelBuff( BT_RECOVERY );
		CheckWoundCancelBuff( BT_RESTORATION );
		CheckWoundCancelBuff( BT_INFECTION );
		CheckWoundCancelBuff( BT_RESTORATION_GAUGE );
	}

	//������ ����
	NotifyOwnerDamaged( pAttacker->GetCharName(), fDamage, dwAttackerWeaponItemCode, iWeaponType );

	// Check Cur WoundedDamage
	AddWoundDamageInfoList( fDamage, (int)pWeapon->GetType() );

	// Check Cur AttackDamage
	if( pWeapon->GetWeaponBySkillName().IsEmpty() )
		pAttacker->AddAttackDamageInfoList( fDamage, (int)pWeapon->GetType() );

	//---������ 2�� ����
	ApplyWeaponWoundedExtraDamageCalc( pWeapon, pAttacker, bProtectDamage, fPreCheckDamage, fDamage );

	if( iIgnoreProtect == IPT_ALL )
		ApplyDamage( fDamage, true );
	else
		ApplyDamage( fDamage, false );

	//---������ ���� ����.
#ifndef SHIPPING 
	if( Setting::ShowExtraInfo() )
	{
		if ( fDamage != 0.0f )
			LOG.PrintTimeAndLog( 0, "ApplyDamage - %s, Damage: %f", m_Name.c_str(), fDamage );
	}
#endif

	ApplyWeaponWoundedDamageFunc( pWeapon, fDamage );

	//------------------------------------------------------------------------------------------------------------------------------------

	//---Ǫ���Ŀ� ���뿩��
	bool bApplyPushPower;
	bool bCharDie;
	ApplyWeaponWoundedApplyPushPower( pWeapon, bApplyPushPower, bCharDie, bFrozenState, bFrozenBuff, iFrozenType, 
										eBreakType, eAttackSide, eMethodType, iIgnoreProtect, bProtectionMagic, bTeamAttack, bContactType, bCheckStopMotion, vAttackDir, fDamage );

	//---Ǫ���Ŀ� ��� ���� (���º���)
	bool bAllWeakWounded;
	ioHashString szWoundAni;
	DWORD dwWoundDuration;
	bool bWoundedLoopAni;
	bool bIgnorCatch;

	float fDefenseEnableRate;
	BlowTargetType eNewBlowType;

	ioHashString szExtendWoundAni;
	DWORD dwExtendWoundDuration;

	ApplyWeaponWoundedChangeState( pWeapon, pAttacker, 
									bAllWeakWounded, szWoundAni, dwWoundDuration, bWoundedLoopAni, 
									bIgnorCatch, fDefenseEnableRate, eNewBlowType, bCharDie, 
									fPushPower, fBlowPower, bApplyPushPower, bTeamAttack, fDamage, fBoundJumpAmt, fBoundForceAmt, fBoundGrvityRate, fBoundDamage,
									bPrisoner, bEnableCancel, bNoCancel, szExtendWoundAni, dwExtendWoundDuration, fExtendWoundJumpAmt, fExtendWoundForceAmt );

	//---���θ�� ����
	ApplyWeaponWoundedChangePrisonerState( bTeamAttack, bFrozenState, bContactType, eEscapeCatchType );

	//---Ǫ�� ��ο찪 ����
	ApplyWeaponWoundedPushBlowCalcByBuff( pWeapon, pAttacker, fPushPower, fBlowPower, bTeamAttack );

	//---���� ��ο찪, Ǫ���� ����
	ApplyWeaponWoundedPushBlowPower( pWeapon, bApplyPushPower, vAttackDir, fPushPower, fBlowPower, bForceLinear );

	//---���� �߰�
	bool bEnableTeamAttack;
	bool bTeamAttackNoDamage;
	ioHashString szDefenseBreakBuff;
	bool bLandStunBuff;
	BuffInfoList kAddBuffList;
	ApplyWeaponWoundedAddBuff( pWeapon, pAttacker, bApplyPushPower, bEnableCancel, bTeamAttack, bEnableTeamAttack, bTeamAttackNoDamage, 
								bPrisoner, szFrontBackWoundBuff, szFrontBackAttackerBuff, szDefenseBreakBuff, 
								bAirTargetBuff, bTargetBuff, bDownTargetBuff, eBreakType, bLandStunBuff, kAddBuffList );	

	// buff add ����
	if( bApplyPushPower )
		pWeapon->CheckWeaponCollision();

	//---��Ʈī��Ʈ Ȯ��
	bool bEnableHitCount = true;
	if( !bApplyPushPower || pWeapon->HasFeature(WF_DISABLE_HIT_COUNT) )
		bEnableHitCount = false;

	if( pWeapon->GetTeam() != GetTeam() && bEnableHitCount )
	{
		//pAttacker->CheckRealComboHit( GetCharName() );
		pAttacker->CheckComboSkillHit();
	}

	//---ActionStopDelay
	int iShakeCamera;
	bool bTargetDie;
	ActionStopType eActionStype;
	ApplyWeaponWoundedActionStopDelay( pWeapon, pAttacker, iShakeCamera, bTargetDie, eActionStype, bProtectShakeCamera, bCharDie, bStructWeapon, fDamage, bApplyPushPower );

	//---��� ����
	bool bNoChangeGrappling;
	ApplyWeaponWoundedGrappling( pWeapon, bNoChangeGrappling, bApplyPushPower, bTargetDie, bPrisoner, eCheckCharState );

	int eCurState = m_CharState;

	if( bCharDie )
	{
		if( m_pEquipSlot && ( !m_pEquipSlot->IsEquipWait() || ToNpcChar( this ) || IsGangsi() ) )
			OnWeaponDie( GetLastAttackerName(), GetLastAttackerSkillName() );
	}
	else
	{
		if( pWeapon->CheckWeaponBySkill() )
		{
			if( m_CharState == CS_BLOW_WOUNDED ||
				m_CharState == CS_BOUND_BLOW_WOUNDED ||
				m_CharState == CS_WOUNDED ||
				m_CharState == CS_BLOW_EXTEND_WOUND )
			{
				g_CameraEventMgr.CreateCE( m_dwOwnerSkillWoundedCameraEvent, m_pCreator );
			}
		}
	}

	bool bBuffCreate = false;
	if( pWeapon->CheckWeaponByBuff() )
		bBuffCreate = true;

	//---��Ŷ ����
	if( IsNeedSendNetwork() )
	{
		SP2Packet kPacket( CUPK_WEAPON_WOUNDED );
		kPacket << GetCharName();
		kPacket << P2PNetwork::GetNextSendIndex( IsNpcChar( this ) );

		kPacket << pWeapon->GetWeaponBySkillName();
		kPacket << (BYTE)iWeaponType;
		kPacket << dwAttackerWeaponItemCode;

		kPacket << pWeapon->GetWeaponIndex();
		kPacket << pWeapon->GetAttributeIndex();
		kPacket << pAttacker->GetCharName();

		kPacket << (BYTE)m_AttackedSide;
		kPacket << (BYTE)eNewBlowType;
		kPacket << floatToHALF(m_HP.m_fCurValue);

		for( int i=ES_WEAPON; i < ES_OBJECT; i++ )
		{
			ioItem *pItem = GetEquipedItem( i );
			if( pItem )
				kPacket << (BYTE)pItem->GetCurSkillGuage();
			else
				kPacket << (BYTE)0.0f;
		}

		UpdateMoveDir2BYTE();
		UpdateInputDir2BYTE();
		UpdateForceDir2BYTE();

		kPacket << floatToHALF(m_fCurMoveSpeed);
		kPacket << m_byAngleMoveDir << m_byAngleInputDir << m_byAngleForceDirX << m_byAngleForceDirY << m_byAngleForceDirZ;
		kPacket << floatToHALF(m_fForceAmt);
		kPacket << floatToHALF(m_fForceFrictionRate);

		BYTE bPack8 = ((int)m_bForcePowerLinear << 3) |
			((int)bApplyPushPower << 2) |
			((int)IsFloatingState() << 1) |
			((int)IsFloating() << 0);
		kPacket << bPack8;

		kPacket << (BYTE)iMagicResType;
		kPacket << (BYTE)iBombResType;
		kPacket << floatToHALF(m_fCurJumpPowerAmt);
		kPacket << floatToHALF(m_fGravityAmt);

		D3DXVECTOR3 vPos = GetWorldPosition();
		kPacket << floatToHALF(vPos.x) << floatToHALF(vPos.y) << floatToHALF(vPos.z);
		kPacket << m_halfRot[0] << m_halfRot[1] << m_halfRot[2];
		kPacket << g_GlobalTimer.GetGlobalTime();
		kPacket << floatToHALF(fDamage);
		kPacket << (BYTE)pWeapon->GetType();
		kPacket << (BYTE)pWeapon->GetTeam();

		kPacket << (BYTE)eCurState;
		bPack8 = ((int)m_bJumpedBlowWounded << 3) |
			((int)m_bFallBlowWounded << 2) |
			((int)bDefenseBroken << 1) |
			((int)bLandStunBuff << 0);
		kPacket << bPack8;

		kPacket << floatToHALF(fDefenseEnableRate);
		kPacket << bWoundedLoopAni;

		if( eCurState == CS_WOUNDED || eCurState == CS_GRAPPLING_WOUNDED_SKILL )
		{
			BYTE bPack = ((int)bAllWeakWounded << 1) |
				((int)bIgnorCatch << 0);

			kPacket << szWoundAni;
			kPacket << dwWoundDuration;
			kPacket << bPack;
		}
		else if( eCurState == CS_BOUND_BLOW_WOUNDED )
		{
			kPacket << floatToHALF(fBoundDamage);
			kPacket << floatToHALF(fBoundJumpAmt);
			kPacket << floatToHALF(fBoundForceAmt);
			kPacket << floatToHALF(fBoundGrvityRate);
		}
		else if( eCurState == CS_BLOW_EXTEND_WOUND )
		{
			kPacket << szExtendWoundAni;
			kPacket << dwExtendWoundDuration;
			kPacket << floatToHALF(fExtendWoundJumpAmt);
			kPacket << floatToHALF(fExtendWoundForceAmt);
		}

		kPacket << pWeapon->GetCreateItem();
		kPacket << GetRandomSeed();
		bPack8 = ((int)bTeamAttack << 3) |
			((int)bEnableTeamAttack << 2) |
			((int)bTeamAttackNoDamage << 1) |
			((int)bPrisoner << 0);
		kPacket << bPack8;

		int iAddBuffCnt = kAddBuffList.size();
		kPacket << (BYTE)iAddBuffCnt;
		for( int j=0; j<iAddBuffCnt; ++j )
		{
			kPacket << kAddBuffList[j].m_BuffName;
			kPacket << (BYTE)kAddBuffList[j].m_iBuffType;
			kPacket << floatToHALF(pWeapon->GetOwnerChargeBuffDurationRate());
		}

		kPacket << szDefenseBreakBuff;
		kPacket << (BYTE)eBreakType;

		kPacket << szFrontBackWoundBuff;
		kPacket << pWeapon->CheckWeaponByJump();

		//Block
		bPack8 = ((int)bApplyPushPower << 1) |
			((int)bOnlyEmoticon);
		kPacket << bPack8;
		kPacket << (BYTE)eBlockType;
		kPacket << (BYTE)eProtectionType;
		kPacket << floatToHALF(fWeakAttackPushPower);

		//Reflect
		bPack8 = ((int)bPushAttacker << 2) |
			((int)bStateProtectIgnoreReflect << 1) |
			((int)bApplyStateProtect);
		kPacket << bPack8;
		float compactX, compactY;
		encodeXYZ( vAttackDir, compactX, compactY );
		kPacket << compactX << compactY;
		kPacket << floatToHALF(fAttackerPushPower);
		kPacket << szReflectAni;
		kPacket << floatToHALF(fReflectAniRate);

		//CollisionCancel
		bPack8 = ((int)pWeapon->IsCollisionCancel() << 2) |
			((int)bNoChangeGrappling << 1) |
			((int)pWeapon->IsChangeJumpping( false, bAirTargetBuff ));
		kPacket << bPack8;

		//Grappling
		kPacket << (BYTE)m_GrapplingType;
		kPacket << szFrontBackAttackerBuff;

		kPacket << pWeapon->GetWeaponPeculiarity();
		kPacket << floatToHALF(pWeapon->GetChangeJumppingPowerRate());
		kPacket << (BYTE)pWeapon->GetChangeJumppingLimitCnt();

		kPacket << (BYTE)eActionStype;
		kPacket << (BYTE)iShakeCamera;	//����� ������ �����, ��ȭ�ÿ� �߰� ����..

		bPack8 = ((int)bBuffCreate << 2) |
			((int)bTargetDie << 1) |
			((int)bEnableHitCount);
		kPacket << bPack8;


		P2PNetwork::SendToAllPlayingUser( kPacket );
		m_pCreator->NotifyOwnerSendControlInfo();

		if( bEnableHitCount )
		{
			//�� ����
			if( GetTeam() != pAttacker->GetTeam() )
			{
				m_pModeParent->AttackCntContribute( pAttacker->GetCharName(), GetCharName(), dwAttackerWeaponItemCode );

				if( m_pModeParent && m_pModeParent->GetModeType() != MT_FOOTBALL )
					g_AwardMgr.AddWounded( GetCharName(), 100.0f * m_pModeParent->ContributeTeamPerA( GetTeam(), 1.0f, 0.1f ) );
			}
		}

		// �� ��Ʈ ����
		if( GetTeam() != pAttacker->GetTeam() )
		{
			if( m_pModeParent && m_pModeParent->GetModeType() != MT_FOOTBALL )
				g_AwardMgr.AddHit( pAttacker->GetCharName() );
		}

		// NPC Ÿ�ݵ� ���� ����
		if( ToNpcChar( this ) )
			g_AbuseMgr.AddDamage( (int)fDamage );

		if( m_pModeParent->GetModeType() == MT_TEAM_SURVIVAL_AI  )
		{
			if( IsNpcChar(this) && fDamage > 0.f )
			{
				int iDamage = fDamage;
				SetAggro( pAttacker->GetCharName(), iDamage );
			}
		}


#ifdef ANTIHACK
		// anti-hack : new wound check packet
		BYTE eFireType = (BYTE)ANTIHACK_HIT_NORMAL;
		switch( pWeapon->GetCreateFireTimeType() )
		{
		case FTT_SPECIAL_ATTACK:
			eFireType = (BYTE)ANTIHACK_HIT_SPECIAL;
			break;
		case FTT_SKILL_ATTACK:
			eFireType = (BYTE)ANTIHACK_HIT_SKILL;
			break;
		}

		if(GetUserIndex() == g_MyInfo.GetUserIndex())
		{
			bool bNPCAttack = false;
			if( ToNpcChar(pAttacker) )
				bNPCAttack = true;

			SP2Packet kAntiPacket( CUPK_ANTIHACK_CHAR_WOUNDED );			
			if( bNPCAttack )
				kAntiPacket << pAttacker->GetNPCIndex();
			else
				kAntiPacket << pAttacker->GetUserIndex();

			kAntiPacket << pWeapon->GetWeaponIndex();
			kAntiPacket << eFireType;
			P2PNetwork::SendToAllPlayingUser( kAntiPacket );

			if( g_bDetailLog )
			{
				if( g_bDetailLog )
				{
					if( bNPCAttack )
						LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_CHAR_WOUNDED - %u << %u, GetWeaponIndex(%u)", GetUserIndex(), pAttacker->GetNPCIndex(), pWeapon->GetWeaponIndex() );
					else
						LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_CHAR_WOUNDED - %u << %u, GetWeaponIndex(%u)", GetUserIndex(), pAttacker->GetUserIndex(), pWeapon->GetWeaponIndex() );
				}
			}
		}
		else if( IsNpcChar(this) )
		{
			ioNpcChar* pNPC = ToNpcChar(this);
			if( pNPC->GetSyncUser() == g_MyInfo.GetPublicID() )
			{
				bool bNPCAttack = false;
				if( ToNpcChar(pAttacker) )
					bNPCAttack = true;

				SP2Packet kAntiPacket( CUPK_ANTIHACK_CHAR_WOUNDED );			
				if( bNPCAttack )
					kAntiPacket << pAttacker->GetNPCIndex();
				else
					kAntiPacket << pAttacker->GetUserIndex();

				kAntiPacket << pWeapon->GetWeaponIndex();
				kAntiPacket << eFireType;
				P2PNetwork::SendToAllPlayingUser( kAntiPacket, this );

				if( g_bDetailLog )
				{
					if( bNPCAttack )
						LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_CHAR_WOUNDED - %u << %u, GetWeaponIndex(%u)", GetNPCIndex(), pAttacker->GetNPCIndex(), pWeapon->GetWeaponIndex() );
					else
						LOG.PrintTimeAndLog( 0, "CUPK_ANTIHACK_CHAR_WOUNDED - %u << %u, GetWeaponIndex(%u)", GetNPCIndex(), pAttacker->GetUserIndex(), pWeapon->GetWeaponIndex() );
				}
			}
		}
#endif		
	}

	//---���� �߰� ���
	ApplyWeaponWoundedWeaponFunc( pWeapon, pAttacker, bApplyPushPower );

	int iDummyIndex = pWeapon->GetWeaponByDummyChar();
	g_DummyCharMgr.CheckEntityCollision( pAttacker, this, iDummyIndex );

	//---��ų ���� ���
	ApplyWeaponWoundedSkillFunc( pWeapon, pAttacker, bApplyPushPower, fDamage, eBreakType, eBlockType, eProtectionType );

	//---���� ��� ó��
	ApplyWeaponWoundedBuffFunc( pAttacker );

	//---ĳ���� ���º� ���
	ApplyWeaponWoundedNextStateFunc( pWeapon, pAttacker, fDamage, kAddBuffList );

	//---�̱� ��� ó��
	ApplyWeaponWoundedSingleFunc( pWeapon, pAttacker, fDamage, bAirTargetBuff, bApplyPushPower, eBlockType, eProtectionType );

	if ( fDamage > 0.0f )
		g_RSoldierInfoManager.ApplyRandomBuff( pAttacker, this );

	//Attacker Accessory �˻�
	for( int i=ES_RING; i<MAX_EQUIP_SLOT; i++ )
	{
		ioSkill *pSkill = pAttacker->GetEquipedSkill( i );
		ioRandomPassiveSkill *pRandomPassiveSkill = ToRandomPassiveSkill( pSkill );
		if( pRandomPassiveSkill )
		{
			pRandomPassiveSkill->AddWounderRandomDebuffs( pAttacker, this, m_pModeParent->GetModeType() );
			pRandomPassiveSkill->AddAttackerRandombuffs( pAttacker, this, m_pModeParent->GetModeType() );
		}
	}

	//wounder�� Accessory �˻�
	for( int i=ES_RING; i<MAX_EQUIP_SLOT; i++ )
	{
		ioSkill *pSkill = GetEquipedSkill( i );
		ioAccessoryPassiveSkill *pAccPassiveSkill = ToAccessoryPassiveSkill( pSkill );
		ioRandomPassiveSkill *pRandomPassiveSkill = ToRandomPassiveSkill( pSkill );
		if ( pAccPassiveSkill )
			pAccPassiveSkill->CheckReducePassiveGaugeByCheckBuff( this );
		else if( pRandomPassiveSkill )
			pRandomPassiveSkill->AddWounderRandombuffs( pAttacker, this, m_pModeParent->GetModeType() );					
	}
}
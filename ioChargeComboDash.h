#pragma once

#include "ioExtendDash.h"
#include "ioAttackAttribute.h"

class ioINILoader;
class ioBaseChar;
class ioPlayStage;

class ioChargeComboDash : public ioExtendDash
{
public:
	enum DashState
	{
		DS_NONE,
		DS_CHARGE,
		DS_NORMAL_ATTACK,
		DS_COMBO_ATTACK,
		DS_BUFF_ADD,
		DS_MECHANICS_FLY,
		DS_EXTEND_COMBO,
		DS_RUN_ROTATE,
		DS_RUN_ROTATE_END,
		DS_POWER_CHARGE,
		DS_POWER_CHARGE_ATTACK,				// 10
		DS_PHANTOM_FLY,
		DS_LUCIFER_FLY,
		DS_DESTROYER_FLY,
		DS_MICHAEL_FLY,
		DS_GOOMIHO_FLY,
		DS_SUCCUBUS_CHAIN,
		DS_DRACULA,
		DS_FLY_DASH_ATTACK,
		DS_FLY_DASH_ATTACK_LANDING,
		DS_FLY_DASH_ATTACK_END,				// 20
		DS_FLASH,
		DS_EVY,
		DS_SLASHER,
		DS_TOP_BL_EXTEND,
		DS_TOP_BL_RETURN,
		DS_KAELTHAS_EXTEND,
		DS_DRAGONNEST_ASS,
		DS_THUNDER_BIRD,
		DS_EQUIP_OBJECT,
		DS_DELAY_STATE,						// 30
		DS_LEE_DRAGON_ATTACK_STATE,
		DS_PARKILPYO_NORMAL_GATHER,
		DS_PARKILPYO_NORMAL_CHARGE_ATTACK,
		DS_PARKILPYO_NORMAL_CHARGE_COMBO,
		DS_PARKILPYO_NORMAL_EQUIP,
		DS_PARKILPYO_OBJECT_CHARGE_ATTACK,
		DS_JACK_CHARGE_ATTACK,
		DS_SHADOW_RANGER,
		DS_EXTEND_COMBO2,
		DS_KEY_INPUT_LAST_ATTACK,			// 40
		DS_BULLFIGHT,
		DS_JEONWOOCHI_DASH,
		DS_RED_HOOD_CHARGE_ATTACK_STATE,
		DS_CUCHULAIN_MIDDLE_ATTACK_STATE,
		DS_ONIMUSHA_CHARGE_ATTACK,
		DS_ONIMUSHA_RUN,
		DS_HALF_EXTEND_ATTACK,
		DS_SASINMU,
	};

	enum DashExtendType
	{
		DET_NONE,
		DET_BUFF_ADD,
		DET_MECHANICS_FLY,
		DET_EXTEND_COMBO,
		DET_RUN_ROTATE,
		DET_POWER_CHARGE,
		DET_RUN_ROTATE2,
		DET_PHANTOM_FLY,
		DET_LUCIFER_FLY,
		DET_DESTROYER_FLY,
		DET_MICHAEL_FLY,		// 10
		DET_GOOMIHO_FLY,
		DET_SUCCUBUS_CHAIN,
		DET_DRACULA,
		DET_FLY_DASH_ATTACK,
		DET_FLASH,
		DET_EVY,
		DET_SLASHER,
		DET_TOP_BL,
		DET_KAELTHAS,
		DET_DRAGONNEST_ASS,		// 20
		DET_THUNDER_BIRD,
		DET_EQUIP_OBJECT,
		DET_LEE,
		DET_PARKILPYO_NORMAL,
		DET_PARKILPYO_OBJECT,
		DET_JACK,
		DET_SHADOW_RANGER,
		DET_JUMPER,
		DET_SABER,
		DET_ANGEL_OF_DEATH,		// 30
		DET_HADES,
		DET_MAGIC_SWORD,
		DET_NATA,
		DET_RESEARCHER_REAR_FLY,
		DET_BULLFIGHT,
		DET_JEONWOOCHI,
		DET_CHEMISTRY,
		DET_SAWANG_SLIDE,
		DET_RED_HOOD,
		DET_STRIDER,			//40
		DET_APOSTATE_MONK,
		DET_CUCHULAIN,
		DET_ONIMUSHA,
		DET_DRUID,
		DET_SASINMU,
		DET_PREDATOR,
		DET_CROW,
		DET_BLACK_SPHEAR,
		DET_MEPHISTO,
	};

	enum DashSyncType
	{
		DST_STATE,
		DST_KEY,
	};

	enum JeonWooChiMoveStateType
	{
		JWCST_NONE,
		JWCST_POWER_CHARGE,
		JWCST_DASH_STATE,
		JWCST_DASH_ATTACK,
		JWCST_DASH_ADD_ATTACK,
	};

protected:
	JeonWooChiMoveStateType m_JeonWooChiState;

	DashState m_DashState;

	bool m_bNormalAttacked;
	bool m_bSetCombo;
	int m_iCurComboCnt;

	DWORD m_dwActionEndTime;
	DWORD m_dwActionStartTime;

	DWORD m_dwPreReserveEnableTime;

	DWORD m_dwChargingTime;
	DWORD m_dwHalfChargingTime;
	DWORD m_dwChargingStartTime;

	ioHashStringVec m_ChargingAniList;

	//ž����Ʈ
	std::vector<StTopMoveInfo> m_vDummyMoveInfoCombo;
	StTopMoveInfo m_TopBl_Normal;
	StTopMoveInfo m_TopBl_ChargeInfo;
	AttackAttribute m_TopBl_ChargeAttack;

	DWORD			m_dwChargeAttackDummyDelayTime;
	DWORD			m_dwChargeAttackDummyFireTime;



// dash extend type
protected:
	DashExtendType m_DashExtendType;

	// buff add
	AttackAttribute m_BuffAddAttribute;
	ioHashStringVec m_vBuffAddList;

	// mechanics fly
	MechanicsFlyInfo m_DashMechanicsFly;

	// LuciferFlyInfo
	LuciferFlyInfo	 m_DashLuciferFly;

	//����
	DestroyerFlyInfo	 m_DestroyerFly;

	// michael fly
	MichaelFlyInfo m_DashMichaelFly;

	//����ȣ
	GoomihoFlyInfo m_DashGoomihoFly;

	//��ť����
	SuccubusAttackInfo m_DashSuccubusChain;

	//���ŧ��
	DraculaFlyInfo m_DashDraculaFly;

	//�̺�
	EvyBlinkInfo m_EvyBlinkInfo;

	//������
	SlasherRunInfo m_SlasherRunInfo;

	//����ġ
	JeonWooChiDashMoveInfo m_JeonWooChiDashInfo;
	//////////////////////////////////////////////////////////////////////////
	float m_fJeonWooChiCurMoveLength;
	float m_fJeonWooChiNeedMoveLength;
	D3DXVECTOR3 m_vJeonWooChiMoveDir;
	ioHashString m_stJeonWooChiChargeTargeted;
	//////////////////////////////////////////////////////////////////////////

	// extend combo
	AttributeList m_ExtendAttackList;

	// extend combo2
	AttributeList m_ExtendAttackList2;

	//�ź����
	AttackAttribute m_KaelthasExtendAttribute;
	DWORD			m_dwKaelthasExtendAttackStartTime;

	//LeeDragonAttack
	AttackAttribute m_LeeDragonExtendAttribute;
	DWORD			m_dwLeeExtendAttackStartTime;
	ioHashString	m_stDrangonAttackBuff;
	ioHashString	m_stDrangonAttackCancelBuff;

	//fly dash
	CEncrypt<float> m_fFlyDashSpeed;
	CEncrypt<DWORD> m_dwFlyDashDuration;

	// run rotate
	CEncrypt<float> m_fRunSpeed;
	CEncrypt<DWORD> m_dwRunDuration;
	CEncrypt<DWORD> m_dwRotateSpeed;

	CEncrypt<DWORD> m_dwRunRotateStartTime;
	CEncrypt<DWORD> m_dwRunRotateJumpTime;
	CEncrypt<DWORD> m_dwFlyDashAttackStartTime;



	AttackAttribute m_RunRotateAttack;
	AttackAttribute m_FlyDashAttack;

	ioHashString	m_FlyDashLandingAni;
	float			m_fFlyDashLandingAniRate;

	CEncrypt<bool> m_bTargetRot;
	ioUserKeyInput::DirKeyInput m_CurDirKey;

	AttackAttribute m_RunRotateAttackEnd;
	AttackAttribute m_FlyDashAttackEnd;

	// power charge
	ioHashString m_PowerChargeAni;

	CEncrypt<DWORD> m_dwMaxChargeTime;
	CEncrypt<DWORD> m_dwChargeStartTime;

	CEncrypt<float> m_fCurChargeRate;

	CEncrypt<float> m_fMaxDamageRate;
	CEncrypt<float> m_fMaxForceRate;
	CEncrypt<float> m_fMaxBlowRate;

	CEncrypt<float> m_fMaxRangeRate;
	CEncrypt<float> m_fMaxSpeedRate;
	CEncrypt<float> m_fMaxChargeAniRate;
	CEncrypt<float> m_fMaxForceSlidingRate;

	CEncrypt<float> m_fMaxTargetRangeRate;
	CEncrypt<float> m_fMaxTargetAngleRate;

	CEncrypt<float> m_fMaxFloatRate;

	CEncrypt<float> m_fMaxScaleRate;
	CEncrypt<float> m_fMaxExplosionRate;
	CEncrypt<float> m_fMaxLiveTimeRate;
	CEncrypt<float> m_fMaxWoundedTimeRate;

	AttackAttribute m_PowerChargeAttack;

	ioHashString m_DashGatheringEffect;
	UniqueObjID m_GatheringEffectID;

	CEncrypt<bool> m_bPowerChargeUseDefense;

	//
	DWORD m_dwInputCancelCheckTime;
	DWORD m_dwCancelCheckTime;

	//
	bool m_bPlayDashExtendWoundAni;

	//Fly Dash Attack ������ �Ҹ� 
	float	m_fFlyDashDecreseGauge;

	//KeyReserve�� ������� Input_Jump_S ��ũ�ΰ� �ִ� �ð����� ���� üũ
	bool m_bExceptionInputJumpSCheck;
	bool m_bInitActionDelayTime_NormalAttack;

protected:
	// For Ani Rotate
	vAniRotateInfoList m_vAniRotateInfoList;

	int m_iCurAniRotate;
	DWORD m_dwAniRotateTime;
	float m_fAniRotateAngle;

//�巡�� �����̾�
protected:
	bool m_bAniJumpInputS;

protected:
	ioHashString				m_szInputEventAnimation;
	float						m_fInputEventAnimationRate;	
	ioUserKeyInput::DirKeyInput	m_eSpecialDashCurrDirkey;

//�巡�� �׽�Ʈ ����
protected:
	enum DragonNestAssSubState
	{
		DNASS_Pre,
		DNASS_Teleport,
		DNASS_LandMove,
	};

protected:
	DragonNestAssSubState m_DragonNestAssSubState;

	AttackAttribute m_DragonNestAssassinPreAttribute;
	AttackAttribute m_DragonNestAssassinTeleportAttackAttribute;
	float m_fDragonNestAssassinExtendTeleportEndJump;
	D3DXVECTOR3 m_vDragonNestAssassinTeleportOffSet;

//�������
protected:
	float m_fThunderBirdUseGauge;

protected:
	bool m_bMaxDashLastChargeAttack;

//����ǥ
protected:
	ioHashString m_szParkilpyoGatherAni;
	float m_fParkilpyoGatherAniRate;
	AttackAttribute m_ParkilpyoChargeAttack;
	AttributeList m_ParkilpyoChargeComboList;
	float m_fParkilpyoUseGauge;
	DWORD m_dwEnableKeyTime;
	int m_nCurChargeCombo;

//��
protected:
	bool m_bSetEnableReserveDash;
	AttackAttribute m_JackChargeAttack;
	float m_fJackUseGauge;
	int m_nMaxReserveDash;
	int m_nCurReserveDash;

//�����췹����
protected:
	float m_fShadowRangerUseGauge;

//���˻�
	bool						m_bKeyInputLastAttack;
	AttackAttribute				m_AttackKeyAttribute;
	AttackAttribute				m_DefenseKeyAttribute;
	AttackAttribute				m_JumpKeyAttribute;

//RedHood
	DWORD						m_dwRedHoodSpecialMoveMacroTime_S;
	DWORD						m_dwRedHoodSpecialMoveMacroTime_E;


//���Ǹ�
	AttackAttribute				m_CuchulainMiddleAttack;
	AttackAttribute				m_CuchulainChargeAttack;
	AttributeList				m_CuchulainChargeComboAttack;
	bool						m_bMiddleChargeStarted;
	CuchulainFlyInfo			m_CuchulainFlyInfo;

	//�͹���
protected:
	AttackAttribute m_OnimushaChargeAttack;
	float m_fOnimushaChargeUseGauge;
	float m_fOnimushaObjectChargeUseGauge;
	bool m_bOnimushaChargeAttack;
	
//�� D~ �߰�
	AttackAttribute				m_HalfExtendAttack;
	bool						m_bEnableHalfExtendAttack;
	int							m_nHalfAttackCount;

protected:
	void CheckAniRotate( ioBaseChar *pChar );

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual DashType GetType() const;
	virtual ioExtendDash* Clone();

	virtual bool StartDash( ioBaseChar *pOwner );
	virtual void ProcessDash( ioBaseChar *pOwner );
	virtual void ExtendDashEnd( ioBaseChar *pOwner );

	virtual void ApplyDash( ioBaseChar *pOwner, SP2Packet &rkPacket );

	virtual bool GetAutoTargetValue( IN float &fDashRange, IN float &fDashMinAngle, IN float &fDashMaxAngle,
									 OUT float &fRange, OUT float &fMinAngle, OUT float &fMaxAngle );

	virtual bool IsEnableDefense();
	bool IsPowerChargingState();

	//����ġ�� �߰�
	bool CheckProcessDash( ioBaseChar *pOwner );

	virtual void SetNextDashState( ioBaseChar *pWoundChar, ioBaseChar *pAttacker, ioWeapon *pWeapon, bool bDefense, float fDamage );

public:
	void SetExtraInfoToWeapon( ioBaseChar *pOwner, ioPlayStage *pStage, ioWeapon *pWeapon, const ioHashString &szName );
	void CheckCancelReserveByConditionalSpecialDash( ioBaseChar *pOwner );

public:
	void SetCombo( bool bSetCombo ){ m_bSetCombo = bSetCombo; }
	void SetCurrComboCnt( int iCurrComoboCnt ){ m_iCurComboCnt = iCurrComoboCnt; }
	int GetCurrComboCnt(){ return m_iCurComboCnt; }

protected:
	void SetComboDash( ioBaseChar *pOwner, bool bCharge = false );

	void ProcessCharging( ioBaseChar *pOwner );
	void ProcessNormalComboDash( ioBaseChar *pOwner );
	void ProcessExtendComboDash( ioBaseChar *pOwner );
	void ProcessRunRotate( ioBaseChar *pOwner );
	void ProcessRunRotateEnd( ioBaseChar *pOwner );
	void ProcessPowerCharge( ioBaseChar *pOwner );
	void ProcessPowerChargeAttack( ioBaseChar *pOwner );
	void ProcessFlyDashAttack( ioBaseChar *pOwner );
	void ProcessFlyDashAttackLanding( ioBaseChar *pOwner );
	void ProcessFlyDashAttackEnd( ioBaseChar *pOwner );
	void ProcessKaelthasExtendAttack( ioBaseChar *pOwner );
	void ProcessDragonNestAssassinState( ioBaseChar *pOwner );
	void ProcessJeonWooChiDashState( ioBaseChar *pOwner );

	void ChangeToCharging( ioBaseChar *pOwner );
	void ChangeToNormalAttack( ioBaseChar *pOwner, bool bCancelAttack = false, bool bChargeAttack = false);

	virtual void CheckKeyReserve( ioBaseChar *pOwner );
	virtual void ProcessKeyReserve( ioBaseChar *pOwner );
	
	void CheckExtraReserve( ioBaseChar *pOwner );
	void CheckExtendReserve( ioBaseChar *pOwner );
	bool CheckPreReserve( ioBaseChar *pOwner );

	void ApplyDashState( ioBaseChar *pOwner, SP2Packet &rkPacket );
	void ApplyDashDirKey( ioBaseChar *pOwner, SP2Packet &rkPacket );

// buff add
protected:
	bool CheckEnableBuffAddState( ioBaseChar *pOwner );
	void SetBuffAddState( ioBaseChar *pOwner );

// fly dash attack
	bool CheckEnableFlyDashAttackState( ioBaseChar *pOwner );
	void SetFlyDashAttackState( ioBaseChar *pOwner, bool bSendNet );
	void SetFlyDashAttackLandingState( ioBaseChar *pOwner );
	void SetFlyDashAttackStateEnd( ioBaseChar *pOwner );

protected:
	bool CheckEnableMechanicsFlyState( ioBaseChar *pOwner );
	void SetMechanicsFlyState( ioBaseChar *pOwner );

	void LoadMechanicsFlyInfo( ioINILoader &rkLoader );

protected:
	bool CheckEnableDestroyerFlyState( ioBaseChar *pOwner );
	void SetDestroyerFlyState( ioBaseChar *pOwner );

	void LoadDestroyerFlyInfo( ioINILoader &rkLoader );

protected:
	bool CheckEnableExtendComboState( ioBaseChar *pOwner );
	void SetExtendComboState( ioBaseChar *pOwner );

protected:
	bool CheckEnableAngelOfDeathPowerAttackState( ioBaseChar *pOwner );
	void SetAngelOfDeathPowerAttackState( ioBaseChar *pOwner );

protected:
	bool CheckRunRotateState( ioBaseChar *pOwner );
	bool CheckRunRotateState2( ioBaseChar *pOwner );

	void SetRunRotateState( ioBaseChar *pOwner, bool bSendNet );
	void SetRunRotateStateEnd( ioBaseChar *pOwner );

	void CheckKeyInput( ioBaseChar *pOwner );
	void ProcessRotate( ioBaseChar *pOwner );

	bool CheckJumpRunRoateEnd( ioBaseChar *pOwner );

	bool CheckAniJump_Input_Jump_S( ioBaseChar *pOwner );
	bool CheckAniJump_Input_Jump_S_Exception(  ioBaseChar *pOwner );
	bool CheckAniDash_Input_Speical_Dash( ioBaseChar *pOwner );

protected:
	bool CheckPowerChargeState( ioBaseChar *pOwner );
	void SetPowerChargeState( ioBaseChar *pOwner, bool bSendNet );
	void SetPowerChargeAttack( ioBaseChar *pOwner );

	void CheckChargeInfo( ioWeapon *pWeapon );

protected:
	bool CheckEnablePhantomFlyState( ioBaseChar *pOwner );
	void SetPhantomFlyState( ioBaseChar *pOwner );

	//�����
protected:
	bool CheckEnableLuciferFlyState( ioBaseChar *pOwner );
	void SetLuciferFlyState( ioBaseChar *pOwner );

	void LoadLuciferFlyInfo( ioINILoader &rkLoader );

// michael fly
protected:
	bool CheckEnableMichaelFlyState( ioBaseChar *pOwner );
	void SetMichaelFlyState( ioBaseChar *pOwner );

	void LoadMichaelFlyInfo( ioINILoader &rkLoader );

//����ȣ 
protected:
	bool CheckEnableGoomihoFlyState( ioBaseChar *pOwner );
	void SetGoomihoFlyState( ioBaseChar *pOwner );

	void LoadGoomihoFlyInfo( ioINILoader &rkLoader );

//��ť����
protected:
	bool CheckEnableSuccubusState( ioBaseChar *pOwner );
	void SetSuccubusState( ioBaseChar *pOwner );
	
	void LoadSuccubusStateInfo( ioINILoader &rkLoader );

//���ŧ��
protected:
	bool CheckEnableDraculaState( ioBaseChar * pOwner );
	void SetDraculaState( ioBaseChar *pOwner );

	void LoadDraculaStateInfo( ioINILoader &rkLoader );

//�÷���
protected:
	bool CheckEnableFlashState( ioBaseChar * pOwner );
	void SetFlashState( ioBaseChar *pOwner );

//�̺�
protected:
	bool CheckEnableEvyState( ioBaseChar *pOwner );
	void SetEvyState( ioBaseChar *pOwner );

	void LoadEvyStateInfo( ioINILoader &rkLoader );
	
protected:
	bool CheckEnableSlasherState( ioBaseChar * pOwner );
	void SetSlasherState( ioBaseChar *pOwner );

	void LoadSlasherStateInfo( ioINILoader &rkLoader );

//�ź����
protected:
	bool CheckKaelthasExtendAttack( ioBaseChar *pOwner );
	void SetKaelthasExtendAttack( ioBaseChar *pOwner );

	void LoadKaelthasExtendAttackInfo( ioINILoader &rkLoader );


//ž����Ʈ
protected:
	void LoadTopBlDummyInfo( ioINILoader &rkLoader );
	bool CheckEnalbe_Top_Bl_ExtendMod( ioBaseChar *pOwner );
	bool CheckIsExtend_Top_Bl_mod( ioBaseChar *pOwner );

	void SetTopBlExtendAttack( ioBaseChar *pOwner );
	void SetTopBlExtendMod( ioBaseChar *pOwner );

	void ProcessTopBlReturn( ioBaseChar *pOwner );

	void SetTopBlDummyNormalMove( ioBaseChar *pOwner, bool bCombo, int nCombo );

//�ϵ���
	bool CheckEnableHadesState( ioBaseChar *pOwner );
	void SetHadesHalfChargeState( ioBaseChar *pOwner );
	void SetHadesFullChargeState( ioBaseChar *pOwner );

//����ġ
	void LoadJeonWooChiExtendDashInfo( ioINILoader &rkLoader );
	bool CheckEnableJeonWooChiChargeAttack( ioBaseChar *pOwner );
	void SetJeonWooChiChargeState( ioBaseChar *pOwner );
	void SetJeonWooChiDashState( ioBaseChar *pOwner );
	void SetJeonWooChiDashAttack( ioBaseChar *pOwner );
	void SetJeonWooChiDashAddAttack( ioBaseChar *pOwner );
	void OnJeconWooChiDashMove( ioBaseChar *pOwner );

//ȭ�к�
	void LoadChemistryInfo( ioINILoader &rkLoader );
	bool CheckEnableChemistyChargeAttack( ioBaseChar *pOwner );
	void SetChemistryChargeState( ioBaseChar *pOwner );
	
//�巡�� �׽�Ʈ ����
protected:
	void LoadDragonNestAssExtendInfo( ioINILoader &rkLoader );
	bool CheckDragonNestAssExtendState( ioBaseChar *pOwner );
	void CheckDragonNestAssExtendReserveKey( ioBaseChar *pOwner );

//RedHood
protected:
	void SetMacroTime( ioEntityGroup* pGrp, int iAniID, float fTimeRate, DWORD dwEndTime );
	
protected:
	void SetDragonNestAssExtendPreState( ioBaseChar *pOwner );
	void SetDragonNestAssExtendTeleportState( ioBaseChar *pOwner );

public:
	void SetDragonNestEnableTeleport( ioBaseChar *pOwner );

//�������
protected:
	bool CheckEnableThunderBirdState( ioBaseChar * pOwner );
	void SetThunderBirdState( ioBaseChar *pOwner );

//������
protected:
	bool CheckEnableEquipObjectItemState( ioBaseChar *pOwner );
	void SetObjectEquipState( ioBaseChar *pOwner );

//LeeDragonAttack
protected:
	bool CheckEnableLeeDragonAttackState( ioBaseChar *pOwner );
	void SetLeeDragonAttackState( ioBaseChar *pOwner );
	bool DecreaseLeeGauge( ioBaseChar *pOwner );
	void ProcessLeeDragonAttackState( ioBaseChar *pOwner );

//����ǥ
protected:
	void LoadParkilpyoNormalExtendInfo( ioINILoader &rkLoader );
	void ChangeToParkilpyoNormalGathering( ioBaseChar *pOwner );
	void ProcessParkilpyoNormalGathering( ioBaseChar *pOwner );
	void ChangeToParkilpyoNormalChargeAttack( ioBaseChar *pOwner );
	void ChangeToParkilpyoNormalChargeCombo( ioBaseChar *pOwner );

	bool CheckEnableParkilpyoNormalState( ioBaseChar * pOwner );
	void SetParkilpyoNormalEquipState( ioBaseChar *pOwner );

	bool CheckParkilpyoChargeComboInput( ioBaseChar *pOwner );
	void CheckParkilpyoCancelReserve( ioBaseChar *pOwner );

	void LoadParkilpyoObjectExtendInfo( ioINILoader &rkLoader );
	void ChangeToParkilpyoObjectChargeAttack( ioBaseChar *pOwner );

//��
protected:
	void LoadJackExtendInfo( ioINILoader &rkLoader );
	bool CheckEnableJackChargeAttack( ioBaseChar *pOwner );
	void ChangeToJackChargeAttack( ioBaseChar *pOwner );

//�����췹����
protected:
	void LoadShadowRangerExtendInfo( ioINILoader &rkLoader );
	bool CheckEnableShadowRangerState( ioBaseChar * pOwner );
	void SetShadowRangerMoveState( ioBaseChar *pOwner );

//����
protected:
	bool CheckEnableJumperState( ioBaseChar * pOwner );
	void SetJumperState( ioBaseChar *pOwner );

//���̹�
protected:
	bool CheckEnableSaberExtendState( ioBaseChar * pOwner );
	void SetSaberExtendState( ioBaseChar *pOwner );

//���˻�
protected:
	void ChangeToKeyInputAttack( ioBaseChar * pOwner );
	void ProcessKeyInputLastAttack( ioBaseChar *pOwner );
	bool CheckEnableMagicSwordAttackState( ioBaseChar * pOwner );
	void SetMagicSwordAttackState( ioBaseChar *pOwner );
	
//��Ÿ
protected:
	bool CheckEnableNataState( ioBaseChar * pOwner );
	void SetNataComboState( ioBaseChar *pOwner );


//������������
protected:
	bool CheckEnableResearcherRearFlyState( ioBaseChar * pOwner );
	void SetResearcherRearFlyState( ioBaseChar *pOwner );

//������ ���·� ����
protected:
	bool CheckMaxComboSetDelayState( ioBaseChar *pOwner );

//�����
protected:
	bool CheckEnableBullFightState( ioBaseChar * pOwner );
	void SetBullFightState( ioBaseChar *pOwner );

//Sawang
protected:
	bool CheckEnableSawangState( ioBaseChar *pOwner );
	void SetSawangState( ioBaseChar *pOwner );

//Strider
protected:
	bool CheckEnableStriderExtendState( ioBaseChar * pOwner );
	void SetStriderExtendState( ioBaseChar *pOwner );

//�İ��
protected:
	bool CheckEnableApostateMonkExtendState( ioBaseChar * pOwner );
	void SetApostateMonkExtendState( ioBaseChar *pOwner );

//���Ǹ�
	void LoadCuchulainInfo( ioINILoader &rkLoader );
	void MiddleAimInit_Cuchulain( ioBaseChar *pOwner );
	void CheckMiddleCharge_Cuchulain( ioBaseChar *pOwner );
	void ClearMiddleAim_Cuchulain( ioBaseChar *pOwner );
	void ChangeCuchulainMiddleAttack( ioBaseChar *pOwner );
	void ProcessCuchulainMiddleAttack( ioBaseChar *pOwner );
	void SetCuchulainSpecialMove( ioBaseChar *pOwner );
	bool IsEnableCuchulainSpecialMove( ioBaseChar *pOwner );
	ioHashString GetCuchulainMiddleTarget( ioBaseChar *pOwner );
	void CuchulainChargeBuffAttack( ioBaseChar *pOwner );

protected:
	void LoadOniMushaInfo( ioINILoader &rkLoader );
	void SetOniMushaObjectEquipState( ioBaseChar *pOwner );
	bool CheckEnableOniMushaHalfChargeState( ioBaseChar *pOwner );
	bool CheckEnableOniMushaRunState( ioBaseChar *pOwner );
	void ChangeToOniMushaRunState( ioBaseChar *pOwner );
	void ChangeToOniMushaChargeAttack( ioBaseChar *pOwner );
	void ProcessOniMushaChargeAttack( ioBaseChar *pOwner );

//����̵�
protected:
	bool CheckEnableDruidBearState( ioBaseChar *pOwner );
	void SetDruidBearState( ioBaseChar *pOwner );

protected:
	bool CheckEnableCrowSpecialDashState( ioBaseChar *pOwner );
	void SetCrowSpecialDashState( ioBaseChar *pOwner );

//�����Ǿ�
protected:
	void CheckSpecialStateByExtendType( ioBaseChar *pOwner );

//���ǽ���
protected:
	bool CheckEnableMephistoSpecialDashState( ioBaseChar *pOwner );
	void SetMephistoSpecialDashState( ioBaseChar *pOwner );

//�� D~
protected:
	void ChangeHalfExtendAttack( ioBaseChar *pOwner );

protected:
	bool CheckEnableSasinmuState( ioBaseChar *pOwner );
	void SetSasinmuState( ioBaseChar *pOwner );

protected:
	bool CheckEnablePredatorState( ioBaseChar *pOwner );
	void SetPredatorState( ioBaseChar *pOwner );

	//����
public:
	bool IsCanPlayWoundedAniFromExtendDash();
	bool IsNoDropState();

protected:
	bool CheckCanComboDash( ioBaseChar *pOwner );

	void SetRunRotateBuff( ioBaseChar *pOwner );
	void RemoveRundRotateBuff( ioBaseChar *pOwner );

public:
	ioChargeComboDash();
	ioChargeComboDash( const ioChargeComboDash &rhs );
	virtual ~ioChargeComboDash();
};

inline ioChargeComboDash* ToChargeComboDash( ioExtendDash *pDash )
{
	if( !pDash || pDash->GetType() != DT_CHARGE_COMBO )
		return NULL;

	return dynamic_cast< ioChargeComboDash* >( pDash );
}


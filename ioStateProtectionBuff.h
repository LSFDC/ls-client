#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioStateProtectionBuff : public ioBuff
{
public:
	// 숫자가 높을수록 우선권을 가진다
	enum HPProtectType
	{
		HPT_ZERO_CANCEL		= 1,
		HPT_HP_NO_CANCEL	= 2,
	};

protected:
	float m_fOutLineR;
	float m_fOutLineG;
	float m_fOutLineB;
	float m_fOutLineAlpha;
	float m_fOutLineThickness;
	bool m_bEnableOutLine;

	CEncrypt<float> m_fDamageRate;
	CEncrypt<bool> m_bNoBlockState;
	CEncrypt<bool> m_bNoDefenseBreakState;

	HPProtectType m_HPCancelType;

	AttackSide m_ProtectSide;

	bool m_bDisableDownState;

	bool m_bNoEmoticon;

	bool m_bNoCancel;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void SetExtraDurationByGrowth( float fDuration );

	virtual void StartBuff( ioBaseChar *pOwner );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );

	float GetProtectionRate();
	inline const bool IsNoEmoticon() const { return m_bNoEmoticon; }
	inline const bool IsNoBlockState() const { return m_bNoBlockState; }
	inline const bool IsNoDefenseBreakState() const { return m_bNoDefenseBreakState; }
	inline AttackSide GetProtectSide() { return m_ProtectSide; }
	inline int GetHPCancelType() const { return m_HPCancelType; }

	virtual void CheckActiveCount();

protected:
	virtual void SetAction();
	virtual void ProcessDelay( float fTimePerSec );

	bool CheckOwnerStateCheck();

public:
	bool IsNoCancel();

public:
	ioStateProtectionBuff();
	ioStateProtectionBuff( const ioStateProtectionBuff &rhs );
	virtual ~ioStateProtectionBuff();

};

inline ioStateProtectionBuff* ToStateProtectBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_STATE_PROTECTION )
		return NULL;

	return dynamic_cast< ioStateProtectionBuff* >( pBuff );
}


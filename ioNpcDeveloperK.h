#pragma once


class ioNpcChar;
class ioNpcDeveloperK : public ioNpcChar
{
public:
	virtual void Initialize( const ITEM_DATA *pItem );

public:
	virtual NpcType GetNpcType() const;

public:
    virtual bool IsDropDamage(){ return true; }         //������ ������K�� ����������� �ް� ������ NPC�� ��������� ����.

protected:
	virtual void ProcessAI();

public:
	virtual void Update( float fTimePerSec, DWORD dwPreTime, DWORD dwCurTime );

public:
	virtual DWORD GetNoInputDelayTime();
	virtual DWORD GetLimiteDieFlyTime();
	virtual float GetDieActionRate();
	virtual float GetDieDownActionRate();

public:
	virtual const ioHashString &GetAniDieAction();
	virtual const ioHashString &GetAniBackDieAction();
	virtual const ioHashString &GetAniBlowDieAction();
	virtual const ioHashString &GetAniBackBlowDieAction();
	virtual const ioHashString &GetAniDownBlowDieAction();
	virtual const ioHashString &GetAniBackdownBlowDieAction();

public:
	virtual int GetDelayAnimationIdx() const;

public:
	virtual void FaceChange( const char *szTexPostfix, int iDuration );

public:
	ioNpcDeveloperK( ioEntityGroup *pGrp, ioPlayStage *pCreator, ioPlayMode *pMode );
	virtual ~ioNpcDeveloperK();
};

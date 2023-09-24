#pragma once

#include "ioBuff.h"

class ioBaseChar;
class ioINILoader;

class ioDestroyCallBuff : public ioBuff
{
protected:
	
	//��� ����Ʈ, ����Ʈ ���� ��ȣ
	ioHashString m_szDelayEffect;
	DWORD m_dwDelayEffectID;
	
	//���� ���� ���� �̸�
	ioHashString m_szRemoveBuffName;
	//����Ű�Է½� �����Ұ��ΰ�
	bool m_bIsUseKeyDownDestroy;
	//���� �ð���?
	DWORD m_dwDestroyStartTime;
	DWORD m_dwDestroyEndTime;

	bool m_bRcvDestroy;
	bool m_bDestroySeted;

public:
	virtual void LoadProperty( ioINILoader &rkLoader );
	virtual ioBuff* Clone();

public:
	virtual void StartBuff( ioBaseChar *pOwner );
	virtual bool ApplyStartBuff( ioBaseChar *pOwner, DWORD dwGlobalTime, DWORD dwGapTime );
	virtual void ProcessBuff( float fTimePerSec );
	virtual void EndBuff();

protected:
	bool CheckDestroy( ioBaseChar *pOwner );
	bool CheckDestroyTime();

	//�߰�
protected:
	//������ �б�
	void LoadDestroyProperty( ioINILoader &rkLoader );

public:
	void ApplyExtraBuffInfo( SP2Packet &rkPacket );

public:
	ioDestroyCallBuff();
	ioDestroyCallBuff( const ioDestroyCallBuff &rhs );
	virtual ~ioDestroyCallBuff();
};

inline ioDestroyCallBuff* ToDestroyCallBuff( ioBuff *pBuff )
{
	if( !pBuff || pBuff->GetTypeID() != BT_CALL_DESTROY )
		return NULL;

	return dynamic_cast< ioDestroyCallBuff* >( pBuff );
}
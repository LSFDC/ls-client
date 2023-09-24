#pragma once


#include "EncryptValue.h"
#include "ioCustomSoundDefine.h"
#include "ioFileWriteClient.h"


#ifdef SRC_SA
#include "Apex/ioApex.h"
#endif

struct CHARACTER;
struct ControlKeys;

class SP2Packet : public CPacket
{
public:
	// >> 쪽으로 current_pos 이동
	void  MovePointer( DWORD dwMoveBytes );
	DWORD StrToDwordIP( char *iip );
	void  DwordToStrIP( DWORD dwIP, char *iip );
	void  InitCurPos();
	DWORD GetUDPIndex();

#ifdef ANTIHACK
	//rudp 패킷 전용
	bool  IsUserTarget();
	DWORD GetUserIndex();
	DWORD GetNPCIndex();

	void  CreateRUDPPacket( DWORD dwSeed, DWORD dwTime, DWORD dwNPCIndex );
	DWORD GetSeed();
	DWORD GetHostTime();
#endif

public:
	SP2Packet& operator = ( const SP2Packet &packet );

public:
	SP2Packet& operator << (BYTE arg);
	SP2Packet& operator << (bool arg);
	SP2Packet& operator << (int arg);
	SP2Packet& operator << (LONG arg);
	SP2Packet& operator << (WORD arg);
	SP2Packet& operator << (DWORD arg);
	SP2Packet& operator << (__int64 arg);
	SP2Packet& operator << (LPTSTR arg);
	SP2Packet& operator << (double arg);
	SP2Packet& operator << (float arg);
	SP2Packet& operator << (short arg);
	SP2Packet& operator << ( const char *arg );
	SP2Packet& operator << ( const ioHashString &arg );
	SP2Packet& operator << ( const D3DXVECTOR2 &arg );
	SP2Packet& operator << ( const D3DXVECTOR3 &arg );
	SP2Packet& operator << ( const D3DXQUATERNION &arg );
	SP2Packet& operator << ( CHARACTER &arg );
	SP2Packet& operator << ( const VOICEDATA &arg );
	SP2Packet& operator << ( const CustomSoundPacket &arg);
	SP2Packet& operator << (CEncrypt<BYTE> &arg);
	SP2Packet& operator << (CEncrypt<bool> &arg);
	SP2Packet& operator << (CEncrypt<int> &arg);
	SP2Packet& operator << (CEncrypt<long> &arg);
	SP2Packet& operator << (CEncrypt<WORD> &arg);
	SP2Packet& operator << (CEncrypt<DWORD> &arg);
	SP2Packet& operator << (CEncrypt<__int64> &arg);
	SP2Packet& operator << (CEncrypt<double> &arg);
	SP2Packet& operator << (CEncrypt<float> &arg);

	SP2Packet& operator << ( const FilePacket &arg );
	SP2Packet& operator << ( const ControlKeys &arg );


public:
	SP2Packet& operator >> (BYTE &arg);
	SP2Packet& operator >> (bool &arg);
	SP2Packet& operator >> (int &arg);
	SP2Packet& operator >> (LONG &arg);
	SP2Packet& operator >> (WORD &arg);
	SP2Packet& operator >> (DWORD &arg);
	SP2Packet& operator >> (__int64 &arg);
	SP2Packet& operator >> (LPTSTR arg);
	SP2Packet& operator >> (double &arg);
	SP2Packet& operator >> (float &arg);
	SP2Packet& operator >> (short &arg);
	SP2Packet& operator >> ( ioHashString &arg );
	SP2Packet& operator >> ( D3DXVECTOR2 &arg );
	SP2Packet& operator >> ( D3DXVECTOR3 &arg );
	SP2Packet& operator >> ( D3DXQUATERNION &arg );
	SP2Packet& operator >> ( CHARACTER &arg );
	SP2Packet& operator >> ( VOICEDATA &arg );
	SP2Packet& operator >> ( CustomSoundPacket &arg);
	SP2Packet& operator >> (CEncrypt<BYTE> &arg);
	SP2Packet& operator >> (CEncrypt<bool> &arg);
	SP2Packet& operator >> (CEncrypt<int> &arg);
	SP2Packet& operator >> (CEncrypt<long> &arg);
	SP2Packet& operator >> (CEncrypt<WORD> &arg);
	SP2Packet& operator >> (CEncrypt<DWORD> &arg);
	SP2Packet& operator >> (CEncrypt<__int64> &arg);
	SP2Packet& operator >> (CEncrypt<double> &arg);
	SP2Packet& operator >> (CEncrypt<float> &arg);

	SP2Packet& operator >> ( FilePacket &arg );
	SP2Packet& operator >> ( ControlKeys &arg );



public:
	SP2Packet();
	SP2Packet( const SP2Packet &rhs );
	SP2Packet( DWORD dwID );
	SP2Packet( char *szBuf, int iSize );
	SP2Packet( char *szIP, DWORD dwPort, DWORD dwUserIndex, int iIndex, const SP2Packet &rhs );
	virtual ~SP2Packet();
};




#include "stdafx.h"

#include "SP2Packet.h"

SP2Packet::SP2Packet()
{
}

SP2Packet::SP2Packet( const SP2Packet &rhs )
{
	Clear();
	memcpy( m_pBuffer, rhs.GetBuffer(), rhs.GetBufferSize() );
	m_currentPos = rhs.m_currentPos;
}

SP2Packet::SP2Packet( DWORD ID )
: CPacket( ID )
{
}

SP2Packet::SP2Packet( char *buffer, int size )
: CPacket( buffer, size )
{
}

SP2Packet::SP2Packet( char *szIP, DWORD dwPort, DWORD dwUserIndex, int iIndex, const SP2Packet &rhs )
{
	Clear();

	*m_packet_header.m_ID = rhs.GetPacketID();
	m_currentPos = sizeof(PACKETHEADER);

	*this << StrToDwordIP(szIP) << dwPort << dwUserIndex << iIndex;

	SetDataAdd( (char*)rhs.GetData(), rhs.GetDataSize() );
}

SP2Packet::~SP2Packet()
{
}

void SP2Packet::MovePointer( DWORD dwMoveBytes )
{
	m_currentPos += dwMoveBytes;
	m_currentPos = min( m_currentPos, GetBufferSize() );
}

DWORD SP2Packet::StrToDwordIP( char *iip )
{
	int  count       = 0;
	int  cut_ip		 = 0;
	char szCut_ip[4][4];
	memset(szCut_ip,0,sizeof(szCut_ip));
	int  len	     = strlen(iip);
	for(int i = 0;i < len;i++)
	{
		if(iip[i] == '.')
		{
			count = 0;
			cut_ip++;
		}
		else
			szCut_ip[cut_ip][count++] = iip[i];
	}
	return (DWORD)(atoi(szCut_ip[0])<<24) | (DWORD)(atoi(szCut_ip[1])<<16) | (DWORD)(atoi(szCut_ip[2])<<8) | atoi(szCut_ip[3]);	
}

void SP2Packet::DwordToStrIP( DWORD dwIP, char *iip )
{
	sprintf(iip,"%d.%d.%d.%d",(dwIP & 0xff000000)>>24,(dwIP & 0x00ff0000)>>16,(dwIP & 0x0000ff00)>>8,(dwIP & 0xff));
}

void SP2Packet::InitCurPos()
{
	m_currentPos = sizeof(PACKETHEADER);
}

//operator
SP2Packet& SP2Packet::operator = ( const SP2Packet &packet )
{
	Clear();
	memcpy( m_pBuffer, packet.GetBuffer(), packet.GetBufferSize() );
	m_currentPos = packet.m_currentPos;

	return *this;
}

SP2Packet&  SP2Packet::operator << (BYTE arg)
{
	if( !CheckLeftPacketSize( sizeof(BYTE) ) )
		return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(BYTE));
	m_currentPos += sizeof(BYTE);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (bool arg)
{
	if( !CheckLeftPacketSize( sizeof(bool) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(bool));
	m_currentPos += sizeof(bool);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (int arg)
{
	if( !CheckLeftPacketSize( sizeof(int) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(int));
	m_currentPos += sizeof(int);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (LONG arg)
{
	if( !CheckLeftPacketSize( sizeof(LONG) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(LONG));
	m_currentPos += sizeof(LONG);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (WORD arg)
{
	if( !CheckLeftPacketSize( sizeof(WORD) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(WORD));
	m_currentPos += sizeof(WORD);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (DWORD arg)
{
	if( !CheckLeftPacketSize( sizeof(DWORD) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(DWORD));
	m_currentPos += sizeof(DWORD);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (__int64 arg)
{
	if( !CheckLeftPacketSize( sizeof(__int64) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(__int64));
	m_currentPos += sizeof(__int64);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (LPTSTR arg)
{
	int nlen = lstrlen(arg) * sizeof( TCHAR ) + sizeof( TCHAR );

	if( !CheckLeftPacketSize( nlen ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],arg,nlen);
	m_currentPos += nlen;
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (double arg)
{
	if( !CheckLeftPacketSize( sizeof(double) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(double));
	m_currentPos += sizeof(double);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (float arg)
{
	if( !CheckLeftPacketSize( sizeof(float) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(float));
	m_currentPos += sizeof(float);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (short arg)
{
	if( !CheckLeftPacketSize( sizeof(short) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(short));
	m_currentPos += sizeof(short);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << ( const char *arg )
{
	int nlen = lstrlen(arg) + 1;

	if( !CheckLeftPacketSize( nlen ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],arg,nlen);
	m_currentPos += nlen;
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const ioHashString &arg )
{
	int nlen = arg.Length() + 1;

	if( !CheckLeftPacketSize( nlen ) )
		return *this;

	memcpy( &m_pBuffer[m_currentPos], arg.c_str(), nlen - 1 );
	m_pBuffer[m_currentPos + nlen - 1 ] = '\0';

	m_currentPos += nlen;
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const D3DXVECTOR2 &arg )
{
	if( !CheckLeftPacketSize( sizeof(D3DXVECTOR2) ) ) return *this;

	memcpy( &m_pBuffer[m_currentPos], &arg, sizeof(D3DXVECTOR2) );
	m_currentPos += sizeof(D3DXVECTOR2);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const D3DXVECTOR3 &arg )
{
	if( !CheckLeftPacketSize( sizeof(D3DXVECTOR3) ) ) return *this;

	memcpy( &m_pBuffer[m_currentPos], &arg, sizeof(D3DXVECTOR3) );
	m_currentPos += sizeof(D3DXVECTOR3);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const D3DXQUATERNION &arg )
{
	if( !CheckLeftPacketSize( sizeof(D3DXQUATERNION) ) ) return *this;

	memcpy( &m_pBuffer[m_currentPos], &arg, sizeof(D3DXQUATERNION) );
	m_currentPos += sizeof(D3DXQUATERNION);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( CHARACTER &arg )
{
	arg.FillData( *this );
	return *this;
}

SP2Packet& SP2Packet::operator << ( const VOICEDATA &arg )
{
	if( !CheckLeftPacketSize( sizeof(VOICEDATA) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(VOICEDATA));
	m_currentPos += sizeof(VOICEDATA);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const CustomSoundPacket &arg )
{
	int iSize  = sizeof( arg.m_dwWrittenSize );
	int iTotal = arg.m_dwWrittenSize + iSize;
	if( !CheckLeftPacketSize( iTotal ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg.m_dwWrittenSize, iSize );
	m_currentPos += iSize;
	memcpy(&m_pBuffer[m_currentPos],arg.m_Data,arg.m_dwWrittenSize);
	m_currentPos += arg.m_dwWrittenSize;
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<BYTE> &arg)
{
	if( !CheckLeftPacketSize( sizeof(BYTE) ) ) return *this;

	BYTE decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(BYTE));
	m_currentPos += sizeof(BYTE);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<bool> &arg)
{
	if( !CheckLeftPacketSize( sizeof(bool) ) ) return *this;

	bool decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(bool));
	m_currentPos += sizeof(bool);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<int> &arg)
{
	if( !CheckLeftPacketSize( sizeof(int) ) ) return *this;

	int decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(int));
	m_currentPos += sizeof(int);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<long> &arg)
{
	if( !CheckLeftPacketSize( sizeof(long) ) ) return *this;

	long decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(long));
	m_currentPos += sizeof(long);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<WORD> &arg)
{
	if( !CheckLeftPacketSize( sizeof(WORD) ) ) return *this;

	WORD decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(WORD));
	m_currentPos += sizeof(WORD);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<DWORD> &arg)
{
	if( !CheckLeftPacketSize( sizeof(DWORD) ) ) return *this;

	DWORD decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(DWORD));
	m_currentPos += sizeof(DWORD);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<__int64> &arg)
{
	if( !CheckLeftPacketSize( sizeof(__int64) ) ) return *this;

	__int64 decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(__int64));
	m_currentPos += sizeof(__int64);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet& SP2Packet::operator << (CEncrypt<double> &arg)
{
	if( !CheckLeftPacketSize( sizeof(double) ) ) return *this;

	double decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(double));
	m_currentPos += sizeof(double);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

SP2Packet&  SP2Packet::operator << (CEncrypt<float> &arg)
{
	if( !CheckLeftPacketSize( sizeof(float) ) ) return *this;

	float decryptValue = arg;
	memcpy(&m_pBuffer[m_currentPos],&decryptValue,sizeof(float));
	m_currentPos += sizeof(float);
	*m_packet_header.m_Size = m_currentPos;
	return *this;
}

#ifdef XTRAP
SP2Packet& SP2Packet::operator << ( const XtrapPacket &arg )
{
	if( !CheckLeftPacketSize( sizeof(XtrapPacket) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(XtrapPacket));
	m_currentPos += sizeof(XtrapPacket);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}
#endif 
SP2Packet& SP2Packet::operator << ( const FilePacket &arg )
{
	if( !CheckLeftPacketSize( sizeof(FilePacket) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(FilePacket));
	m_currentPos += sizeof(FilePacket);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}

SP2Packet& SP2Packet::operator << ( const ControlKeys &arg )
{
	if( !CheckLeftPacketSize( sizeof(ControlKeys) ) ) return *this;

	memcpy(&m_pBuffer[m_currentPos],&arg,sizeof(ControlKeys));
	m_currentPos += sizeof(ControlKeys);
	*m_packet_header.m_Size = m_currentPos;

	return *this;
}



//-----------------------------------------------------------------
SP2Packet&  SP2Packet::operator >> (BYTE &arg)
{
	if( !CheckRightPacketSize( sizeof(BYTE) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(BYTE));
	m_currentPos += sizeof(BYTE);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (bool &arg)
{
	if( !CheckRightPacketSize( sizeof(bool) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(bool));
	m_currentPos += sizeof(bool);

	return *this;
}

SP2Packet&  SP2Packet::operator >> (int &arg)
{
	if( !CheckRightPacketSize( sizeof(int) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(int));
	m_currentPos += sizeof(int);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (LONG &arg)
{
	if( !CheckRightPacketSize( sizeof(LONG) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(LONG));
	m_currentPos += sizeof(LONG);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (WORD &arg)
{
	if( !CheckRightPacketSize( sizeof(WORD) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(WORD));
	m_currentPos += sizeof(WORD);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (DWORD &arg)
{
	if( !CheckRightPacketSize( sizeof(DWORD) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(DWORD));
	m_currentPos += sizeof(DWORD);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (__int64 &arg)
{
	if( !CheckRightPacketSize( sizeof(__int64) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(__int64));
	m_currentPos += sizeof(__int64);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (LPTSTR arg)
{
	int nlen = lstrlen((LPTSTR)&m_pBuffer[m_currentPos]) * sizeof( TCHAR ) + sizeof( TCHAR );

	if( !CheckRightPacketSize( nlen ) ) return *this;

	memcpy(arg,&m_pBuffer[m_currentPos],nlen);
	m_currentPos += nlen;
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (double &arg)
{
	if( !CheckRightPacketSize( sizeof(double) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(double));
	m_currentPos += sizeof(double);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (float &arg)
{
	if( !CheckRightPacketSize( sizeof(float) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(float));
	m_currentPos += sizeof(float);
	
	return *this;
}

SP2Packet&  SP2Packet::operator >> (short &arg)
{
	if( !CheckRightPacketSize( sizeof(short) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(short));
	m_currentPos += sizeof(short);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( ioHashString &arg )
{
	int nlen = lstrlen((LPTSTR)&m_pBuffer[m_currentPos]) * sizeof( TCHAR ) + sizeof( TCHAR );

	if( !CheckRightPacketSize( nlen ) ) return *this;

	arg = &m_pBuffer[m_currentPos];
	m_currentPos += nlen;
	
	return *this;
}

SP2Packet& SP2Packet::operator >> ( D3DXVECTOR2 &arg )
{
	if( !CheckRightPacketSize( sizeof(D3DXVECTOR2) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(D3DXVECTOR2));
	m_currentPos += sizeof(D3DXVECTOR2);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( D3DXVECTOR3 &arg )
{
	if( !CheckRightPacketSize( sizeof(D3DXVECTOR3) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(D3DXVECTOR3));
	m_currentPos += sizeof(D3DXVECTOR3);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( D3DXQUATERNION &arg )
{
	if( !CheckRightPacketSize( sizeof(D3DXQUATERNION) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(D3DXQUATERNION));
	m_currentPos += sizeof(D3DXQUATERNION);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( CHARACTER &arg )
{
	arg.ApplyData( *this );
	return *this;
}

SP2Packet& SP2Packet::operator >> ( VOICEDATA &arg )
{
	if( !CheckRightPacketSize( sizeof(VOICEDATA) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(VOICEDATA));
	m_currentPos += sizeof(VOICEDATA);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( CustomSoundPacket &arg )
{
	int iSize  = sizeof( arg.m_dwWrittenSize );
	if( !CheckRightPacketSize( iSize ) ) return *this;
	memcpy(&arg.m_dwWrittenSize,&m_pBuffer[m_currentPos], iSize);
	m_currentPos += iSize;

	if( !CheckRightPacketSize( arg.m_dwWrittenSize ) ) return *this;
	memcpy(arg.m_Data,&m_pBuffer[m_currentPos],arg.m_dwWrittenSize);
	m_currentPos += arg.m_dwWrittenSize;
	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<BYTE> &arg)
{
	if( !CheckRightPacketSize( sizeof(BYTE) ) ) return *this;

	BYTE decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(BYTE));
	m_currentPos += sizeof(BYTE);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<bool> &arg)
{
	if( !CheckRightPacketSize( sizeof(bool) ) ) return *this;

	bool decryptValue = false;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(bool));
	m_currentPos += sizeof(bool);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<int> &arg)
{
	if( !CheckRightPacketSize( sizeof(int) ) ) return *this;

	int decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(int));
	m_currentPos += sizeof(int);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<long> &arg)
{
	if( !CheckRightPacketSize( sizeof(long) ) ) return *this;

	long decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(long));
	m_currentPos += sizeof(long);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<WORD> &arg)
{
	if( !CheckRightPacketSize( sizeof(WORD) ) ) return *this;

	WORD decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(WORD));
	m_currentPos += sizeof(WORD);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<DWORD> &arg)
{
	if( !CheckRightPacketSize( sizeof(DWORD) ) ) return *this;

	DWORD decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(DWORD));
	m_currentPos += sizeof(DWORD);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<__int64> &arg)
{
	if( !CheckRightPacketSize( sizeof(__int64) ) ) return *this;

	__int64 decryptValue = 0;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(__int64));
	m_currentPos += sizeof(__int64);
	arg = decryptValue;

	return *this;
}

SP2Packet& SP2Packet::operator >> (CEncrypt<double> &arg)
{
	if( !CheckRightPacketSize( sizeof(double) ) ) return *this;

	double decryptValue = 0.0f;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(double));
	m_currentPos += sizeof(double);
	arg = decryptValue;

	return *this;
}

SP2Packet&  SP2Packet::operator >> (CEncrypt<float> &arg)
{
	if( !CheckRightPacketSize( sizeof(float) ) ) return *this;

	float decryptValue = 0.0f;
	memcpy(&decryptValue,&m_pBuffer[m_currentPos],sizeof(float));
	m_currentPos += sizeof(float);
	arg = decryptValue;

	return *this;
}

#ifdef XTRAP
SP2Packet& SP2Packet::operator >> ( XtrapPacket &arg )
{
	if( !CheckRightPacketSize( sizeof(XtrapPacket) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(XtrapPacket));
	m_currentPos += sizeof(XtrapPacket);

	return *this;
}
#endif 
SP2Packet& SP2Packet::operator >> ( FilePacket &arg )
{
	if( !CheckRightPacketSize( sizeof(FilePacket) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(FilePacket));
	m_currentPos += sizeof(FilePacket);

	return *this;
}

SP2Packet& SP2Packet::operator >> ( ControlKeys &arg )
{
	if( !CheckRightPacketSize( sizeof(ControlKeys) ) ) return *this;

	memcpy(&arg,&m_pBuffer[m_currentPos],sizeof(ControlKeys));
	m_currentPos += sizeof(ControlKeys);

	return *this;
}


DWORD SP2Packet::GetUDPIndex()
{	
	//헤더 + ip + port + userindex + udpindex 
	DWORD dwHostTimePosEnd = sizeof(PACKETHEADER) + 16;
	if( *m_packet_header.m_Size < dwHostTimePosEnd )
	{
		assert(0);
		LOG.PrintConsoleLog( 0, "[INNI][PACKET] GetUDPIndex Error, ID(%x)", GetPacketID() );
		return 0;
	}

	DWORD dwUDPIndex;
	memcpy(&dwUDPIndex,&m_pBuffer[dwHostTimePosEnd-4],4);
	return dwUDPIndex;

}

#ifdef ANTIHACK
void SP2Packet::CreateRUDPPacket( DWORD dwSeed, DWORD dwTime, DWORD dwNPCIndex )
{
	//기존 내용을 뒤로 밀고 앞쪽에 채워줌
	
	//BYTE tempBuffer[MAX_BUFFER] = {0,};
	// udp 패킷 중엔 1024 넘는게 없을듯..
	static char tempBuffer[1024] = {0,};
	int dataSize = GetDataSize();
	if( dataSize > 1024 )
	{
		assert(0);
		LOG.PrintTimeAndLog( 0, "[err] createUdpPacket faild. size over - id(%x), size(%u)", GetPacketID(), dataSize );
		return;
	}
	memcpy( tempBuffer, GetData(), dataSize );

	InitCurPos();
	*this << dwSeed << dwTime << dwNPCIndex;
	SetDataAdd( tempBuffer, dataSize );
}

bool SP2Packet::IsUserTarget()
{
	//헤더 + ip + port + userindex + udpindex 
	DWORD dwNPCIndexPosEnd = PK_HEADER_SIZE + 16;
	if( *m_packet_header.m_Size < dwNPCIndexPosEnd )
	{
		//잘못된거!!!!
		assert(0);
		return true;
	}

	DWORD dwNPCIndex;
	memcpy(&dwNPCIndex,&m_pBuffer[dwNPCIndexPosEnd-4],4);
	if( dwNPCIndex == 0)
		return true;

	return false;
}

DWORD SP2Packet::GetUserIndex()
{
	//헤더 + ip + port + userindex
	DWORD dwUserIndexPosEnd = PK_HEADER_SIZE + 12;
	if( *m_packet_header.m_Size < dwUserIndexPosEnd )
	{
		//잘못된거!!!!
		assert(0);
		return 0;
	}
	DWORD dwValue;
	memcpy(&dwValue,&m_pBuffer[dwUserIndexPosEnd-4],4);

	return dwValue;	
}

DWORD SP2Packet::GetSeed()
{
	//헤더 + ip + port + userindex + udpindex > seed
	DWORD dwPos = PK_HEADER_SIZE + 20;
	if( *m_packet_header.m_Size < dwPos )
	{
		//잘못된거!!!!
		assert(0);
		return 0;
	}
	DWORD dwValue;
	memcpy(&dwValue,&m_pBuffer[dwPos-4],4);

	return dwValue;
}

DWORD SP2Packet::GetHostTime()
{
	//헤더 + ip + port + userindex + udpindex + seed > time
	DWORD dwPos = PK_HEADER_SIZE + 24;
	if( *m_packet_header.m_Size < dwPos )
	{
		//잘못된거!!!!
		assert(0);
		return 0;
	}
	DWORD dwValue;
	memcpy(&dwValue,&m_pBuffer[dwPos-4],4);

	return dwValue;
}

DWORD SP2Packet::GetNPCIndex()
{
	//헤더 + ip + port + userindex + 
	DWORD dwUserIndexPosEnd = PK_HEADER_SIZE + 12;
	if( *m_packet_header.m_Size < dwUserIndexPosEnd )
	{
		//잘못된거!!!!
		assert(0);
		return 0;
	}
	DWORD dwValue;
	memcpy(&dwValue,&m_pBuffer[dwUserIndexPosEnd-4],4);

	return dwValue;	
}

#endif
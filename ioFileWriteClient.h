#pragma once


#define WM_FILESOCKET   (WM_USER + 13)

class SP2Packet;
class ioFileSocket : public ioClientTCPSocket
{
protected:
	DWORD   m_dwProcID;

public:
	virtual void ConnectProcess();
	virtual void DisconnectProcess();
	virtual void ProcessPacket( CPacket &rkPacket );
	virtual bool CheckNS( CPacket &rkPacket );

public:
	DWORD GetProcID(){ return m_dwProcID; }

public:
	ioFileSocket( DWORD dwProcID );
	virtual ~ioFileSocket();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ioFileWriteClient : public Singleton< ioFileWriteClient >
{
public:
	enum
	{
		MAX_PACKET_BUF             = 512,
		RETURN_WND_PARAM_COMPLETE  = 10001,
		RETURN_WND_PARAM_FAIL      = 10002,
	};

protected:
	ioFileSocket   *m_pSocket;

	HANDLE          m_hFile;
	DWORD           m_dwFileSize;
	DWORD           m_dwReadSize;
	DWORD           m_dwWriteSendTime;

	DWORD           m_dwFileCRC;
	ioHashString    m_szWriteFileName;

	DWORD           m_dwFileWidth;
	DWORD           m_dwFileHeight;

	ioHashString    m_szServerIP;
	int             m_iServerPort;

	// ioWnd * ��� ID�� ������ ���� ioWnd*�� tangle point�� �� �� �ִ�.
	DWORD           m_dwReturnParentID;
	DWORD           m_dwReturnID;

	DWORD           m_dwConnectCheckTime;

protected:
	void InitData();

	void OnFileWrite( SP2Packet &rkPacket );
	bool SendLoop( SP2Packet &rkPacket );
	void SetReturnWndParam( int iParam );
	void ProcessConnectCheck();

public:
	static ioFileWriteClient &GetSingleton();

public:
	void LoadINI();

	bool Start( const char *szFilePath, const char *szWriteFileName, DWORD dwMaxFileSize, DWORD dwFileWidth, DWORD dwFileHeight, DWORD dwReturnParentID, DWORD dwReturnID );
	void Process();
	
	bool EvaluateSocket( WPARAM wParam, LPARAM lParam );
	void RecvPacket( SP2Packet &rkPacket );
                             
	void ConnectProcess();
	void DisconnectProcess(); 

	//������ ���Ų ���ε� ���� (�ӽ� ��� �Ұ���)
protected:
	enum TempState 
	{
		TS_NEW,
		TS_SEND,
		TS_END,
	};

	//�ӽ÷� ���� �ڽ��� Ŀ���� ���ϸ��
	ioHashStringVec m_TempCustomFileList;
	int m_nMaxSubCnt;
	ioHashString m_szTempUploadDir;
	TempState m_TempState;
	bool m_bTempSend;
	bool m_bSetTempUpload;
	
public:
	bool IsFileCheck( const ioHashString &rkFileName );
	void CreateUploadList( DWORDVec &dwCustomIdxList );
	bool TempStart( const char *szWriteFileName, DWORD dwMaxFileSize );
	void ProcessTemp();
	void ProcessTempSend();
	void NextUploadList();
	void OnTempFileWrite( SP2Packet &rkPacket );

public:
	ioFileWriteClient(void);
	virtual ~ioFileWriteClient(void);
};

typedef struct tagFilePacket
{
	char m_FilePacket[ioFileWriteClient::MAX_PACKET_BUF];

	tagFilePacket()
	{
		ZeroMemory( m_FilePacket, sizeof( m_FilePacket ) );
	}
}FilePacket;


#define g_FileWriteClient ioFileWriteClient::GetSingleton()


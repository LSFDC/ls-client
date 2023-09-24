#pragma once

#include "../io3DEngine/ioLocalManagerParent.h"


class ioLocalParent;

class ioLocalManager : public ioLocalManagerParent, public Singleton< ioLocalManager >
{
protected:
	typedef std::vector< ioLocalParent* > vLocalVector;
	vLocalVector m_vLocalVector;

public:
	char	m_szNexonPassport[1024];  // nexon passport size = 1024
	bool	m_bNexonInit;
protected:
	ioLocalParent *CreateLocal( LocalType eLocalType );
	void AddLocal( ioLocalParent *pLocal );
	const char *GetLocalName( LocalType eLocalType );

public:
	void Init();
	ioLocalParent *GetLocal( LocalType eLocalType );
	void ParseCmd( const char *szCmd );

public:
	static ioLocalManager &GetSingleton();

public:
#ifdef SRC_EU
	int NexonCertification(HWND hWnd);
	void NexonLogout(HWND hWnd);
#endif

public:
	ioLocalManager(void);
	virtual ~ioLocalManager(void);
};


#define g_LocalMgr ioLocalManager::GetSingleton()


#pragma once

#ifdef SRC_SA

#define APEX_CMD_LOGIN						0x11
#define APEX_CMD_LOGOUT						0x12
#define APEX_CMD_DATA						0x13
#define APEX_CMD_INPUT_MSG					0x14
#define APEX_MAX_PACKET						2048

typedef struct tagApexPacket
{
	int	nType;
	char byBuffer[APEX_MAX_PACKET];
	short nLength;
} ApexPacket;

//
#pragma comment(lib, "Apex/AHClientInterface.lib")

class ioApex
{
protected:
	static ioApex *sg_Instance;

protected:

public:
	static ioApex &GetInstance();
	static void ReleaseInstance();

public:
	bool Start();
	void End();
	void OnRecieveCheck( ApexPacket &rkPacket );

	void Process();

private: // Singleton class
	ioApex(void);
	virtual ~ioApex(void);
};

#define g_ioApex ioApex::GetInstance()

#endif // APEX


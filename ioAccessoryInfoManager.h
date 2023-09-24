#pragma once
class ioPlayStage;

class ioAccessoryInfoManager : public Singleton< ioAccessoryInfoManager >
{
private:
	float m_fMortmainItemSellPrice;
	float m_fTimeItemSellPrice;

public:
	void OnAccessoryBuyResult( SP2Packet &rkPacket );
	void OnAccessorySellResult( SP2Packet &rkPacket );
	void OnAccessoryPresentResult( SP2Packet &rkPacket );
	void OnAccessoryAdd( SP2Packet &rkPacket, int &iCode );
	void OnAccessoryChange( SP2Packet &rkPacket, ioPlayStage* pPlayStage );

public:
	void LoadAccessoryInfo();
public:
	const float& GetResellMortmainAccessoryPeso() const;
	const float& GetResellTimeAccessoryTimePeso() const;
public:
	static ioAccessoryInfoManager& GetSingleton();
public:
	ioAccessoryInfoManager();
	virtual ~ioAccessoryInfoManager();
};

#define g_AccessoryInfoMgr ioAccessoryInfoManager::GetSingleton()
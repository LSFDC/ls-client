#pragma once


class ioFieldItemMgr : public Singleton< ioFieldItemMgr >
{
protected:
	ioHashString m_DefaultItemEffect;
	ioHashString m_DefaultItemOverEffect;

	CEncrypt<float> m_fMoveDropJumpAmt;
	CEncrypt<float> m_fMoveDropGravityAmt;

public:
	void LoadINIData();

public:
	inline const ioHashString& GetItemEffect() const { return m_DefaultItemEffect; }
	inline const ioHashString& GetItemOverEffect() const { return m_DefaultItemOverEffect; }
	inline float GetItemJumpAmt() const { return m_fMoveDropJumpAmt; }
	inline float GetItemGravityAmt() const { return m_fMoveDropGravityAmt; }

public:
	static ioFieldItemMgr& GetSingleton();

public:   
	ioFieldItemMgr();
	virtual ~ioFieldItemMgr();
};
#define g_FieldItemMgr ioFieldItemMgr::GetSingleton()

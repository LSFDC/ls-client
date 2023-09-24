#pragma once



class ioBaseChar;

#define DEFAULT_FISH_INVEN 10

struct FishItem
{
	BYTE m_iType;
	int m_iArray;

	FishItem()
	{
		m_iType = 0;
		m_iArray = 0;
	}
};
typedef std::vector<FishItem> FishItemList;

class FishItemSort : public std::binary_function< const FishItem&, const FishItem&, bool >
{
public:
	bool operator()( const FishItem &lhs , const FishItem &rhs ) const
	{
		if( lhs.m_iArray < rhs.m_iArray )
		{
			return true;
		}
		return false;
	}
};

class ioUserFishItem
{
protected:
	CEncrypt<int> m_iCurMaxInventory;
	FishItemList m_vFishItemList;

public:
	void Initialize();
	void ApplyFishItemData( SP2Packet &rkPacket );

	void AddFishItemData( BYTE iType, int iIndexArray );
	void DeleteFishItemData( int iIndexArray );

	int GetFishItemType( int iListArray );
	int GetFishItemArray( int iListArray );

	int GetCurFishItemCnt();

	void CheckCurMaxInventory();
	int GetCurMaxInventory();

public:
	ioUserFishItem();
	virtual ~ioUserFishItem();
};


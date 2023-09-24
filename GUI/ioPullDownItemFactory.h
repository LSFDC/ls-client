#pragma once
#include "ioWnd.h"

class ioPullDownList;
class ioPullDownItem;
struct PullDownInnerImage;

enum PullDownItemType
{
	PDIT_SINGLE,
	PDIT_DOUBLE,
};

//Ǯ�ٿ� ��ħ ��Ÿ��
enum PullDownStyle
{
	PDS_DOWN,	//�Ʒ���(�⺻��)	
	PDS_UP,		//����
};

//-----------------------------------------------------------------------------------------------------------------------------

//Ǯ�ٿ� ���ο��� ������ �̹��� ��
struct PullDownInnerImage
{
	ioUIRenderImage*	m_pImage;
	DWORD				m_dwColor;
	DWORD				m_dwOverColor;
	bool				m_bManagedImageByItem;
	UIRenderType		m_eRenderType;

	int					m_iXPos;
	int					m_iYPos;

	PullDownInnerImage()
	{
		m_pImage				= NULL;
		m_dwColor				= 0;
		m_dwOverColor			= 0;
		m_bManagedImageByItem	= false;
		m_iXPos					= 0;
		m_iYPos					= 0;
		m_eRenderType			= UI_RENDER_NORMAL;
	}
};
typedef std::vector<PullDownInnerImage> vPullDownInnerImage;

//-----------------------------------------------------------------------------------------------------------------------------
//Ǯ�ٿ������ ���丮

class ioPullDownItemFactory : public Singleton< ioPullDownItemFactory >
{
public:
	static ioPullDownItemFactory& GetSingleton();

public:
	void Layout( ioPullDownList* pList, PullDownStyle eStyle );
	int PullDownItemLayout( ioPullDownList* pList, ioPullDownItem* pPullDown, PullDownStyle eStyle, int iCurrXPos, int iCurrYPos, int BlankWidth, int iBlankHeight );

	int SetSelectLayout( ioPullDownItem* pPullDown, PullDownStyle eStyle, int iXPos, int iYPos, int iWidth, int iHeight );

	__forceinline float GetFontTextPixel( float fScale ){ return 20.0f * fScale; }

public:
	ioPullDownItemFactory(void);
	virtual ~ioPullDownItemFactory(void);
};

#define g_PullDownFactory ioPullDownItemFactory::GetSingleton()
#pragma once

class ioComplexStringPrinter
{
public:
	enum
	{
		BUFFER_SIZE = 2048,
	};
private:
	DWORD m_dwTextColor;
	DWORD m_dwBkColor;
	TextStyle m_TextStyle;

	typedef struct tagTextPiece
	{
		DWORD dwTextColor;
		DWORD dwBkColor;
		TextStyle eStyle;
		float fScale;
		float fYOffset;
		float fWidth;
		std::string szText;
	}TextPiece;

	typedef std::vector< TextPiece > TextPieceList;
	TextPieceList m_vPieceList;
	float m_fFullWidth;

private:
	static char m_szBuffer[BUFFER_SIZE];

private:
	void SetTextPiece( float fScale, float fYOffset );
	void SetTextPieceWithoutXOffset( float fScale, float fYOffset );

public:
	void SetTextColor( int iRed, int iGreen, int iBlue );
	void SetTextColor( DWORD dwTextColor );
	void SetFullTextColor( DWORD dwTextColor );
	void SetFullTextTableColor();
	void SetFullTextStyle( TextStyle eStyle );

	void SetBkColor( int iRed, int iGreen, int iBlue );
	void SetBkColor( DWORD dwBkColor );
	void SetFullBkColor( DWORD dwBkColor );

	void SetTextStyle( TextStyle eStyle );
	

	// ���ڿ� ���� �پ��� AddTextPiece / AddTextPieceWithYOffset �߰� 
#ifdef USE_LIST_ARQUMENTS_TEXT_FUNCTION
	void AddTextPiece( float fScale, const char *szSource, ... );
#else
	void AddTextPiece( float fScale, const char *szSource );
#endif
	void AddTextPiece( float fScale, const char *szSource, int iArg1 );
	void AddTextPiece( float fScale, const char *szSource, DWORD dwArg1 );
	void AddTextPiece( float fScale, const char *szSource, float fArg1 );
	void AddTextPiece( float fScale, const char *szSource, __int64 i64Arg1 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, const char *szArg2 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1, int iArg2 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1, const char *szArg2 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2, int iArg3 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2, const char *szArg3 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, const char *szArg2, const char *szArg3 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1, int iArg2, int iArg3 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1, int iArg2, const char *szArg3 );
	void AddTextPiece( float fScale, const char *szSource, const char *szArg1, const char *szArg2, const char *szArg3 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2, int iArg3, int iArg4 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2, const char *szArg3, int iArg4 );
	void AddTextPiece( float fScale, const char *szSource, int iArg1, int iArg2, int iArg3, int iArg4, int iArg5 );
#ifdef USE_LIST_ARQUMENTS_TEXT_FUNCTION
	void AddTextPieceWithYOffset( float fScale, float fYOffset, const char *szSource, ... );
#else
	void AddTextPieceWithYOffset( float fScale, float fYOffset, const char *szSource );
#endif
	void AddTextPieceWithYOffset( float fScale, float fYOffset, const char *szSource, int iArg1 );
	void AddTextPieceWithYOffset( float fScale, float fYOffset, const char *szSource, const char *szArg1 );
#ifdef USE_LIST_ARQUMENTS_TEXT_FUNCTION
	void AddTextPieceWithoutXOffset( float fScale, const char *szSource, ... );
#else
	void AddTextPieceWithoutXOffset( float fScale, const char *szSource );
#endif

	void CopyFullText( ioComplexStringPrinter &rkPrinter ) const;
	void CopyOnlyText( OUT std::string &rkText ) const;
	void CopyModifyScaleText( ioComplexStringPrinter &rkPrinter, float fScale );
	void SplitFullText( ioComplexStringPrinter &rkPrinterA, ioComplexStringPrinter &rkPrinterB, float fSplitWidth );

	void UpdateFullWidth();

public:
	void PrintFullText( float fXPos,
						float fYPos,
						TextAlignType eAlign,
						BYTE bAlpha = 255 );

	void PrintFullModifyText( float fXPos,
							  float fYPos,
							  DWORD dwTextColor,
							  TextStyle eStyle,
							  TextAlignType eAlign,
							  BYTE bAlpha = 255 );

	// �ڸ��� ���� �ؽ�Ʈ�� "..."ó��
	void PrintFullTextWidthCut( float fXPos, 
		                        float fYPos,
								TextAlignType eAlign, 
								float fWidth, 
								BYTE bAlpha = 255 );

	// �ڸ��� ���� �ؽ�Ʈ�� �ƹ��͵� ǥ������ ����
	float PrintFullTextWidthDirectCut( float fXPos, 
									   float fYPos,		
									   TextAlignType eAlign, 		
									   float fWidth, 		
									   BYTE bAlpha = 255 );
	void ClearList();

public:
	inline bool IsEmpty() const { return m_vPieceList.empty(); }
	inline float GetFullWidth() const { return m_fFullWidth; }

	bool IsCompare( const ioComplexStringPrinter &rkPrinter ) const;

public:
	ioComplexStringPrinter();
	~ioComplexStringPrinter();
};

typedef std::vector<ioComplexStringPrinter> ComplexStringPrinterVec;


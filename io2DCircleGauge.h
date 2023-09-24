#pragma once

class ioMeshData;
class ioTexture;
class ioRenderSystem;

class io2DCircleGauge
{
public:
	enum StartType
	{
		ST_6OCLOCK  = 0,
		ST_12OCLOCK = 1,
	};
protected:
	ioMeshData *m_pGaugeData;
	ioTexture  *m_pGaugeTex;

	int m_iRenderX;
	int m_iRenderY;

	int m_iMaxCount;
	int m_iFrontSkip;
	int m_iBackSkip;
	int m_iCurRenderCount;

	StartType m_eStartType;

public:
	// �������� ���������� �ƴ� �Ʒ����� �Ը������ε� �� �Ը����� ������ iFrontSkip ~ iBackSkip��ŭ ������
	void InitResource( const char *szTexFileName,	// �ؽ��������̸�
					   int iDivideCount,			// �ִ� ����
					   int iFrontSkip = 0,			// �տ��� ��� �����Ұ���
					   int iBackSkip = 0,           // �ڿ��� ��� �����Ұ���
					   StartType eType = ST_6OCLOCK );			

public:
	bool IsNeedRender() const;
	void UpdateGauge( int iXPos, int iYPos, float fRate );	// ��� xPos, yPos, 
	void Render( ioRenderSystem *pSystem );

public:
	io2DCircleGauge();
	virtual ~io2DCircleGauge();
};


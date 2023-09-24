#pragma once

#include "../io3DEngine/ioButton.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��� ���� ��ư
class SelectMaterialItemBtn : public ioButton
{
protected:
	int m_iCode;
	int m_iPreCode;
	int m_iCurCnt;
	int m_nRecommandCode;

	ioComplexStringPrinter m_CountText;

	ioUIRenderFrame *m_pMaterialFrm;
	ioUIRenderImage *m_pMaterialImg;
	ioUIRenderImage *m_pRecommandMaterialImg;
	ioUIRenderImage *m_pIconImg;
	ioUIRenderImage *m_pIconBackImg;

public:
	virtual void iwm_hide();
	
public:
	virtual void AddRenderFrame( const ioHashString &szType, ioUIRenderFrame *pFrame );
	virtual void AddRenderImage( const ioHashString &szType, ioUIRenderImage *pImage );

protected:
	virtual void OnProcess( float fTimePerSec );
	virtual void OnRender();

public:
	//�ʱ�ȭ
	void InitData();
	//������ ����
	void SetData( int iCode, int iCnt );
	//������ ����
	void SetRecommand( int iCode, int iCnt );
	//�ڵ� ���
	int GetCode();

public:
	SelectMaterialItemBtn();
	virtual ~SelectMaterialItemBtn();
};

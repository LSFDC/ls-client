#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SelectPetTargetBtn : public ioButton
{
protected:
	int m_nCurSelectSlot;
	DWORD m_dwCurTitleColor;

	ioHashString m_szDefaultText;
	ioHashString m_szSelectText;
	
public:
	virtual void iwm_show();

protected:
	virtual void OnRender();

public:
	virtual void ParseExtraInfo( ioXMLElement &xElement );

public:
	//�ʱ�ȭ
	void InitData();
	//�� ����
	void SetSelectPetSlot( int nSelectSlot );
	//��� ����
	void SetSelectMaterial( int nSelectMaterial, ioHashString szTitle );
	//�� ����
	void SetCurTitleColor( DWORD dwTitleColor );

public:
	inline int GetSelectSlot() const { return m_nCurSelectSlot; }

public:
	SelectPetTargetBtn();
	virtual ~SelectPetTargetBtn();
};
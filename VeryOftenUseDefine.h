#pragma once


//HARDCODE: �� ������� ���� ���־��̴�, Ư�� ������ "������ ����" define ���� ���� �ִ´�.
//			�� ����� �����Ѵٴ� ���� ��ü �������� �����ؾ��Ѵٴ°� ���

#define LENGTH_IP	16

#define COMPARE(x,min,max) (((x)>=(min))&&((x)<(max)))
#define SAFEDELETE(x)		if(x != NULL) { delete x; x = NULL; }
#define SAFEDELETEARRAY(x)	if(x != NULL) { delete [] x; x = NULL; }
#define SAFERELEASE(x)		if(x != NULL) { x->Release(); x = NULL; }


__forceinline unsigned int return57() { __asm mov eax, 57 };

template <typename type>					// RADtoDEG
type  RADtoDEG (type val) {return (type) (val * (return57() + 0.2957795132)); }
template <typename type>					// DEGtoRAD
type  DEGtoRAD (type val) {return (type) (val * 0.0174532925199);}


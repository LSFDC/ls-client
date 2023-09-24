#pragma once


// 1. ����� �� �ִ� ����
//    bool, char, BYTE, WORD, long, int , unsigned int, DWORD , float, double, __int64
//    enum( CharState, JumpState, BlowWoundedState, FallState, DieState �� �ټ� ) // For LosaSaga client
// 2. ���� ����
//    1) �޸� ���� ����
//       int�� float�� ���� Ÿ������ �����Ͽ� memcpy, memset, ������ ������ �ϸ� ������ �մϴ�.
//       ��) memcpy(encryptvlaue, int , 4);, memcpy(int, encryptvlaue , 4); �� ���� ������
//    2) %d,%u,%f���� ���ÿ� casting �ʿ�.
//       ��) printf(%d, (int)encryptvalue); casting�� ���� ������ �����Ⱚ ���.
//    3) ��ȣȭ ������ �ִ� ����ü�� class�� stl ���� pointer�� ���� 
//       ��) typedef std::vector< RoomOptionInfo* > RoomOptionInfoList; 
//           typedef std::vector< RoomOptionInfo > RoomOptionInfoList; <--- ���� �߻�
//    4) ���� ��ȣȭ ���� ���� =������ ���� �����մϴ�.
//       ��)CEncrypt<int> iEncrypt1 = 1; CEncrypt<int> iEncrypt2=3;
//          iEncrypt1 = iEncrypt2; // �Ϲ� Ŭ������ �ٸ��� ��ü ��������� ������� �ʰ� ������3�� ���Եȴ�
// 3. ��Ÿ
//    1) Debug ��忡�� ���� ��� �� �� ����. ��ȣȭ ������ �Ϲ� ������ �����ϰų�. ��) int = encryptvalue;
//       #define _NOTUSE�� �����Ͽ� m_value ������ �̿��ؼ� Ȯ��. (_NOTUSE ���� �ӵ��� ������ ������) 
//    2) template class�� cpp�� �и� �ȵ�. 

#include <MMSystem.h>
#include "GameEnumType.h" // For LostSaga client
#include "ccddaabb.h"

#pragma warning( disable : 4800 4172 4018 )

template <class type>
class CEncrypt  
{
private:
	//UJ 130829, reduced for better performance
	//enum { MAX_MEM_POOL = 200, MAX_ENCRYPT = 16, };
	enum { MAX_MEM_POOL = 12, MAX_ENCRYPT = 8, DEFAULT_ENCRYPT = 4 };

#ifdef _NOTUSE
public:
	type            m_value;
#endif
	
	unsigned short  m_MemPoolPos;
	unsigned short  m_ValueSize;
	char            m_EncryptSeed;
	ccddaabb        m_RC5;
	char            m_MemPool[MAX_MEM_POOL];
	char            m_TempMem[MAX_ENCRYPT];

	void  DecryptValueToPool2(char *value) // ����
	{
		for(int i = 0; i < m_ValueSize; i++)
		{
			value[i] = m_MemPool[m_MemPoolPos+i] ^ 0xF1 ^ m_EncryptSeed;
		}
	}

	void   EncryptValueToPool2(const char *value) // ����
	{
		for(int i = 0; i < m_ValueSize; i++)
		{
			m_MemPool[m_MemPoolPos+i] = value[i] ^ 0xF1 ^ m_EncryptSeed;
		}
	}

	void EncryptValueToPool(const char *value)
	{
		m_RC5.EncryptByte( value, m_MemPool, max(m_ValueSize, DEFAULT_ENCRYPT) );
		
	}

	void Init()
	{
		DWORD pid = GetCurrentProcessId();
		DWORD tick = timeGetTime();
		m_ValueSize    = sizeof(type);
		m_MemPoolPos   = (unsigned short)( rand()+pid+tick )%(MAX_MEM_POOL-m_ValueSize);
		m_EncryptSeed  = (char)( rand()+pid+tick )%256;

		for (int i = 0; i < MAX_MEM_POOL ; i++)
			m_MemPool[i] = (char)( rand()+pid+tick )%256; // ����

		//UJ 130913, ����ȭ: szKey �����ϵ��� ����ȭ
		//char szKey[ccddaabb::KEY_SIZE];//="";
		//for (int i = 0; i < ccddaabb::KEY_SIZE  ; i++)
		//	szKey[i] = (char)( rand()+pid+tick )%256; // ����
		m_RC5.Setup( NULL, pid, tick );

#ifdef _NOTUSE
		m_value        = 0;
		EncryptValueToPool((char*)&m_value);
		Debug();
#endif
	}
	
#ifdef _NOTUSE
	void Debug()
	{
		type value;
		DecryptValueToPool(&value);
		if(m_value != value)
		{
			int k = 0; // �극��ũ ����Ʈ�� �ɱ� ���ؼ� 
		}
	}
#endif

public:
	CEncrypt()
	{
		Init();
	}

	CEncrypt( const type &inValue )        // ����� ���ÿ� ���� �����ϱ� ���ؼ�
	{
		Init();
		*this = inValue;
	}

	CEncrypt( const CEncrypt<type> &inValue )   // ����� ���ÿ� ���� �����ϱ� ���ؼ�
	{
		Init();
		*this = inValue;
	}

	virtual ~CEncrypt()
	{
	}
	
public:
	///// ���� 
	////////////////////////////////////////////////////////////////////////////////
	operator type()
	{
		type value;
		DecryptValueToPool(&value);

#ifdef _NOTUSE
		Debug();
#endif
		return value;
	}

	operator type() const
	{
		//UJ 130829, const�� �Ȱ��� �ϸ� �ȴ� --start
		type value;
		CEncrypt<type>* pThis = const_cast<CEncrypt<type>*>(this);
		pThis->DecryptValueToPool(&value);
		//// const���� DecryptValueToPool ����� �� �����Ƿ� �Ʒ��� ������.
 		//type value;
		//char szTempMem[MAX_ENCRYPT];
		//m_RC5.DecryptByte( m_MemPool, szTempMem, max(m_ValueSize, DEFAULT_ENCRYPT) );
		//memcpy( &value, szTempMem, m_ValueSize );
		//UJ --end

#ifdef _NOTUSE
		Debug();
#endif
		return value;
	}

	type operator++()    
	{
		type value;
		DecryptValueToPool(&value);
		++value;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		++m_value;
		Debug();
#endif
		return value;
	}	

	type operator--()
	{
		type value;
		DecryptValueToPool(&value);
		--value;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		--m_value;
		Debug();
#endif
		return value;
	}

	type operator++(int)    
	{
		type value;
		DecryptValueToPool(&value);
		++value;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		++m_value;
		Debug();
#endif
		return value;
	}	

	type operator--(int)
	{
		type value;
		DecryptValueToPool(&value);
		--value;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		--m_value;
		Debug();
#endif
		return value;
	}

	// bool
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(bool *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((bool*)m_TempMem);
	}

	bool operator=(bool inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	
	bool operator==(const bool& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator!=(const bool& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	// char
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	type operator=(char inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const char& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const char& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}

	///////////////////////////////////////////////
	type& operator+= (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 
	bool operator!=(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		
	bool operator> (const char& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		
	bool operator<=(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		
	bool operator>=(const char& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	


	// BYTE
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(BYTE *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((BYTE*)m_TempMem);
	}

	type operator=(BYTE inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const BYTE& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const BYTE& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}
	///////////////////////////////////////////////
	type& operator+= (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 	

	bool operator> (const BYTE& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const BYTE& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// WORD
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(WORD *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((WORD*)m_TempMem);
	}

	type operator=(WORD inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const WORD& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const WORD& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}
	///////////////////////////////////////////////
	type& operator+= (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const WORD& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const WORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// int
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(int *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((int*)m_TempMem);
	}

	type operator=(int inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	
	type operator-(const int& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const int& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}

	///////////////////////////////////////////////
	type& operator+= (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const int& inValue)  
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// long
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(long *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((long*)m_TempMem);
	}

	type operator=(long inValue)
	{
		
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const long& inValue)
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const long& inValue)
	{
		
		type value;
		DecryptValueToPool(&value);
		return value % inValue;
	}

	///////////////////////////////////////////////
	type& operator+= (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const long& inValue)  
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const long& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		
	
	// DWORD
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(DWORD *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((DWORD*)m_TempMem);
	}

	type operator=(DWORD inValue)
	{
		
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const DWORD& inValue)
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const DWORD& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}
	///////////////////////////////////////////////
	type& operator+= (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const DWORD& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const DWORD& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const DWORD& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const DWORD& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// unsigned int
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	type operator=(unsigned int inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const unsigned int& inValue)
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const unsigned int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const unsigned int& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}

	///////////////////////////////////////////////
	type& operator+= (const unsigned int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const unsigned int& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 	

	bool operator> (const unsigned int& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		
	bool operator<=(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		
	bool operator>=(const unsigned int& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// float
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(float *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 4 );
		*value = *((float*)m_TempMem);
	}

	type operator=(float inValue)
	{	
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const float& inValue)
	{	
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const float& inValue) 
	{	
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const float& inValue) 
	{	
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const float& inValue)
	{		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}
	///////////////////////////////////////////////
	type& operator+= (const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const float& inValue) 
	{	
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const float& inValue) 
	{	
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator!=(const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const float& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}

	bool operator>=(const float& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// double
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(double *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 8 );
		*value = *((double*)m_TempMem);
	}

	type operator=(double inValue)
	{
		type value = (type)inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const double& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const double& inValue) 
	{	
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const double& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}
	///////////////////////////////////////////////
	type& operator+= (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 
	bool operator!=(const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		
	bool operator> (const double& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		
	bool operator<=(const double& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		
	bool operator>=(const double& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	

	// __int64
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(__int64 *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, 8 );
		*value = *((__int64*)m_TempMem);
	}

	type operator=(__int64 inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	type operator-(const __int64& inValue)
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value - inValue;
	}

	type operator+(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value + inValue;
	}

	type operator*(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value * inValue;
	}

	type operator/(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value / inValue;
	}
	
	type operator%(const __int64& inValue)
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return value % inValue;
	}

	///////////////////////////////////////////////
	type& operator+= (const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value += inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value += inValue;
		Debug();
#endif
		return value;
	} 

	type& operator-= (const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value -= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value -= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator*= (const __int64& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value *= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value *= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator/= (const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
		value /= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value /= inValue;
		Debug();
#endif
		return value;
	} 

	type& operator%= (const __int64& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
		value %= inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value %= inValue;
		Debug();
#endif
		return value;
	}

	/////////////////////////////////////////////
	bool operator==(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 
	
	bool operator< (const __int64& inValue) 
	{
		
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 
	bool operator!=(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 		

	bool operator> (const __int64& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const __int64& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	

	// CharState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(CharState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(CharState) );
		*value = *((CharState*)m_TempMem);
	}

	type operator=(CharState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const CharState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const CharState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const CharState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const CharState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const CharState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const CharState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	
	// JumpState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(JumpState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(JumpState) );
		*value = *((JumpState*)m_TempMem);
	}

	type operator=(JumpState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const JumpState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const JumpState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	}

	bool operator< (const JumpState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const JumpState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const JumpState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const JumpState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// BlowWoundedState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(BlowWoundedState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(BlowWoundedState) );
		*value = *((BlowWoundedState*)m_TempMem);
	}

	type operator=(BlowWoundedState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const BlowWoundedState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const BlowWoundedState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const BlowWoundedState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const BlowWoundedState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const BlowWoundedState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const BlowWoundedState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// FallState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(FallState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(FallState) );
		*value = *((FallState*)m_TempMem);
	}

	type operator=(FallState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const FallState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const FallState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const FallState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const FallState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const FallState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const FallState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// DieState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(DieState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(DieState) );
		*value = *((DieState*)m_TempMem);
	}

	type operator=(DieState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const DieState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const DieState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const DieState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const DieState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const DieState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const DieState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// NonePlayState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(NonePlayState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(NonePlayState) );
		*value = *((NonePlayState*)m_TempMem);
	}

	type operator=(NonePlayState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const NonePlayState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const NonePlayState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const NonePlayState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const NonePlayState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const NonePlayState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const NonePlayState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	

	// ChatModeState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(ChatModeState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(ChatModeState) );
		*value = *((ChatModeState*)m_TempMem);
	}

	type operator=(ChatModeState inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const ChatModeState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const ChatModeState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const ChatModeState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const ChatModeState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const ChatModeState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const ChatModeState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}


	// FishingState // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(FishingState *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(FishingState) );
		*value = *((FishingState*)m_TempMem);
	}

	type operator=(FishingState inValue)
	{

		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const FishingState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const FishingState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const FishingState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const FishingState& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const FishingState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const FishingState& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}

	// ProtectionMode // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(ProtectionMode *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(ProtectionMode) );
		*value = *((ProtectionMode*)m_TempMem);
	}

	type operator=(ProtectionMode inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const ProtectionMode& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const ProtectionMode& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const ProtectionMode& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const ProtectionMode& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const ProtectionMode& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const ProtectionMode& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}		

	// TeamType // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(TeamType *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(TeamType) );
		*value = *((TeamType*)m_TempMem);
	}

	type operator=(TeamType inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const TeamType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const TeamType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const TeamType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const TeamType& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const TeamType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const TeamType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	

	// ModeType // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(ModeType *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(ModeType) );
		*value = *((ModeType*)m_TempMem);
	}

	type operator=(ModeType inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const ModeType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const ModeType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const ModeType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const ModeType& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const ModeType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const ModeType& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}	

	// RoomStyle // enum ���� ( for LostSaga Client )
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	void  DecryptValueToPool(RoomStyle *value)
	{
		m_RC5.DecryptByte( m_MemPool, m_TempMem, sizeof(RoomStyle) );
		*value = *((RoomStyle*)m_TempMem);
	}

	type operator=(RoomStyle inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}
	/////////////////////////////////////////////
	bool operator==(const RoomStyle& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value == inValue);
	} 

	bool operator!=(const RoomStyle& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value != inValue);
	} 

	bool operator< (const RoomStyle& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value < inValue);
	} 

	bool operator> (const RoomStyle& inValue)  
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value > inValue);
	}		

	bool operator<=(const RoomStyle& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value <= inValue);
	}		

	bool operator>=(const RoomStyle& inValue) 
	{
		type value;
		DecryptValueToPool(&value);
#ifdef _NOTUSE
		Debug();
#endif
		return (value >= inValue);
	}

	// ��������� ������ �־�� ��. �ƴϸ� ��ü �ɹ� ������ �����. //////////////////////////////////
	
	// CEncrypt<bool>
	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	bool operator=(CEncrypt<bool>& inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	// CEncrypt<BYTE>
	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	bool operator=(CEncrypt<BYTE>& inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	// CEncrypt<char>
	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	bool operator=(CEncrypt<char>& inValue)
	{
		type value = inValue;
		EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		m_value = inValue;
		Debug();
#endif
		return value;
	}

	// CEncrypt<WORD>
	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<WORD>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<long>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<long>& inValue)
	 {
		 
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<int>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<int>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<unsigend int>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<unsigned int>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<DWORD>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<DWORD>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<float>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<float>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<double>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<double>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<__int64>
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<__int64>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<CharState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<CharState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<JumpState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<JumpState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<BlowWoundedState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<BlowWoundedState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<FallState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<FallState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<DieState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<DieState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<NonePlayState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<NonePlayState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<ChatModeState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<ChatModeState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<FishingState> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<FishingState>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<ProtectionMode> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<ProtectionMode>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<TeamType> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<TeamType>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<ModeType> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<ModeType>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }

	 // CEncrypt<RoomStyle> (for LostSaga Client )
	 //////////////////////////////////////////////////////////////////////////////////////
	 /////////////////////////////////////////////////////////////////////////////////////
	 bool operator=(CEncrypt<RoomStyle>& inValue)
	 {
		 type value = inValue;
		 EncryptValueToPool((char*)&value);
#ifdef _NOTUSE
		 m_value = inValue;
		 Debug();
#endif
		 return value;
	 }
};

#pragma warning( default : 4800 4172 4018 )


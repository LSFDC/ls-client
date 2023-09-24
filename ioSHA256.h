#pragma once
#ifndef _IOSHA256_H_

#ifdef _LZMA_UINT32_IS_ULONG
typedef long Int32;
typedef unsigned long UInt32;
#else
typedef int Int32;
typedef unsigned int UInt32;
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#define UINT64_CONST(n) n
#else
typedef long long int Int64;
typedef unsigned long long int UInt64;
#define UINT64_CONST(n) n ## ULL
#endif

#ifdef _MSC_VER

#include <stdlib.h>
#define rotlFixed(x, n) _rotl((x), (n))
#define rotrFixed(x, n) _rotr((x), (n))

#else

#define rotlFixed(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define rotrFixed(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#endif

#define SHA256_DIGEST_SIZE 32

typedef unsigned char Byte;

typedef struct
{
	UInt32 state[8];
	UInt64 count;
	Byte buffer[64];
} CSha256;

#define OUT


class ioSHA256
{
public:
	ioSHA256(void);
	~ioSHA256(void);

	const Byte * MakeHashCode(OUT size_t * hashSize, const Byte * sourceData, size_t size );

	void Sha256_Init();
	void Sha256_Update(const Byte *data, size_t size);
	void Sha256_Final(Byte *digest);

protected:
	void Sha256_Transform(UInt32 *state, const UInt32 *data);
	void Sha256_WriteByteBlock();

	CSha256 m_sha;
	Byte m_hash[SHA256_DIGEST_SIZE];
};

#endif //_IOSHA256_H_

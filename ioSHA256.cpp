#include "StdAfx.h"

#include "windows.h"
#include "ioSHA256.h"


ioSHA256::ioSHA256(void)
{
	ZeroMemory(&m_sha, sizeof(m_sha));
	ZeroMemory(&m_hash, sizeof(m_hash));
}


ioSHA256::~ioSHA256(void)
{
}



#define S0(x) (rotrFixed(x, 2) ^ rotrFixed(x,13) ^ rotrFixed(x, 22))
#define S1(x) (rotrFixed(x, 6) ^ rotrFixed(x,11) ^ rotrFixed(x, 25))
#define s0(x) (rotrFixed(x, 7) ^ rotrFixed(x,18) ^ (x >> 3))
#define s1(x) (rotrFixed(x,17) ^ rotrFixed(x,19) ^ (x >> 10))

#define blk0(i) (W[i] = data[i])
#define blk2(i) (W[i&15] += s1(W[(i-2)&15]) + W[(i-7)&15] + s0(W[(i-15)&15]))

#define Ch(x,y,z) (z^(x&(y^z)))
#define Maj(x,y,z) ((x&y)|(z&(x|y)))

#define a(i) T[(0-(i))&7]
#define b(i) T[(1-(i))&7]
#define c(i) T[(2-(i))&7]
#define d(i) T[(3-(i))&7]
#define e(i) T[(4-(i))&7]
#define f(i) T[(5-(i))&7]
#define g(i) T[(6-(i))&7]
#define h(i) T[(7-(i))&7]


#ifdef _SHA256_UNROLL2

#define R(a,b,c,d,e,f,g,h, i) h += S1(e) + Ch(e,f,g) + K[i+j] + (j?blk2(i):blk0(i));\
	d += h; h += S0(a) + Maj(a, b, c)

#define RX_8(i) \
	R(a,b,c,d,e,f,g,h, i); \
	R(h,a,b,c,d,e,f,g, i+1); \
	R(g,h,a,b,c,d,e,f, i+2); \
	R(f,g,h,a,b,c,d,e, i+3); \
	R(e,f,g,h,a,b,c,d, i+4); \
	R(d,e,f,g,h,a,b,c, i+5); \
	R(c,d,e,f,g,h,a,b, i+6); \
	R(b,c,d,e,f,g,h,a, i+7)

#else

#define R(i) h(i) += S1(e(i)) + Ch(e(i),f(i),g(i)) + K[i+j] + (j?blk2(i):blk0(i));\
	d(i) += h(i); h(i) += S0(a(i)) + Maj(a(i), b(i), c(i))

#ifdef _SHA256_UNROLL

#define RX_8(i) R(i+0); R(i+1); R(i+2); R(i+3); R(i+4); R(i+5); R(i+6); R(i+7);

#endif

#endif

static const UInt32 K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void ioSHA256::Sha256_Init()
{
	ZeroMemory(&m_sha, sizeof(m_sha));
	m_sha.state[0] = 0x6a09e667;
	m_sha.state[1] = 0xbb67ae85;
	m_sha.state[2] = 0x3c6ef372;
	m_sha.state[3] = 0xa54ff53a;
	m_sha.state[4] = 0x510e527f;
	m_sha.state[5] = 0x9b05688c;
	m_sha.state[6] = 0x1f83d9ab;
	m_sha.state[7] = 0x5be0cd19;
	m_sha.count = 0;
}

void ioSHA256::Sha256_Transform(UInt32 *state, const UInt32 *data)
{
	UInt32 W[16];
	unsigned j;
#ifdef _SHA256_UNROLL2
	UInt32 a,b,c,d,e,f,g,h;
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	f = state[5];
	g = state[6];
	h = state[7];
#else
	UInt32 T[8];
	for (j = 0; j < 8; j++)
		T[j] = state[j];
#endif

	for (j = 0; j < 64; j += 16)
	{
#if defined(_SHA256_UNROLL) || defined(_SHA256_UNROLL2)
		RX_8(0); RX_8(8);
#else
		unsigned i;
		for (i = 0; i < 16; i++) { R(i); }
#endif
	}

#ifdef _SHA256_UNROLL2
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
#else
	for (j = 0; j < 8; j++)
		state[j] += T[j];
#endif

	/* Wipe variables */
	/* memset(W, 0, sizeof(W)); */
	/* memset(T, 0, sizeof(T)); */
}

#undef S0
#undef S1
#undef s0
#undef s1

void ioSHA256::Sha256_WriteByteBlock()
{
	UInt32 data32[16];
	unsigned i;
	for (i = 0; i < 16; i++)
		data32[i] =
		((UInt32)(m_sha.buffer[i * 4    ]) << 24) +
		((UInt32)(m_sha.buffer[i * 4 + 1]) << 16) +
		((UInt32)(m_sha.buffer[i * 4 + 2]) <<  8) +
		((UInt32)(m_sha.buffer[i * 4 + 3]));
	Sha256_Transform(m_sha.state, data32);
}

void ioSHA256::Sha256_Update(const Byte *data, size_t size)
{
	UInt32 curBufferPos = (UInt32)m_sha.count & 0x3F;
	while (size > 0)
	{
		m_sha.buffer[curBufferPos++] = *data++;
		m_sha.count++;
		size--;
		if (curBufferPos == 64)
		{
			curBufferPos = 0;
			Sha256_WriteByteBlock();
		}
	}
}

void ioSHA256::Sha256_Final(Byte *digest)
{
	UInt64 lenInBits = (m_sha.count << 3);
	UInt32 curBufferPos = (UInt32)m_sha.count & 0x3F;
	unsigned i;
	m_sha.buffer[curBufferPos++] = 0x80;
	while (curBufferPos != (64 - 8))
	{
		curBufferPos &= 0x3F;
		if (curBufferPos == 0)
			Sha256_WriteByteBlock();
		m_sha.buffer[curBufferPos++] = 0;
	}
	for (i = 0; i < 8; i++)
	{
		m_sha.buffer[curBufferPos++] = (Byte)(lenInBits >> 56);
		lenInBits <<= 8;
	}
	Sha256_WriteByteBlock();

	for (i = 0; i < 8; i++)
	{
		*digest++ = (Byte)(m_sha.state[i] >> 24);
		*digest++ = (Byte)(m_sha.state[i] >> 16);
		*digest++ = (Byte)(m_sha.state[i] >> 8);
		*digest++ = (Byte)(m_sha.state[i]);
	}
	Sha256_Init();
}

const Byte * ioSHA256::MakeHashCode(OUT size_t * hashSize, const Byte * sourceData, size_t size )
{
	int hasize = sizeof(m_hash);
	hasize++;

	ZeroMemory(&m_hash, sizeof(m_hash));
	Sha256_Init();
	Sha256_Update(sourceData, size);
	Sha256_Final(m_hash);

	int hsize = 0;
	hsize += hasize;
	*hashSize = SHA256_DIGEST_SIZE;
	return m_hash;
}


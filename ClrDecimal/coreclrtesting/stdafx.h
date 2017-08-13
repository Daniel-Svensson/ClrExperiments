// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <oleauto.h>
#include <assert.h>
#include <intrin.h>

// Following is from coreclr headers
#define LIMITED_METHOD_CONTRACT
#define DEC_SCALE_MAX 28
typedef union {
	DWORDLONG int64;
	struct {
#ifdef BIGENDIAN
		ULONG Hi;
		ULONG Lo;
#else
		ULONG Lo;
		ULONG Hi;
#endif
	} u;
} SPLIT64;


// PAL RT Implementationsions
STDAPI VarDecMul_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecAdd_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecDiv_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);


// Prototype X64 Implementations
STDAPI VarDecMul_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecAdd_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecSub_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecDiv_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);

// TODO: reference additional headers your program requires here
inline void COPYDEC(DECIMAL &to, const DECIMAL &from)
{
	/*(to).Hi32 = (from).Hi32;
	(to).Lo64 = (from).Lo64;
	(to).signscale = (from).signscale;*/
	//*(__m128*)&to = *(__m128*)&from;
	to = from;
}

static_assert(sizeof(DECIMAL) == 16, "unecpected size");
static_assert(sizeof(__m128) == 16, "unecpected size");


#undef DECIMAL_SETZERO
inline void DECIMAL_SETZERO(DECIMAL &dec)
{
	memset(&dec, 0, sizeof(DECIMAL));
	//(*(__m128*)&dec) = _mm_set_zer();
}

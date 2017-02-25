// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define DO_NOT_DISABLE_RAND
#include "targetver.h"


//#include <iostream>

//#include "common.h"
//#include "decimal.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <OleAuto.h>
#include <assert.h>
#include <intrin.h>
#include <locale.h>

#include <array>
#include <vector>
#include <iostream>

// Following is from coreclr headers
#define LIMITED_METHOD_CONTRACT

#define DEC_SCALE_MAX 28
#define Div64by32(num, den) ((ULONG)((DWORDLONG)(num) / (ULONG)(den)))
#define Mod64by32(num, den) ((ULONG)((DWORDLONG)(num) % (ULONG)(den)))


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


inline DWORDLONG DivMod32by32(ULONG num, ULONG den)
{
	SPLIT64  sdl;

	sdl.u.Lo = num / den;
	sdl.u.Hi = num % den;
	return sdl.int64;
}

inline DWORDLONG DivMod64by32(DWORDLONG num, ULONG den)
{
	SPLIT64  sdl;

	sdl.u.Lo = Div64by32(num, den);
	sdl.u.Hi = Mod64by32(num, den);
	return sdl.int64;
}

// PAL RT Implementationsions
STDAPI VarDecMul_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecAdd_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);


// Prototype X64 Implementations
STDAPI VarDecMul_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecAdd_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecSub_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecDiv_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
DWORD64 FullDiv64By64(DWORD64 *pdlNum, DWORD64 ulDen);


// TODO: reference additional headers your program requires here

inline void COPYDEC(DECIMAL &to, const DECIMAL &from)
{
	(to).Hi32 = (from).Hi32;
	(to).Lo64 = (from).Lo64;
	(to).signscale = (from).signscale;
}
inline void DECIMAL_LO64_SET(DECIMAL & dec, DWORD64 value) { dec.Lo64 = value; }
inline DWORD64 & DECIMAL_LO64_GET(DECIMAL & dec) { return dec.Lo64; }
inline ULONG & DECIMAL_HI32(DECIMAL &dec) { return dec.Hi32; }
inline ULONG & DECIMAL_MID32(DECIMAL &dec) { return dec.Mid32; }
inline ULONG & DECIMAL_LO32(DECIMAL &dec) { return dec.Lo32; }
inline USHORT & DECIMAL_SIGNSCALE(DECIMAL &dec) { return dec.signscale; }

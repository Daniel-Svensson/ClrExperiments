// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <oleauto.h>
#include <cassert>
#include <cstring>
//#include <intrin.h>

#include <cstdint>

typedef uint8_t BYTE;
typedef uint16_t USHORT;
typedef uint32_t ULONG;
typedef uint32_t HRESULT;
typedef uint32_t DWORD;
typedef uint32_t DWORD32;
typedef uint64_t DWORD64;
typedef uint64_t DWORDLONG;

#define STDAPI HRESULT

struct DECIMAL
{
	USHORT wReserved;
	union{
		struct
		{
			unsigned char scale;
			unsigned char sign;
		};
		unsigned short signscale;
	};
	DWORD32 Hi32;
	union
	{
		struct 
		{
			DWORD32 Lo32;
			DWORD32 Mid32;
		};
		DWORD64 Lo64;
	};
};
const uint8_t DECIMAL_NEG  = 0x80;

typedef DECIMAL* LPDECIMAL;

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

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) ((DWORDLONG)((DWORD)(a)) * (DWORDLONG)((DWORD)(b)))
#endif


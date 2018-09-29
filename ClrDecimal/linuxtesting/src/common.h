#pragma once

#include "stdafx.h"

#ifdef _MSC_VER
#include <sal.h>
#else // CLANG? 

#define _In_
#define _Out_
#define _Inout_
#define _In_count_(x)
#define _In_range_(a,b)
#define __assume(condition)
#define __analysis_assume(condition)
#define _In_count_(x)

#endif


#ifndef DECIMAL_SCALE
#define DECIMAL_SCALE(dec)       ((dec).scale)
#endif

#ifndef DECIMAL_SIGN
#define DECIMAL_SIGN(dec)        ((dec).sign)
#endif

#ifndef DECIMAL_SIGNSCALE
#define DECIMAL_SIGNSCALE(dec)   ((dec).signscale)
#endif

#ifndef DECIMAL_LO32
#define DECIMAL_LO32(dec)        ((dec).Lo32)
#endif

#ifndef DECIMAL_MID32
#define DECIMAL_MID32(dec)       ((dec).Mid32)
#endif

#ifndef DECIMAL_HI32
#define DECIMAL_HI32(dec)       ((dec).Hi32)
#endif

#ifndef DECIMAL_LO64_GET
#define DECIMAL_LO64_GET(dec)       ((dec).Lo64)
#endif

#ifndef DECIMAL_LO64_SET
#define DECIMAL_LO64_SET(dec,value)   {(dec).Lo64 = value; }
#endif

#ifndef WIN32
// oleauto on windows defines 
struct DECIMAL
{
	uint16_t wReserved;
	union {
		struct
		{
			uint8_t scale;
			uint8_t sign;
		};
		uint16_t signscale;
	};
	uint32_t Hi32;
	union
	{
		struct
		{
			uint32_t Lo32;
			uint32_t Mid32;
		};
		uint64_t Lo64;
	};
};
const uint8_t DECIMAL_NEG = 0x80;
typedef int32_t HRESULT;
typedef HRESULT(*decimal_func)(const DECIMAL*, const DECIMAL*, DECIMAL*);
typedef uint8_t BYTE;

const HRESULT DISP_E_DIVBYZERO = (HRESULT)0x80020012;
const HRESULT DISP_E_OVERFLOW = (HRESULT)0x8002000A;
const HRESULT NOERROR = 0;
#define __stdcall

// Returns index of most significant bit in mask if any bit is set, returns false if Mask is 0
inline bool BitScanReverse(uint32_t *Index, uint32_t Mask) {
	// G++/Clang __builtin_clz count LSB as 1, and MSB as 0 while BitScanReverse works in other way around
	*Index = (uint32_t)(31 - __builtin_clz(Mask));
	return (Mask != 0);
}

// Returns index of most significant bit in mask if any bit is set, returns false if Mask is 0
inline unsigned char BitScanReverse64(uint32_t * Index, uint64_t Mask)
{
	*Index = (uint32_t)(63 - __builtin_clzll(Mask));
	return (Mask != 0);
}
#endif

#ifndef STDAPI
#define STDAPI extern "C" HRESULT
#endif


#ifdef _MSC_VER
#include <intrin.h>
#else // CLANG? 
#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_)
#include <x86intrin.h>
#endif
#endif


#ifndef min
#define min std::min
#endif
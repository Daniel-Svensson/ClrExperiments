#pragma once

#include "stdafx.h"

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

const HRESULT DISP_E_DIVBYZERO = 0x80020012L;
const HRESULT DISP_E_OVERFLOW = 0x8002000AL;
const HRESULT NOERROR = 0;

#include "decimal_calc.h"

#ifdef _MSC_VER
#include <intrin.h>
#else // CLANG? 
#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_)
#include <x86intrin.h>
#endif
#endif

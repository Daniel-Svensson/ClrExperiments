// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef _STDAFX_H
#define _STDAFX_H


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <oleauto.h>
#endif

#include <cassert>
#include <cstring>
#include <cstdint>

#ifndef _WINDOWS_
typedef uint8_t BYTE;
typedef uint16_t USHORT;
typedef uint32_t ULONG;
typedef uint32_t DWORD;
typedef uint32_t DWORD32;
typedef uint64_t DWORD64;
typedef uint64_t DWORDLONG;
#endif

//#define STDAPI HRESULT


// Following is from coreclr headers
#define LIMITED_METHOD_CONTRACT
#define DEC_SCALE_MAX 28

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) ((uint64_t)((uint32_t)(a)) * (uint64_t)((uint32_t)(b)))
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

#endif // !_STDAFX_H
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef _STDAFX_H
#define _STDAFX_H

#include <cassert>
#include <cstring>
#include <cstdint>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <oleauto.h>
#else
typedef union {
	uint64_t int64;
	struct {
#ifdef BIGENDIAN
		uint32_t Hi;
		uint32_t Lo;
#else
		uint32_t LowPart;
		uint32_t HighPart;
#endif
	} u;
} ULARGE_INTEGER;

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

#endif // !_STDAFX_H
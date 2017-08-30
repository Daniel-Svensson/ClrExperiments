#pragma once

#ifndef  _DECIMAL_CALC_
#define _DECIMAL_CALC_

#include <cstdint>

#if defined(_WIN32)
	#if 1
	#include <windows.h>
	#else
		#ifndef STDAPI
		#define STDAPI extern "C" long __stdcall
		#endif
	#endif
#else // ! _WIN32
	typedef uint32_t HRESULT;
#endif

#ifndef STDAPI
#define STDAPI extern "C" HRESULT
#endif

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

#endif

#ifndef __wtypes_h__
struct DECIMAL;
#endif



// Prototype X64 Implementations
STDAPI VarDecMul_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecDiv_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecAdd_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI VarDecSub_x64(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI DecAddSub_x64(_In_ const DECIMAL * pdecL, _In_ const DECIMAL * pdecR, _Out_ DECIMAL * __restrict pdecRes, char bSign);

#endif // ! _DECIMAL_CALC_
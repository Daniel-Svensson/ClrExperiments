#pragma once

#ifndef _DECIMAL_CALC_
#define _DECIMAL_CALC_

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


STDAPI DecimalMul(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI DecimalDiv(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res);
STDAPI DecimalAddSub(_In_ const DECIMAL * pdecL, _In_ const DECIMAL * pdecR, _Out_ DECIMAL * __restrict pdecRes, uint8_t bSign);

inline HRESULT __stdcall DecimalAdd(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res)
{
	return DecimalAddSub(l, r, res, 0);
}
inline HRESULT __stdcall DecimalSub(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res)
{
	return DecimalAddSub(l, r, res, DECIMAL_NEG);
}

#endif // ! _DECIMAL_CALC_
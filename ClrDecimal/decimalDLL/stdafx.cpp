// stdafx.cpp : source file that includes just the standard includes
// decimalDLL.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#include <OleAuto.h>

STDAPI DecimalAddSub(_In_ const DECIMAL * pdecL, _In_ const DECIMAL * pdecR, _Out_ DECIMAL * __restrict pdecRes, char bSign);

HRESULT DecimalAdd(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res)
{
	return DecimalAddSub(l, r, res, 0);
}
HRESULT DecimalSub(const DECIMAL* l, const DECIMAL *r, DECIMAL * __restrict res)
{
	return DecimalAddSub(l, r, res, DECIMAL_NEG);
}
// decimalDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <windows.h>

#define API _declspec(dllexport);

struct DECIMAL;

extern "C"
{
	// Prototype X64 Implementations
	API HRESULT VarDecMul_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
	//API STDAPI VarDecAdd_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
	//API STDAPI VarDecSub_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
	//API STDAPI VarDecDiv_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
}
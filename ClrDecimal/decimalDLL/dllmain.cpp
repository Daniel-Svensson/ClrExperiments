// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <OleAuto.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// add noop operation to
// measure overhead of just accessing memory
// without any expensive calculations or branching
STDAPI VarDecNoop(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	pdecRes->signscale = pdecL->signscale | pdecR->signscale;
	pdecRes->Hi32 = pdecL->Hi32 + pdecR->Hi32;
	pdecRes->Lo64 = pdecL->Lo64 + pdecR->Lo64;

	return 0;
}
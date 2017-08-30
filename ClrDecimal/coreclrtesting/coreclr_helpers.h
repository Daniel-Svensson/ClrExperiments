#pragma once

#include <Windows.h>
#include <OleAuto.h>

#define Div64by32(num, den) ((ULONG)((DWORDLONG)(num) / (ULONG)(den)))
#define Mod64by32(num, den) ((ULONG)((DWORDLONG)(num) % (ULONG)(den)))

inline void DECIMAL_LO64_SET(DECIMAL & dec, DWORD64 value) { dec.Lo64 = value; }
inline DWORD64 & DECIMAL_LO64_GET(DECIMAL & dec) { return dec.Lo64; }
inline ULONG & DECIMAL_HI32(DECIMAL &dec) { return dec.Hi32; }
inline ULONG & DECIMAL_MID32(DECIMAL &dec) { return dec.Mid32; }
inline ULONG & DECIMAL_LO32(DECIMAL &dec) { return dec.Lo32; }
inline USHORT & DECIMAL_SIGNSCALE(DECIMAL &dec) { return dec.signscale; }


inline DWORDLONG DivMod32by32(ULONG num, ULONG den)
{
	SPLIT64  sdl;

	sdl.u.Lo = num / den;
	sdl.u.Hi = num % den;
	return sdl.int64;
}

inline DWORDLONG DivMod64by32(DWORDLONG num, ULONG den)
{
	SPLIT64  sdl;

	sdl.u.Lo = Div64by32(num, den);
	sdl.u.Hi = Mod64by32(num, den);
	return sdl.int64;
}
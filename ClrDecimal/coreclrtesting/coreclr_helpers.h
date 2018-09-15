#pragma once

#include <Windows.h>
#include <OleAuto.h>

#define Div64by32(num, den) ((ULONG)((DWORDLONG)(num) / (ULONG)(den)))
#define Mod64by32(num, den) ((ULONG)((DWORDLONG)(num) % (ULONG)(den)))

typedef union {
	uint64_t int64;
	struct {
#ifdef BIGENDIAN
		uint32_t Hi;
		uint32_t Lo;
#else
		uint32_t Lo;
		uint32_t Hi;
#endif
	} u;
} SPLIT64;


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
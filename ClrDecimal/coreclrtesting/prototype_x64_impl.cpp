#include "stdafx.h"
#include <assert.h>
#include <functional> // swap
#include <intrin.h>
#include "PROTOTYPE_X64_IMPL.H"

// Switches to test
#define INLINE_ASM // Enable or disable inline asm for x86
// #define PROFILE_NOINLINE DECLSPEC_NOINLINE
#define PROFILE_NOINLINE
//#define NO_UDIV128
#ifndef _AMD64
#define NO_UDIV128
#endif

DWORD32 IncreaseScale96By32(ULONG *rgulNum, DWORD32 ulPwr);
DWORD64 IncreaseScale96By64(ULONG *rgulNum, DWORD64 ulPwr);

// TODO: reference additional headers your program requires here
inline void COPYDEC(DECIMAL &to, const DECIMAL &from)
{
	(to).Hi32 = (from).Hi32;
	(to).signscale = (from).signscale;
	(to).Lo64 = (from).Lo64;
}

#undef DECIMAL_SETZERO
inline void DECIMAL_SETZERO(DECIMAL &dec)
{
	memset(&dec, 0, sizeof(DECIMAL));
}

// ------------------------- INSTRINCTS AND SIMPLE HELPERS ------------------------
inline unsigned int & low32(unsigned __int64 &value)
{
	return *(unsigned int*)&((SPLIT64*)&value)->u.Lo;
}

inline const unsigned int & low32(const unsigned __int64 &value)
{
	return *(const unsigned int*)&((SPLIT64*)&value)->u.Lo;
}

inline unsigned int & hi32(unsigned __int64 &value)
{
	return *(unsigned int*)&((SPLIT64*)&value)->u.Hi;
}

inline const unsigned int & hi32(const unsigned __int64 &value)
{
	return *(const unsigned int*)&((SPLIT64*)&value)->u.Hi;
}

// Determine if 64 bit value can be stored in 32 bits without loss (upper 32 bits are 0)
static inline unsigned char FitsIn32Bit(const DWORD64* pValue)
{
#ifdef _AMD64_
	return *pValue <= (DWORD64)(MAXDWORD32);
#else
	return hi32(*pValue) == 0;
#endif
}

#ifdef _AMD64_
#define UDIV1238_IMPL 1
#if UDIV1238_IMPL == 1
// Performs division of a 128bit number with 
// requires that hi < ulDen in order to not loose precision
// so it is best to set it to 0 unless when chaining calls in which case 
// the remainder from a previus devide should be used
extern "C" DWORD64 _udiv128(DWORD64 low, DWORD64 hi, DWORD64 divisor, __out DWORD64 *remainder);
// Performs inplace division of a 128bit number with 
// requires that hi < ulDen in order to not loose precision
// so it is best to set it to 0 unless when chaining calls in which case 
// the remainder from a previus devide should be used
extern "C" DWORD64 _udiv128_v2(__inout DWORD64* pLow, DWORD64 hi, DWORD64 ulDen);
#else

extern "C" __m128i _udiv128_impl_sse(DWORD64 low, DWORD64 hi, DWORD64 divisor);

inline DWORD64 _udiv128(DWORD64 low, DWORD64 hi, DWORD64 divisor, __out DWORD64 *remainder)
{
	__m128i temp = _udiv128_impl_sse(low, hi, divisor);
	*remainder = temp.m128i_u64[1];
	return temp.m128i_u64[0];
}

inline DWORD64 _udiv128_v2(__inout DWORD64* pLow, DWORD64 hi, DWORD64 ulDen)
{
	__m128i temp = _udiv128_impl_sse(*pLow, hi, ulDen);
	*pLow = temp.m128i_u64[0];
	return temp.m128i_u64[1];
}
#endif

#define UDIV_IMPL 1

#if (UDIV_IMPL != 2 && UDIV_IMPL != 3)
extern "C" unsigned int _udiv64(unsigned int lo, unsigned int hi, unsigned int ulDen, __out unsigned int *remainder);
extern "C" unsigned int _udiv64_v2(__inout unsigned int* pLow, unsigned int hi, unsigned int ulDen);
#endif
#if (UDIV_IMPL == 3)
extern "C" DWORD64 _udiv64_impl(unsigned int lo, unsigned int hi, unsigned int ulDen);
inline unsigned int _udiv64(unsigned int lo, unsigned int hi, unsigned int ulDen, __out unsigned int *remainder)
{
	DWORD64 temp = _udiv64_impl(lo, hi, ulDen);
	*remainder = hi32(temp);
	return temp;
}
inline unsigned int _udiv64_v2(__inout unsigned int* pLow, unsigned int hi, unsigned int ulDen)
{
	DWORD64 temp = _udiv64_impl(*pLow, hi, ulDen);
	*pLow = (DWORD32)temp;
	return hi32(temp);
}
#endif

#else

// TODO: Define addcaryy etc for ARM 
// CLANG builtins availible 
/*
unsigned char      __builtin_addcb (unsigned char x, unsigned char y, unsigned char carryin, unsigned char *carryout);
unsigned short     __builtin_addcs (unsigned short x, unsigned short y, unsigned short carryin, unsigned short *carryout);
unsigned           __builtin_addc  (unsigned x, unsigned y, unsigned carryin, unsigned *carryout);
unsigned long      __builtin_addcl (unsigned long x, unsigned long y, unsigned long carryin, unsigned long *carryout);
unsigned long long __builtin_addcll(unsigned long long x, unsigned long long y, unsigned long long carryin, unsigned long long *carryout);
unsigned char      __builtin_subcb (unsigned char x, unsigned char y, unsigned char carryin, unsigned char *carryout);
unsigned short     __builtin_subcs (unsigned short x, unsigned short y, unsigned short carryin, unsigned short *carryout);
unsigned           __builtin_subc  (unsigned x, unsigned y, unsigned carryin, unsigned *carryout);
unsigned long      __builtin_subcl (unsigned long x, unsigned long y, unsigned long carryin, unsigned long *carryout);
unsigned long long __builtin_subcll(unsigned long long x, unsigned long long y, unsigned long long carryin, unsigned long long *carryout);
*/

inline unsigned char _addcarry_u64(unsigned char carry, unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64 *pRes)
{
	carry = _addcarry_u32(carry, low32(lhs), low32(rhs), &low32(*pRes));
	return _addcarry_u32(carry, hi32(lhs), hi32(rhs), &hi32(*pRes));
}

inline unsigned char _subborrow_u64(unsigned char carry, unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64 *pRes)
{
	carry = _subborrow_u32(carry, low32(lhs), low32(rhs), &low32(*pRes));
	return _subborrow_u32(carry, hi32(lhs), hi32(rhs), &hi32(*pRes));
}

inline unsigned char _BitScanReverse64(unsigned long * Index, unsigned __int64 Mask)
{
	unsigned char res = _BitScanReverse(Index, hi32(Mask));
	if (res)
	{
		*Index += 32;
	}
	else
	{
		res = _BitScanReverse(Index, low32(Mask));
	}

	return res;
}

// Performs multiplications of 2 64bit values, returns lower 64bit and store upper 64bit in _HighProduct
static unsigned __int64 __fastcall _umul128(unsigned __int64 lhs, unsigned __int64 rhs, unsigned __int64 * _HighProduct)
{
	unsigned __int64 lowRes, sdltmp1, sdltmp2;
	unsigned __int64 &hiRes = *_HighProduct;

	unsigned char carry = 0;

	lowRes = UInt32x32To64(low32(lhs), low32(rhs));
	if ((hi32(lhs) | hi32(rhs)) == 0)
	{
		*_HighProduct = 0;
		return lowRes;
	}

	sdltmp1 = UInt32x32To64(hi32(lhs), low32(rhs));
	carry = _addcarry_u32(0, low32(sdltmp1), hi32(lowRes), &hi32(lowRes));

	hiRes = UInt32x32To64(hi32(lhs), hi32(rhs));
	carry = _addcarry_u32(carry, hi32(sdltmp1), low32(hiRes), &low32(hiRes));
	_addcarry_u32(carry, hi32(hiRes), 0, &hi32(hiRes));

	sdltmp2 = UInt32x32To64(low32(lhs), hi32(rhs));
	carry = _addcarry_u32(0, low32(sdltmp2), hi32(lowRes), &hi32(lowRes));
	carry = _addcarry_u32(carry, hi32(sdltmp2), low32(hiRes), &low32(hiRes));
	_addcarry_u32(carry, hi32(hiRes), 0, &hi32(hiRes));

	return lowRes;
}

unsigned __int64 ShiftLeft128(unsigned __int64 _LowPart, unsigned __int64 _HighPart, unsigned char _Shift)
{
	return (_HighPart << _Shift) + (_LowPart >> (64 - _Shift));
}

#ifndef INLINE_ASM
inline DWORD64 __fastcall _udiv64_impl(unsigned int lo, unsigned int hi, unsigned int ulDen)
{
	assert(hi < ulDen);
	if (hi == 0)
	{
		SPLIT64 res;
		res.u.Hi = lo % ulDen;
		res.u.Lo = lo / ulDen;
		return res;
	}
	else
	{
		SPLIT64 sdl;
		sdl.u.Hi = hi;
		sdl.u.Lo = lo;

		SPLIT64 res;
		res.u.Hi = static_cast<unsigned int>(sdl.int64 % ulDen);
		res.u.Lo = static_cast<unsigned int>(sdl.int64 / ulDen);
		return res;
	}
}
#else

//#define NAKED _declspec(naked)
#ifdef NAKED
_declspec(naked)
#endif
inline DWORD64 __fastcall _udiv64_impl(unsigned int lo, unsigned int hi, unsigned int ulDen)
{


	// DX:AX = DX:AX / r/m; resulting 
	// 
	// DX == remainder
#ifdef NAKED
	_asm
	{
		// edx is already hi
		mov eax, ecx; // lo is in ecx
		div dword ptr [esp+4];
		ret 4;// ulDen (4 byte on stack)
	}
#else
	_asm
	{
		// edx is already hi
		mov eax, ecx; // lo is in ecx
		div ulDen;
	}
#endif

}
#endif

inline unsigned int _udiv64(unsigned int lo, unsigned int hi, unsigned int ulDen, __out unsigned int *remainder)
{

	DWORD64 temp = _udiv64_impl(lo, hi, ulDen);
	*remainder = hi32(temp);
	return low32(temp);
}

inline unsigned int _udiv64_v2(__inout unsigned int* pLow, unsigned int hi, unsigned int ulDen)
{
	auto temp = _udiv64_impl(*pLow, hi, ulDen);
	*pLow = low32(temp);
	return hi32(temp);
}
#endif

// Performs multiplications of 2 64bit values, returns lower 64bit and store upper 64bit in _HighProduct
static unsigned __int64 __fastcall _umul64by32(unsigned __int64 lhs, unsigned __int32 rhs, unsigned __int32 * _HighProduct)
{
#ifdef _AMD64_
	unsigned __int64 temp;
	auto res = _umul128(lhs, rhs, &temp);
	*_HighProduct = (unsigned int)temp;
	return res;
#else
	DWORD64 lowRes = UInt32x32To64(low32(lhs), rhs); // quo * lo divisor
	DWORD64 hiRes = UInt32x32To64(hi32(lhs), rhs); // quo * mid divisor

	hiRes += hi32(lowRes);

	hi32(lowRes) = low32(hiRes);
	*_HighProduct = hi32(hiRes);

	return lowRes;
#endif
}


// ------------------------- CONSTANTS ------------------------
const int POWER10_MAX64 = 19;
const int POWER10_MAX32 = 9;
const int SEARCHSCALE_MAX_SCALE = POWER10_MAX64;

static constexpr DWORD64 rgulPower10_64[] = {
1,
10,
100,
1000,
10000,
100000,
1000000,
10000000,
100000000,
1000000000,
10000000000,
100000000000,
1000000000000,
10000000000000,
100000000000000,
1000000000000000,
10000000000000000,
100000000000000000,
1000000000000000000,
10000000000000000000,
};

static const DWORD64 POWER10_MAX_VALUE64 = rgulPower10_64[POWER10_MAX64];
static const DWORD32 POWER10_MAX_VALUE32 = (DWORD32)rgulPower10_64[POWER10_MAX32];

struct DECOVFL2
{
	DWORD64 Hi;
	DWORD32 Lo;
};

static constexpr DECOVFL2 PowerOvfl[] = {
	{ ULLONG_MAX, ULONG_MAX }, // 
	{ 1844674407370955161uI64, 2576980377u }, // 10^1 0,6
	{ 184467440737095516uI64, 687194767u }, // 10^2 0,16
	{ 18446744073709551uI64, 2645699854u }, // 10^3 0,616
	{ 1844674407370955uI64, 694066715u }, // 10^4 0,1616
	{ 184467440737095uI64, 2216890319u }, // 10^5 0,51616
	{ 18446744073709uI64, 2369172679u }, // 10^6 0,551616
	{ 1844674407370uI64, 4102387834u }, // 10^7 0,9551616
	{ 184467440737uI64, 410238783u }, // 10^8 0,09551616
	{ 18446744073uI64, 3047500985u }, // 10^9 0,709551616 
	{ 1844674407uI64, 1593240287u }, // 10^10 0,3709551616
	{ 184467440uI64, 3165801135u }, // 10^11 0,73709551616
	{ 18446744uI64, 316580113u }, // 10^12 0,073709551616
	{ 1844674uI64, 1749644929u }, // 10^13 0,4073709551616
	{ 184467uI64, 1892951411u }, // 10^14 0,44073709551616
	{ 18446uI64, 3195772248u }, // 10^15 0,744073709551616
	{ 1844uI64, 2896557602u }, // 10^16 0,674407370955162
	{ 184uI64, 2007642678u }, // 10^17 0,467440737095516
	{ 18uI64, 1918751186u }, // 10^18 0,446744073709552
	{ 1uI64, 3627848955u }, // 10^19 0,844674407370955
};

const DWORD32 OVFL_MAX32_1_HI = 429496729;

// Divides a 64bit number (ullNum) by 64bit value (ullDen)
// returns 64bit quotient with remainder in *pRemainder
// This results in a single div instruction on x64 platforms
static inline DWORD64 DivMod64By64_x64(DWORD64 ullNum, DWORD64 ullDen, __out DWORD64* pRemainder)
{
#ifndef _AMD64_
	if (FitsIn32Bit(&ullDen))
	{
		SPLIT64 res;
		DWORD32 ulDen = low32(ullDen);
		DWORD32 remainder;
		auto hi = hi32(ullNum);
		if (hi < ulDen)
		{
			res.u.Hi = 0;
			remainder = hi;
		}
		else
		{
			res.u.Hi = hi / ulDen;
			remainder = hi % ulDen;
		}
		// 32 by 32 div mod
		res.u.Lo = _udiv64(low32(ullNum), remainder, ulDen, &remainder);
		*pRemainder = remainder;
		return res.int64;
	}
#endif
	auto mod = ullNum % ullDen;
	auto res = ullNum / ullDen;

	*pRemainder = mod;
	return res;
}

static inline unsigned char Add96(DECIMAL *pDec, DWORD64 value)
{
	auto carry = _addcarry_u64(0, pDec->Lo64, value, &pDec->Lo64);
	return _addcarry_u32(carry, pDec->Hi32, 0, (unsigned int*)&pDec->Hi32);
}

static inline unsigned char Add96(ULONG *plVal, DWORD64 value)
{
	auto carry = _addcarry_u64(0, *(DWORD64*)&plVal[0], value, (DWORD64*)&plVal[0]);
	return _addcarry_u32(carry, plVal[2], 0, (unsigned int*)(&plVal[2]));
}

static inline unsigned char Add96(DWORD64 *pllVal, DWORD64 value)
{
	auto carry = _addcarry_u64(0, pllVal[0], value, &pllVal[0]);
	return _addcarry_u32(carry, (DWORD32)pllVal[1], 0, &low32(pllVal[1]));
}

static inline unsigned char Sub96(ULONG *plVal, DWORD64 value)
{
	auto carry = _subborrow_u64(0, *(DWORD64*)&plVal[0], value, (DWORD64*)&plVal[0]);
	return _subborrow_u32(carry, plVal[2], 0, (DWORD32*)(&plVal[2]));
}

/***
* Div96By32_x64
*
* Entry:
*   pdlNum - Pointer to 96-bit dividend as array of ULONGs, least-sig first
*   ulDen   - 32-bit divisor.
*
* Purpose:
*    Divides a 96bit ulong by 32bit, returns 32bit remainder
*
* Exit:
*   Quotient overwrites dividend.
*   Returns remainder.
*
* Exceptions:
*   None.
*
***********************************************************************/
static inline DWORD32 Div96By32_x64(__inout ULONG *pdlNum, DWORD32 ulDen)
{
	unsigned int *const rgulNum = (unsigned int *)pdlNum;
	DWORD32 remainder;
#if 0
	remainder = 0;
	if (pdlNum[2] != 0)
		goto Div3Word;

	if (pdlNum[1] >= ulDen)
		goto Div2Word;

	remainder = pdlNum[1];
	pdlNum[1] = 0;
	goto Div1Word;
#else
	if (rgulNum[2] >= ulDen)
		goto Div3Word;
		
	remainder = rgulNum[2];
	rgulNum[2] = 0;
	if (remainder | (rgulNum[1] >= ulDen))
		goto Div2Word;

	remainder = rgulNum[1];
	rgulNum[1] = 0;
	goto Div1Word;
#endif

Div3Word:
	remainder = rgulNum[2] % ulDen;
	rgulNum[2] = rgulNum[2] / ulDen;
Div2Word:
	remainder = _udiv64_v2(&rgulNum[1], remainder, ulDen);
Div1Word:
	remainder = _udiv64_v2(&rgulNum[0], remainder, ulDen);
	return remainder;
}

static inline DWORD32 Div96By32_x64(__inout DWORD64 *pdllNum, DWORD32 ulDen)
{
	return Div96By32_x64((ULONG*)pdllNum, ulDen);
}

PROFILE_NOINLINE
DWORD32 InplaceDivide_32(_In_count_(iHiRes) DWORD64 * rgullRes, _Inout_ _In_range_(0, 2) int& iHiRes, DWORD32 ulDen)
{
	int iCur = iHiRes * 2 + 1; // convert from 64bit to 32bit indicec (+1 == always point to upper 32 bits at start)
	DWORD32 *const rgulRes = (DWORD32*)rgullRes;
	DWORD32 ulRemainder = 0;

	if (rgulRes[iCur] == 0)
		iCur--;

	if (rgulRes[iCur] < ulDen)
	{
		ulRemainder = rgulRes[iCur];
		rgulRes[iCur] = 0;
		iCur--;
	}

	while (iCur >= 0) {
		// Compute subsequent quotients.
		//
		ulRemainder = _udiv64_v2(&rgulRes[iCur], ulRemainder, ulDen);
		iCur--;
	}

	// If upper 64bits are 0 then decrease iHiRes
	if (iHiRes > 0 && rgullRes[iHiRes] == 0)
		iHiRes--;

	return ulRemainder;
}

#if defined(_AMD64_)
//PROFILE_NOINLINE
inline DWORD64 InplaceDivide_64(_In_count_(iHiRes) DWORD64 * rgullRes, _Inout_ _In_range_(0, 2) int& iHiRes, DWORD64 ullDen)
{
	DWORD64 ullRemainder;
	int iCur = iHiRes - 1;

	if (rgullRes[iHiRes] < ullDen)
	{
		ullRemainder = rgullRes[iHiRes];
		rgullRes[iHiRes] = 0;
		// If first quotient was 0, update iHiRes.
		if (iHiRes > 0)
			iHiRes--;
	}
	else
	{
		rgullRes[iHiRes] = DivMod64By64_x64(rgullRes[iHiRes], ullDen, &ullRemainder);
	}

	while (iCur >= 0) {
		ullRemainder = _udiv128_v2(&rgullRes[iCur], ullRemainder, ullDen);
		iCur--;
	}

	return ullRemainder;
}
#endif

PROFILE_NOINLINE
DWORD64 ReduceScale(_In_count_(iHiRes) DWORD64 * rgullRes, _Inout_ _In_range_(0, 2)
	int &iHiRes, _Out_ DWORD64 &ullDen, _Inout_ int &iNewScale)
{
#if !defined(_AMD64_)
	DWORD32 ulDen;
	// Handle up to POWER10_MAX32 scale at a time
	if (iNewScale < POWER10_MAX32) {
		ulDen = (DWORD32)rgulPower10_64[iNewScale];
	}
	else {
		ulDen = POWER10_MAX_VALUE32;
	}
	iNewScale -= POWER10_MAX32;
	ullDen = ulDen;
	return InplaceDivide_32(rgullRes, iHiRes, ulDen);
#else // _AMD64_

	// Handle up to POWER10_MAX64 scale at a time
	// with fast path for scaling by 32bit 
	if (iNewScale < POWER10_MAX64)
	{
		ullDen = rgulPower10_64[iNewScale];
		if (iNewScale <= POWER10_MAX32)
		{
			iNewScale = 0;
			return InplaceDivide_32(rgullRes, iHiRes, (DWORD32)ullDen);
		}
	}
	else
	{
		ullDen = POWER10_MAX_VALUE64;
	}

	iNewScale -= POWER10_MAX64;
	return InplaceDivide_64(rgullRes, iHiRes, ullDen);
#endif
}

/***
* ScaleResult_x64
*
* Entry:
*   rgullRes - Array of DWORD64 with value, least-significant first.
*   iHiRes  - Index of last non-zero value in rgulRes, Max 2.
*   iScale  - Scale factor for this value, range 0 - 2 * DEC_SCALE_MAX
*
* Purpose:
*   See if we need to scale the result to fit it in 96 bits.
*   Perform needed scaling.  Adjust scale factor accordingly.
*   ScaleResult is called from AddSub as well as multiply
*
* Exit:
*   rgullRes updated in place, all items with index <= iHiRes are updated.
*   New scale factor returned, -1 if overflow error.
*
***********************************************************************/
int ScaleResult_x64(__inout DWORD64 *rgullRes, __in_range(0, 2) int iHiRes, __in_range(0, 2 * DEC_SCALE_MAX) int iScale)
{
	LIMITED_METHOD_CONTRACT;

	__assume(0 <= iHiRes &&  iHiRes <= 2);
	__assume(0 <= iScale &&  iScale <= 2 * DEC_SCALE_MAX);

	int     iNewScale;
	DWORD   ulMsb;
	DWORD64 ullSticky;
	DWORD64 ullPwr;
	DWORD64 ullRemainder;

	// See if we need to scale the result.  The combined scale must
	// be <= DEC_SCALE_MAX and the upper 96 bits must be zero.
	// 
	// Start by figuring a lower bound on the scaling needed to make
	// all but the lower 96 bits zero.  iHiRes is the index into rgulRes[]
	// of the highest non-zero element.
	// 
	BOOLEAN found = _BitScanReverse64(&ulMsb, rgullRes[iHiRes]);
	assert(found);
	iNewScale = iHiRes * 64 + ulMsb - 96;

	if (iNewScale >= 0) {
		// Multiply bit position by log10(2) to figure it's power of 10.
		// We scale the log by 256.  log(2) = .30103, * 256 = 77.  Doing this 
		// with a multiply saves a 96-byte lookup table.  The power returned
		// is <= the power of the number, so we must add one power of 10
		// to make it's integer part zero after dividing by 256.
		// 
		// Note: the result of this multiplication by an approximation of
		// log10(2) have been exhaustively checked to verify it gives the 
		// correct result.  (There were only 95 to check...)
		// 
		iNewScale = ((iNewScale * 77) >> 8) + 1;

		// iNewScale = min scale factor to make high 96 bits zero, 0 - 29.
		// This reduces the scale factor of the result.  If it exceeds the
		// current scale of the result, we'll overflow.
		// 
		if (iNewScale > iScale)
			return -1;
	}
	else
		iNewScale = 0;

	// Make sure we scale by enough to bring the current scale factor
	// into valid range.
	//
	if (iNewScale < iScale - DEC_SCALE_MAX)
		iNewScale = iScale - DEC_SCALE_MAX;

	if (iNewScale != 0) {

		// Scale by the power of 10 given by iNewScale.  Note that this is 
		// NOT guaranteed to bring the number within 96 bits -- it could 
		// be 1 power of 10 short.
		//
		iScale -= iNewScale;
		ullSticky = 0;
		ullRemainder = 0;

		for (;;) {

			ullSticky |= ullRemainder; // record remainder as sticky bit

			ullRemainder = ReduceScale(rgullRes, iHiRes, ullPwr, iNewScale);

			if (iNewScale > 0)
				continue; // scale some more

			// If we scaled enough, iHiRes would be 0 or 1 without anything above first 96bits.  If not,
			// divide by 10 more.
			//
			// TODO (iHiRes > 1 | !FitsIn32Bit(&rgullRes[1]))
			if (iHiRes > 1 || !FitsIn32Bit(&rgullRes[1])) {
				iNewScale = 1;
				iScale--;
				continue; // scale by 10
			}

			// Round final result.  See if remainder >= 1/2 of divisor.
			// If remainder == 1/2 divisor, round up if odd or sticky bit set.
			//
			ullPwr >>= 1;  // power of 10 always even
			if (ullRemainder > ullPwr || (ullPwr == ullRemainder && ((rgullRes[0] & 1) | ullSticky))) {

				// Add 1 to first 96 bit word and check for overflow.
				// We only scale if iHiRes was originally >= 1 so rgulRes[1] is already initalized.
				// 
				auto carry = Add96(rgullRes, 1);
				if (carry != 0) {
					// The rounding caused us to carry beyond 96 bits. 
					// Scale by 10 more.
					// We know that hi32(rgullRes[1]) == 0 before rounding up
					// so adding the carry results in 1.
					//
					assert(iHiRes == 1);
					hi32(rgullRes[1]) = 1;
					ullSticky = 0;  // no sticky bit
					ullRemainder = 0; // or remainder
					iNewScale = 1;
					iScale--;
					continue; // scale by 10
				}
			}

			// We may have scaled it more than we planned.  Make sure the scale 
			// factor hasn't gone negative, indicating overflow.
			// 
			if (iScale < 0)
				return -1;

			return iScale;
		} // for(;;)
	}
	return iScale;
}

//**********************************************************************
//
// VarDecMul_x64 - Decimal Multiply
// x64 implementation of VarDecMul
//
//**********************************************************************
STDAPI VarDecMul_x64(const DECIMAL* pdecL, const DECIMAL *pdecR, DECIMAL * __restrict res)
{
	DWORD64 lo;
	DWORD64 hi;
	DWORD64 ullPwr;
	DWORD64 ullRem;

	int iScale = pdecL->scale + pdecR->scale;

	// If high bits are not set, them we can do a single 64x64bit multiply
	if ((pdecL->Hi32 | pdecR->Hi32) == 0)
	{
		lo = _umul128(pdecL->Lo64, pdecR->Lo64, &hi);
		if (hi == 0ui64)
		{
			// Result iScale is too big.  Divide result by power of 10 to reduce it down to 'DEC_SCALE_MAX'
			if (iScale > DEC_SCALE_MAX)
			{
				// If the amount to divide by is > 19 the result is guaranteed
				// less than 1/2.  [max value in 64 bits = 1.84E19]
				iScale -= DEC_SCALE_MAX;
				if (iScale > 19)
				{
					// TODO: if the implementation is only for .Net are we allowed to set the reserved byts to 0
					// if so we can just do 2 64bit writes, insteas of 64bit + 32bit + 16 bit
				ReturnZero:
					DECIMAL_SETZERO(*res);
					return NOERROR;
				}
				ullPwr = rgulPower10_64[iScale];

				lo = DivMod64By64_x64(lo, ullPwr, &ullRem);

				// Round result towards even.  See if remainder >= 1/2 of divisor.
				ullPwr >>= 1; // Divisor is a power of 10, so it is always even.
				if (ullRem > ullPwr || (ullRem == ullPwr && (lo & 1)))
					lo++;

				iScale = DEC_SCALE_MAX;
			}

			res->Hi32 = 0;
			res->Lo64 = lo;
		}
		else
		{
			DWORD64 tmpSpace[2];
			tmpSpace[0] = lo;
			tmpSpace[1] = hi;
			iScale = ScaleResult_x64(tmpSpace, 1, iScale);

			if (iScale == -1)
				return DISP_E_OVERFLOW;

			res->Lo64 = tmpSpace[0];
			res->Hi32 = (DWORD32)tmpSpace[1];
		}
	}
	else
	{
		DWORD64 tmpSum;
		DWORD64 tmpLo2;
		DWORD32 tmpHi1;
		DWORD32 tmpHi2;
		DWORD64 rgulProd[3];

		// At least one operand has bits set in the upper 64 bits.
		//
		// Compute and accumulate the 9 partial products into a 
		// 192-bit (24-byte) result.
		//
		//                [l-hi][l-lo]   left high32, lo64
		//             x  [r-hi][r-lo]   right high32, lo64
		// -------------------------------
		//
		//                [ 0-h][0-l ]   l-lo * r-lo => 64 + 64 bit result
		//          [ h*l][h*l ]         l-lo * r-hi => 32 + 64 bit result
		//          [ l*h][l*h ]         l-hi * r-lo => 32 + 64 bit result
		//          [ h*h]               l-hi * r-hi => 32 + 32 bit result
		// ------------------------------
		//          [p-2 ][p-1 ][p-0 ]   prod[] array
		// 
		// 
		// We can add 2 32bit numers to a 32bit*32bit result withaout overflow / carry
		//  This is the reason that adding 
		//  The maximum value of the "hi" result of the middle products are MAXDWORD32-1 each
		//  so adding their carry to the addition will only result in a 32bit value at most MAXDWORD32
		//  so this will never generate a carry.

		rgulProd[0] = _umul128(pdecL->Lo64, pdecR->Lo64, &tmpSum);
		rgulProd[2] = UInt32x32To64(pdecL->Hi32, pdecR->Hi32);

		// Do crosswise multiplications between upper 32bit and lower 64 bits
		// tmpSum keeps value for tmpSum (initialized from first multiply)
		// Add lo64 and result to tmpSum and propagate upper bits (hi) to rgulProd[2]
		lo = _umul64by32(pdecL->Lo64, pdecR->Hi32, &tmpHi1);
		auto carry1 = _addcarry_u64(0, lo, tmpSum, &tmpSum);
		_addcarry_u64(carry1, tmpHi1, rgulProd[2], &rgulProd[2]);

		tmpLo2 = _umul64by32(pdecR->Lo64, pdecL->Hi32, &tmpHi2);
		auto carry2 = _addcarry_u64(0, tmpLo2, tmpSum, &tmpSum);
		_addcarry_u64(carry2, tmpHi2, rgulProd[2], &rgulProd[2]);

		rgulProd[1] = tmpSum;

		// Check for leading zero ULONGs on the product
		//
		int iHiProd = 2;
		while (rgulProd[iHiProd] == 0) {
			if (--iHiProd < 0)
				goto ReturnZero;
		}

		__analysis_assume(iHiProd >= 0);
		iScale = ScaleResult_x64(rgulProd, iHiProd, iScale);
		if (iScale == -1)
			return DISP_E_OVERFLOW;

		res->Lo64 = rgulProd[0];
		res->Hi32 = (DWORD32)rgulProd[1];
	}

	res->sign = pdecL->sign ^ pdecR->sign;
	res->scale = (BYTE)iScale;
	return NOERROR;
}

//**********************************************************************
//
// DecAddSub_x64 - Decimal Add / Subtract
//
//**********************************************************************
static HRESULT DecAddSub_x64(__in const DECIMAL * pdecL, __in const DECIMAL * pdecR, __out DECIMAL * __restrict pdecRes, char bSign)
{
	DECIMAL   decTmp;
	DECIMAL   decRes;
	DWORD64   rgulNum[3];

	bSign ^= (pdecR->sign ^ pdecL->sign) & DECIMAL_NEG;

	if (pdecR->scale == pdecL->scale) {
		// Scale factors are equal, no alignment necessary.
		//
		decRes.signscale = pdecL->signscale;

	AlignedAdd:
		if (bSign) {
			// Signs differ - subtract
			//
			auto carry = _subborrow_u64(0, pdecL->Lo64, pdecR->Lo64, &decRes.Lo64);
			carry = _subborrow_u32(carry, pdecL->Hi32, pdecR->Hi32, (unsigned int*)&decRes.Hi32);

			// Propagate carry
			//
			if (carry != 0) {
				// Got negative result.  Flip its sign.
				//
			SignFlip:
				decRes.Lo64 = -(LONGLONG)decRes.Lo64;
				decRes.Hi32 = ~decRes.Hi32;
				if (decRes.Lo64 == 0)
					decRes.Hi32++;
				decRes.sign ^= DECIMAL_NEG;
			}
		}
		else {
			// Signs are the same - add
			//
			auto carry = _addcarry_u64(0, pdecL->Lo64, pdecR->Lo64, &decRes.Lo64);
			carry = _addcarry_u32(carry, pdecL->Hi32, pdecR->Hi32, (unsigned int*)&decRes.Hi32);

			// Propagate carry
			if (carry != 0) {
				// The addition carried above 96 bits.  Divide the result by 10,
				// dropping the scale factor.
				//
				if (decRes.scale == 0)
					return DISP_E_OVERFLOW;
				decRes.scale--;

				// Dibvide with 10, "carry 1" from overflow
				DWORD32 remainder = _udiv64_v2((DWORD32*)&decRes.Hi32, 1, 10);
				remainder = _udiv64_v2((DWORD32*)&decRes.Mid32, remainder, 10);
				remainder = _udiv64_v2((DWORD32*)&decRes.Lo32, remainder, 10);

				// See if we need to round up.
				//
				if (remainder >= 5 && (remainder > 5 || (decRes.Lo32 & 1))) {
					// Add one, will never overflow since we divided by 10
					Add96(&decRes, 1);
				}
			}
		}
	}
	else {
		DWORD64   ullPwr;
		int       iScale;
		int       iHiProd;

		// Scale factors are not equal.  Assume that a larger scale
		// factor (more decimal places) is likely to mean that number
		// is smaller.  Start by guessing that the right operand has
		// the larger scale factor.  The result will have the larger
		// scale factor.
		//
		decRes.scale = pdecR->scale;  // scale factor of "smaller"
		decRes.sign = pdecL->sign;    // but sign of "larger"
		iScale = decRes.scale - pdecL->scale;

		if (iScale < 0) {
			// Guessed scale factor wrong. Swap operands.
			//
			iScale = -iScale;
			decRes.scale = pdecL->scale;
			decRes.sign ^= bSign;
			std::swap(pdecL, pdecR);
		}

		// *pdecL will need to be multiplied by 10^iScale so
		// it will have the same scale as *pdecR.  We could be
		// extending it to up to 192 bits of precision.
		// Remarks: iScale is in range [0, 28] which can required up to 94 bits
		// so final result will be at most log2(10^28) + 96  < 190 bits without overflow
		//

#if defined(_AMD64_)
		if (iScale <= POWER10_MAX64) {
#else
		if (iScale <= POWER10_MAX32) {
#endif

			// Scaling won't make it larger than 96 + (32/64) bits < so it will 
			// fit in 128 bits (2*DWORD64) for 32bit platfrms
			// fit in 128+32 bits (3*DWORD64) for 64bit platfrms
			//
			ullPwr = rgulPower10_64[iScale];


#if defined(_AMD64_)
			DWORD64 hi;
			rgulNum[0] = _umul128(pdecL->Lo64, ullPwr, &hi);
			rgulNum[1] = _umul128(pdecL->Hi32, ullPwr, &rgulNum[2]);
			auto carry = _addcarry_u64(0, rgulNum[1], hi, &rgulNum[1]);
			_addcarry_u64(carry, rgulNum[2], 0, &rgulNum[2]);
#else
#define TEST_SIMPLER 0
#if TEST_SIMPLER == 1
			// This generates only 1 add instruction more but is a bit shorter
			rgulNum[0] = pdecL->Lo64;
			low32(rgulNum[1]) = pdecL->Hi32;
			hi32(rgulNum[1]) = IncreaseScale96By32((ULONG*)rgulNum, (DWORD32)ullPwr);
#elif TEST_SIMPLER == 0
			rgulNum[0] = UInt32x32To64(pdecL->Lo32, ullPwr);
			auto midResult = UInt32x32To64(pdecL->Mid32, ullPwr);
			rgulNum[1] = UInt32x32To64(pdecL->Hi32, ullPwr);
			auto carry = _addcarry_u32(0, low32(midResult), hi32(rgulNum[0]), &hi32(rgulNum[0]));
			carry = _addcarry_u32(carry, hi32(midResult), low32(rgulNum[1]), &low32(rgulNum[1]));
			_addcarry_u32(carry, 0, hi32(rgulNum[1]), &hi32(rgulNum[1]));	
#endif
#endif

			// TO REVIEW: we can set rgulNum[2] to 0 above for non _AMD64_
			// by adding that extra write we can remove this condition
			// and the compiler will skip this check, but write will impacte cache
#if defined(_AMD64_)
			if (rgulNum[2] != 0) {
				iHiProd = 2;
			}
			else
#endif
			{
				if (FitsIn32Bit(&rgulNum[1])) {
					// Result fits in 96 bits.  Use standard aligned add.
					//
					decTmp.Lo64 = rgulNum[0];
					decTmp.Hi32 = (DWORD32)rgulNum[1];
					pdecL = &decTmp;
					goto AlignedAdd;
				}
				iHiProd = 1;
			}
		}
		else {
			// Have to scale by a bunch.  Move the number to a buffer
			// where it has room to grow as it's scaled.
			//
			rgulNum[0] = pdecL->Lo64;
			rgulNum[1] = pdecL->Hi32;
			// TODO?: rgulNum[2] = 0 to simplify rest of the logic
			rgulNum[2] = 0;
			iHiProd = 1;

			// Scan for zeros in the upper words.
			//
			if (rgulNum[1] == 0) {
				iHiProd = 0;
				if (rgulNum[0] == 0) {
					// Left arg is zero, return right.
					//
					decRes.Lo64 = pdecR->Lo64;
					decRes.Hi32 = pdecR->Hi32;
					decRes.sign ^= bSign;
					goto RetDec;
				}
			}

			// Scaling loop, up to 10^19 at a time.  iHiProd stays updated
			// with index of highest non-zero element.
			//
			for (; iScale > 0; iScale -= POWER10_MAX64) {
				if (iScale >= POWER10_MAX64)
					ullPwr = POWER10_MAX_VALUE64;
				else
					ullPwr = rgulPower10_64[iScale];
#if 1
				// TODO: Try loop unrolling for 2 or 3
				DWORD64 mul_carry;
				unsigned char add_carry = 0;
				rgulNum[0] = _umul128(ullPwr, rgulNum[0], &mul_carry);

				for (int iCur = 1; iCur <= iHiProd; iCur++) {
					DWORD64 tmp = mul_carry;
					DWORD64 product = _umul128(ullPwr, rgulNum[iCur], &mul_carry);
					add_carry = _addcarry_u64(add_carry, tmp, product, &rgulNum[iCur]);
				}

				// We're extending the result by another element.
				// Hi is at least 1 away from it's max value so we can add carry without overflow.
				// ex: 0xffff*0xffff => "fffe0001", and it is the same pattern for all bitlenghts
				//
				if ((mul_carry | add_carry) != 0)
					_addcarry_u64(add_carry, mul_carry, 0, &rgulNum[++iHiProd]);
			}
#else
				DWORD64 hi, mul_carry;
				rgulNum[0] = _umul128(ullPwr, rgulNum[0], &hi);
				DWORD64 product = _umul128(ullPwr, rgulNum[1], &mul_carry);
				DWORD64 product2 = ullPwr*rgulNum[2];

				auto add_carry = _addcarry_u64(0, product, hi, &rgulNum[1]);
				_addcarry_u64(add_carry, mul_carry, product2, &rgulNum[2]);

			}
			
			if (rgulNum[2] != 0)
				iHiProd = 2;
#endif

			// Scaling by 10^28 (DEC_MAX_SCALE) adds upp to 94bits to the result
			// so result will be at most 190 = 96+94 bits (so will always in fit in 3*64 = 192 bits)
			// => iHiProd vill be at most 2
			__analysis_assume(iHiProd <= 2);
		}

		// Scaling complete, do the add.  Could be subtract if signs differ.
		//
		if (bSign) {
			// Signs differ, subtract.
			//
			auto carry = _subborrow_u64(0, rgulNum[0], pdecR->Lo64, &decRes.Lo64);
			carry = _subborrow_u64(carry, rgulNum[1], pdecR->Hi32, &rgulNum[1]);
			decRes.Hi32 = (DWORD32)rgulNum[1];

			// Propagate carry
			//
			if (carry != 0) {
				// If rgulNum has more than 96 bits of precision, then we need to
				// carry the subtraction into the higher bits.  If it doesn't,
				// then we subtracted in the wrong order and have to flip the 
				// sign of the result.
				// 

				// rgulNum[0 .. 1] is at most 96 bits since a 96bit subtraction resulted in carry
				// Use SignFlip to flip result ov values in decRes
				if (iHiProd <= 1)
					goto SignFlip; // Result placed already placed in decRes 
	
				assert(iHiProd == 2);
				if (--rgulNum[2] == 0)
					iHiProd = 1;
			}
		}
		else {
			// Signs are the same - add
			//
			auto carry = _addcarry_u64(0, rgulNum[0], pdecR->Lo64, &decRes.Lo64);
			carry = _addcarry_u64(carry, rgulNum[1], pdecR->Hi32, &rgulNum[1]);
			decRes.Hi32 = (DWORD32)rgulNum[1];

			// Propagate carry
			//
			if (carry) {
				// Result above 128 bits, If upper bits are not yet set 
				// then set it as 1 otherwise increase. There is no risk
				// for overflow
				// 
				if (iHiProd < 2) {
					rgulNum[2] = 1;
					iHiProd = 2;
				}
				else {
					++rgulNum[2];
				}
			}
		}

		// decRes contains the lower 96 bits of the result
		// but at the same time the complete result apart from first element (rgulNum[0])
		// is in rgulNum[1..2]. 
		assert(decRes.Hi32 == low32(rgulNum[1]));

		if (iHiProd > 1 || (iHiProd == 1 && !FitsIn32Bit(&rgulNum[1]))) {
			rgulNum[0] = decRes.Lo64;

			decRes.scale = (BYTE)ScaleResult_x64(rgulNum, iHiProd, decRes.scale);
			if (decRes.scale == (BYTE)-1)
				return DISP_E_OVERFLOW;

			decRes.Lo64 = rgulNum[0];
			decRes.Hi32 = (DWORD32)rgulNum[1];
			assert(FitsIn32Bit(&rgulNum[1]));
		}
	}

RetDec:
	COPYDEC(*pdecRes, decRes);
	return NOERROR;
}

//**********************************************************************
//
// VarDecAdd_x64 - x64 implementation of VarDecAdd
//
//**********************************************************************
STDAPI VarDecAdd_x64(const DECIMAL* pdecL, const DECIMAL *pdecR, DECIMAL * __restrict pdecRes)
{
	return DecAddSub_x64(pdecL, pdecR, pdecRes, 0);
}

//**********************************************************************
//
// VarDecSub_x64 - x64 implementation of VarDecSub
//
//**********************************************************************
STDAPI VarDecSub_x64(const DECIMAL* pdecL, const DECIMAL *pdecR, DECIMAL * __restrict pdecRes)
{
	return DecAddSub_x64(pdecL, pdecR, pdecRes, DECIMAL_NEG);
}

/***
* IncreaseScale
*
* Entry:
*   rgulNum - Pointer to 96-bit number as array of ULONGs, least-sig first
*   ulPwr   - Scale factor to multiply by
*
* Purpose:
*   Multiply the two numbers.  The low 96 bits of the result overwrite
*   the input.  The last 32 bits of the product are the return value.
*
* Exit:
*   Returns highest 32 bits of product.
*
* Exceptions:
*   None.
*
***********************************************************************/

// Add a 64bit number to a 96bit number => 160 bit
// updates 96bit in place and return the overflow (up to 64 bit)
PROFILE_NOINLINE
DWORD64 IncreaseScale96By64(ULONG *rgulNum, DWORD64 ulPwr)
{
	LIMITED_METHOD_CONTRACT;

	DWORD64 *rgullNum = (DWORD64*)rgulNum;
	SPLIT64   sdlTmp;

	DWORD64 hi;
	DWORD64 overflow;
	rgullNum[0] = _umul128(rgullNum[0], ulPwr, &hi);
	sdlTmp.int64 = _umul128(rgulNum[2], ulPwr, &overflow);

	// We will never overflow past 160 bits (32bit stored in overflow)
	auto carry = _addcarry_u64(0, sdlTmp.int64, hi, &sdlTmp.int64);
	_addcarry_u64(carry, overflow, 0, &overflow);

	rgulNum[2] = sdlTmp.u.Lo;

	// shift [overflow, sdlTemp] 32bit right and return lower 64 bits
	// we should return bits [160..97] of our 192 bit result 
	// use lo32 bits of overflow [192..129] by 32 bit left shift
	// use hi32 bits of sdlTmp   [128..97] by 32 bit left shift
	return (overflow << 32) + sdlTmp.u.Hi;
}

PROFILE_NOINLINE
DWORD32 IncreaseScale96By32(ULONG *rgulNum, DWORD32 ulPwr)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64   sdlTmp;
	DWORD64 *rgullNum = (DWORD64*)rgulNum;
	DWORD32 hi;
	rgullNum[0] = _umul64by32(rgullNum[0], ulPwr, &hi);
	sdlTmp.int64 = UInt32x32To64(rgulNum[2], ulPwr) + hi;

	rgulNum[2] = sdlTmp.u.Lo;
	return sdlTmp.u.Hi;
}

/***
* SearchScale
*
* Entry:
*   ulResHi - Top ULONG of quotient
*   ulResLo - Middle ULONG of quotient
*   iScale  - Scale factor of quotient, range -DEC_SCALE_MAX to DEC_SCALE_MAX
*
* Purpose:
*   Determine the max power of 10, <= 19, that the quotient can be scaled
*   up by and still fit in 96 bits.
*
* Exit:
*   Returns power of 10 to scale by, -1 if overflow error.
*
***********************************************************************/
PROFILE_NOINLINE // ONLY FOR TESTING, TODO: REMOVE IT IS Important to inline for perf
// Assumes input is not 0
int SearchScale64(const ULONG(&rgulQuo)[4], int iScale)
{
	DWORD msb;
	int iCurScale;
	DWORD64 ulResHi;

	auto hi32 = rgulQuo[2];
	auto mid32 = rgulQuo[1];
	// Quick check to stop us from trying to scale any more.
	//
	if (iScale >= DEC_SCALE_MAX || hi32 > OVFL_MAX32_1_HI) {
		iCurScale = 0;
		goto HaveScale;
	}

	ulResHi = ((DWORD64)hi32 << 32) + mid32;
	if (iScale > DEC_SCALE_MAX - SEARCHSCALE_MAX_SCALE) {
		// We can't scale by 10^19 without exceeding the max scale factor.
		// See if we can scale to the max.  If not, we'll fall into
		// standard search for scale factor.
		//
		iCurScale = DEC_SCALE_MAX - iScale;
		if (ulResHi < PowerOvfl[iCurScale].Hi)
			goto HaveScale;
		if (ulResHi == PowerOvfl[iCurScale].Hi)
			goto UpperEq;
	}

	// Multiply bit position by log10(2) to figure it's power of 10.
	// We scale the log by 256.  log(2) = .30103, * 256 = 77.  Doing this 
	// with a multiply saves a 96-byte lookup table.  The power returned
	// is <= the power of the number, so we must add one power of 10
	// to make it's integer part zero after dividing by 256.
	// 
	// Note: the result of this multiplication by an approximation of
	// log10(2) have been exhaustively checked to verify it gives the 
	// correct result.  (There were only 95 to check...)
	// 
	if (ulResHi != 0)
	{
		_BitScanReverse64(&msb, ulResHi);
		iCurScale = 63 - msb;

		iCurScale = ((iCurScale * 77) >> 8) + 1;

		if (ulResHi > PowerOvfl[iCurScale].Hi)
			iCurScale--;
		else if (ulResHi == PowerOvfl[iCurScale].Hi)
		{
		UpperEq:
			if (rgulQuo[0] > PowerOvfl[iCurScale].Lo)
				iCurScale--;
		}
	}
	else
	{
		iCurScale = SEARCHSCALE_MAX_SCALE;
	}

HaveScale:

	// iCurScale = largest power of 10 we can scale by without overflow, 
	// iCurScale < SEARCHSCALE_MAX.  See if this is enough to make scale factor 
	// positive if it isn't already.
	// 
	if (iCurScale + iScale < 0 && iCurScale != SEARCHSCALE_MAX_SCALE)
	{
		iCurScale = -1;
	}

	return iCurScale;
}

/***
* Div128By96
*
* Entry:
*   rgulNum - Pointer to 128-bit dividend as array of ULONGs, least-sig first
*   rgulDen - Pointer to 96-bit divisor.
*
* Purpose:
*   Do partial divide, yielding 32-bit result and 96-bit remainder.
*
*   Top divisor ULONG must be larger than top dividend ULONG.  This is
*   assured in the initial call because the divisor is normalized
*   and the dividend can't be. In subsequent calls, the remainder
*   is multiplied by 10^9 (max), so it can be no more than 1/4 of
*   the divisor which is effectively multiplied by 2^32 (4 * 10^9).
*
* Exit:
*   Remainder overwrites lower 96-bits of dividend.
*   Returns quotient.
*
* Exceptions:
*   None.
*
***********************************************************************/
PROFILE_NOINLINE
DWORD64 Div128By96_x64(ULONG * __restrict rgulNum, ULONG *__restrict rgulDen)
{
	LIMITED_METHOD_CONTRACT;

	DWORD64* const rgullNum = (DWORD64*)(&rgulNum[0]);
	DWORD64* const rgullDen = (DWORD64*)(&rgulDen[0]);

	DWORD64 sdlNum;
	DWORD64 sdlProd1;
	DWORD32 remainder;
	DWORD32 hi;

	if (rgulNum[3] == 0 && rgulNum[2] < rgulDen[2])
		// TODO: TEST
		//if (rgulNum[3] == 0 && *rgullNumMid64 < *rgullDenHi64)
			// Result is zero.  Entire dividend is remainder.
			//
		return 0;


	DWORD32 quo = _udiv64(rgulNum[2], rgulNum[3], rgulDen[2], &remainder);

	// Compute full remainder, rem = dividend - (quo * divisor).
	sdlProd1 = _umul64by32(rgullDen[0], quo, &hi);

	auto carry = _subborrow_u64(0, rgullNum[0], sdlProd1, &sdlNum);
	carry = _subborrow_u32(carry, remainder, hi, (DWORD32*)&rgulNum[2]);

	// Propagate carries
	//
	if (carry) {
		// Remainder went negative.  Add divisor back in until it's positive (detected by carry),
		// a max of 2 times.
		//
		do {
			quo--;
			// sdlNum += rgullDen[0];
			carry = _addcarry_u64(0, sdlNum, rgullDen[0], &sdlNum);

			// rgulNum[2] += rgulDen[2];
			carry = _addcarry_u32(carry, rgulNum[2], rgulDen[2], (DWORD32*)&rgulNum[2]);

			//_addcarry_u32(carry, rgulNum[2], 0, (DWORD32*)&rgulNum[2]);

			// OLD ::: -------
			// rgulNum[2] += rgulDen[2];
			//auto carry2 = _addcarry_u32(0, rgulNum[2], rgulDen[2], (DWORD32*)&rgulNum[2]); 
			//_addcarry_u32(carry, rgulNum[2], 0, (DWORD32*)&rgulNum[2]);

			//// TODO: Look though logic, why should we not break when we overflow due to carry??
			//if (carry2)
			//	break; // detected carry

		} while (carry == 0);
	}

	rgullNum[0] = sdlNum;
	return quo;
}



/***
* Div160By96
*
* Entry:
*   rgulNum - Pointer to 160-bit dividend as array of ULONGs, least-sig first
*   rgulDen - Pointer to 96-bit divisor.
*
* Purpose:
*   Do partial divide, yielding 64-bit result and 96-bit remainder.
*
* Exit:
*   Remainder overwrites lower 128-bits of dividend.
*   Returns quotient.
*
* Exceptions:
*   None.
*
***********************************************************************/
inline DWORD64 Div160By96_x64(ULONG rgulNum[6], ULONG rgulDen[4])
{
#if 1
	//* (DWORD64*)(&rgulRem[3]) = tmp64;
	DWORD64 quo;
	if (*(DWORD64*)(&rgulNum[3]) >= rgulDen[2])
	{
		quo = Div128By96_x64(&rgulNum[1], rgulDen) << 32;
	}
	else
	{
		quo = 0;
	}

	return quo + Div128By96_x64(&rgulNum[0], rgulDen);
#else
	This is experimental(non - working) code !


		if (*(DWORD64*)(&rgulNum[3]) < rgulDen[2])
		{
			return Div128By96_x64(&rgulNum[0], rgulDen);
		}

	DWORD64* const rgullNum = (DWORD64*)(&rgulNum[0]);
	DWORD64* const rgullDen = (DWORD64*)(&rgulDen[0]);

	DWORD64 sdlNum;
	DWORD64 sdlProd1;
	DWORD64 remainder;
	DWORD32 hi;

	if (rgulNum[4] == 0 && rgullNum[1] < (DWORD64)rgulDen[2])
		// TODO: TEST
		//if (rgulNum[3] == 0 && *rgullNumMid64 < *rgullDenHi64)
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;


	DWORD64 quo = _udiv128(*(DWORD64*)&rgulNum[1], *(DWORD64*)&rgulNum[3], *(DWORD64*)&rgulDen[1], &remainder);

	// Compute full remainder, rem = dividend - (quo * divisor).
	sdlProd1 = _umul64by32(quo, rgulDen[0], &hi);

	auto carry = _subborrow_u64(0, rgullNum[0], sdlProd1, &sdlNum);

	carry = _subborrow_u64(carry, remainder, hi, &rgullNum[1]);

	// Propagate carries
	//
	if (carry) {
		// Remainder went negative.  Add divisor back in until it's positive (detected by carry),
		// a max of 2 times.
		//
		if ((MAXDWORD64 - rgullNum[1]) > rgulDen[2])
		{
			auto scale = (MAXDWORD64 - rgullNum[1]) / rgulDen[2];
			assert(scale <= MAXDWORD32);

			// determine number of times rgulDen[2] must be added to wrap rgullNum[1] around
			// must scale * rgulDen >= (DWORD64_MAX - rgullNum[1]) 
			// sca
			quo -= scale;

			// sdlNum += rgullDen[0]*scale;
			sdlProd1 = _umul128(rgullDen[0], scale, &hi);
			carry = _addcarry_u64(0, sdlNum, sdlProd1, &sdlNum);
			carry = _addcarry_u64(carry, rgullNum[1], hi, &rgullNum[1]);

			// rgulNum[2] += rgulDen[2]*scale;
			carry |= _addcarry_u64(0, rgullNum[1], rgulDen[2] * scale, &rgullNum[1]);
		}

		// Remainder went negative.  Add divisor back in until it's positive (detected by carry),
		// a max of 2 times.
		//
		while (carry == 0) {
			quo--;
			carry = _addcarry_u64(0, sdlNum, rgullDen[0], &sdlNum);
			carry = _addcarry_u32(carry, rgulNum[2], rgulDen[2], (DWORD32*)&rgulNum[2]);
		};
	}

	rgullNum[0] = sdlNum;
	return quo;
#endif
}

/***
* Div96By64
*
* Entry:
*   rgulNum - Pointer to 96-bit dividend as array of ULONGs, least-sig first
*   sdlDen  - 64-bit divisor.
*
* Purpose:
*   Do partial divide, yielding 32-bit result and 64-bit remainder.
*   Divisor must be larger than upper 64 bits of dividend.
*
* Exit:
*   Remainder overwrites lower 64-bits of dividend.
*   Returns quotient.
*
* Exceptions:
*   None.
*
***********************************************************************/
PROFILE_NOINLINE
DWORD32 Div96By64_x64(DWORD64 *rgullNum, DWORD64 ullDen)
{
	LIMITED_METHOD_CONTRACT;

	DWORD32 quo;
	DWORD64 sdlNum;
	DWORD64 sdlProd;
	DWORD32* rgulNum = (DWORD32*)rgullNum;

	low32(sdlNum) = rgulNum[0];

	if (rgulNum[2] >= hi32(ullDen)) {
		// Divide would overflow.  Assume a quotient of 2^32, and set
		// up remainder accordingly.  Then jump to loop which reduces
		// the quotient.
		//
		hi32(sdlNum) = rgulNum[1] - low32(ullDen);
		quo = 0;
		goto NegRem;
	}

	// Hardware divide won't overflow
	// Check for 0 result, else do hardware divide
	if (rgulNum[2] == 0 && rgullNum[0] < ullDen)
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;

	quo = _udiv64(rgulNum[1], rgulNum[2], hi32(ullDen), &hi32(sdlNum));

	// Compute full remainder, rem = dividend - (quo * divisor).
	//
	sdlProd = UInt32x32To64(quo, low32(ullDen)); // quo * lo divisor
	auto carry = _subborrow_u64(0, sdlNum, sdlProd, &sdlNum);
	if (carry) {
	NegRem:
		// Remainder went negative.  Add divisor back in until it's positive,
		// a max of 2 times.
		//
		do {
			quo--;
			sdlNum += ullDen;
		} while (sdlNum >= ullDen);
	}

	rgullNum[0] = sdlNum;
	return quo;
}

/***
* Div128By64
*
* Entry:
*   rgulNum - Pointer to 128-bit dividend as array of ULONGs, least-sig first
*   sdlDen  - 64-bit divisor.
*
* Purpose:
*   Do partial divide, yielding 64-bit result and 64-bit remainder.
*   Divisor must be larger than upper 64 bits of dividend.
*
* Exit:
*   Remainder overwrites lower 64-bits of dividend.
*   Returns quotient.
*
* Exceptions:
*   None.
*
***********************************************************************/
PROFILE_NOINLINE
DWORD64 Div128By64_x64(DWORD64 *rgullNum, DWORD64 ullDen)
{
#ifdef NO_UDIV128
	DWORD64 res = Div96By64_x64((DWORD64*)&hi32(rgullNum[0]), ullDen);
	return (res << 32) + Div96By64_x64(rgullNum, ullDen);
#else
	LIMITED_METHOD_CONTRACT;

	DWORD64 sdlNum;
	DWORD64 quotient;

	DWORD64 hi64 = rgullNum[1];

	if (hi64 >= ullDen) {
		// Divide would overflow.  Assume a quotient of 2^64, and set
		// up remainder accordingly.  Then jump to loop which reduces
		// the quotient.
		//
		sdlNum = rgullNum[1];
		quotient = 0;

		// Remainder went negative.  Add divisor back in until it's positive,
		// a max of 2 times.
		//
		do {
			quotient--;
			sdlNum += ullDen;
		} while (sdlNum >= ullDen);

		rgullNum[0] = sdlNum;
		return quotient;
	}

	// Hardware divide won't overflow
	// Check for 0 result, else do hardware divide
	DWORD64 lo64 = rgullNum[0];
	if (hi64 == 0 && lo64 < ullDen)
	{
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;
	}

	quotient = _udiv128(lo64, hi64, ullDen, &rgullNum[0]);
	return quotient;
#endif
}



//**********************************************************************
//
// VarDecDiv_x64 - Decimal Divide
// x64 Implementation of VarDecDiv
//
//**********************************************************************
STDAPI VarDecDiv_x64(const DECIMAL *pdecL, const DECIMAL * pdecR, DECIMAL *__restrict  pdecRes)
{
	ULONG   rgulQuo[4]; //[3];
	//ULONG   rgulQuoSave[4]; //[3];
	DWORD64 rgullQuoSave[2];
	ULONG   rgulRem[6]; // [4]
	ULONG   rgulDivisor[4]; //[3];
	ULONG   ulTmp;
	DWORD64 ullTmp64;
	DWORD64 ullDivisor;
	int     iScale;
	int     iCurScale;

	DWORD64* rgullRem = (DWORD64*)(&rgulRem[0]);
	DWORD64* rgullDivisor = (DWORD64*)(&rgulDivisor[0]);
	DWORD64* rgullQuo = (DWORD64*)(&rgulQuo[0]);

	// not part of oleauto impl, only in decimal.cpp from classlibnative in corecrl
	// TODO: Determine if this it improves x64 impl or not
	// for original code the perf diff is -1% to +3% (where later is in case 64bit / 64bit where 64bit*64it -> 64bit)
	// most other scenarios has a small perf inpact from it
	BOOL    fUnscale;

	iScale = pdecL->scale - pdecR->scale;
	fUnscale = FALSE;
	rgullDivisor[0] = pdecR->Lo64;
	rgulDivisor[2] = pdecR->Hi32;

	if (rgulDivisor[2] == 0 && FitsIn32Bit(&rgullDivisor[0])) {
		// Divisor is only 32 bits.  Easy divide.
		//
		if (rgulDivisor[0] == 0)
			return DISP_E_DIVBYZERO;

		// Store dividend in rgu*Quo and use call divide to finally get 
		// remainder in rgulRem and qutient in rgu*Quo
		//
		rgullQuo[0] = pdecL->Lo64;
		rgulQuo[2] = pdecL->Hi32;
		rgulRem[0] = Div96By32_x64(rgulQuo, rgulDivisor[0]);

		for (;;) {
			if (rgulRem[0] == 0) {
				if (iScale < 0) {
					iCurScale = min(POWER10_MAX32, -iScale);
					goto HaveScale32;
				}
				break;
			}
			// We need to unscale if and only if we have a non-zero remainder
			fUnscale = TRUE;

			// We have computed a quotient based on the natural scale 
			// ( <dividend scale> - <divisor scale> ).  We have a non-zero 
			// remainder, so now we should increase the scale if possible to 
			// include more quotient bits.
			// 
			// If it doesn't cause overflow, we'll loop scaling by 10^19 and 
			// computing more quotient bits as long as the remainder stays 
			// non-zero.  If scaling by that much would cause overflow, we'll 
			// drop out of the loop and scale by as much as we can.
			// 
			// Scaling by 10^9 will overflow if rgulQuo[2].rgulQuo[1] >= 2^32 / 10^9 
			// = 4.294 967 296.  So the upper limit is rgulQuo[2] == 4 and 
			// rgulQuo[1] == 0.294 967 296 * 2^32 = 1,266,874,889.7+.  Since 
			// quotient bits in rgulQuo[0] could be all 1's, then 1,266,874,888 
			// is the largest value in rgulQuo[1] (when rgulQuo[2] == 4) that is 
			// assured not to overflow.
			// 
			iCurScale = SearchScale64(rgulQuo, iScale);
			if (iCurScale == 0) {
				// No more scaling to be done, but remainder is non-zero.
				// Round quotient.
				//
				ulTmp = rgulRem[0] << 1;
				if (ulTmp < rgulRem[0] ||
					(ulTmp > rgulDivisor[0] || (ulTmp == rgulDivisor[0] && (rgulQuo[0] & 1)))) {
				RoundUp:
					Add96(rgulQuo, 1);
				}
				break;
			}

			if (iCurScale == -1)
				return DISP_E_OVERFLOW;

			iCurScale = min(iCurScale, POWER10_MAX32);
		HaveScale32:
			DWORD32 ullPwr32 = (DWORD32)rgulPower10_64[iCurScale];
			iScale += iCurScale;

			if (IncreaseScale96By32(rgulQuo, ullPwr32) != 0)
				return DISP_E_OVERFLOW;

			// We can use a single _udiv64 here since upper 32bits must be less than rgulDivisor<<2^32
			// since ullPwr < 2^32 and remainder < divisor
			auto num = UInt32x32To64(rgulRem[0], (DWORD32)ullPwr32);
			DWORD32 ullTmp32 = _udiv64(low32(num), hi32(num), rgulDivisor[0], &low32(rgullRem[0]));
			Add96(rgulQuo, ullTmp32);
		} // for (;;)
	}
	else {
		// Divisor has bits set in the upper 64 bits.
		//
		// Divisor must be fully normalized (shifted so bit 31 of the most 
		// significant ULONG is 1).  Locate the MSB so we know how much to 
		// normalize by.  The dividend will be shifted by the same amount so 
		// the quotient is not changed.
		//
		if (rgulDivisor[2] == 0)
			ulTmp = rgulDivisor[1];
		else
			ulTmp = rgulDivisor[2];

		DWORD msb;
		BOOL found = BitScanReverse(&msb, ulTmp);
		iCurScale = 31 - msb;
		assert(found);

		// Shift both dividend and divisor left by iCurScale.
		// 
		rgullRem[0] = pdecL->Lo64 << iCurScale;
		rgullRem[1] = ShiftLeft128(pdecL->Lo64, pdecL->Hi32, (BYTE)iCurScale);
		ullDivisor = rgullDivisor[0] << iCurScale;

		if (rgulDivisor[2] == 0) {
			// Have a 64-bit divisor in sdlDivisor.  The remainder
			// (currently 96 bits spread over 4 ULONGs) will be < divisor.
			//

			rgulQuo[2] = 0;
			rgullQuo[0] = Div128By64_x64(rgullRem, ullDivisor);

			for (;;) {
				if (rgullRem[0] == 0) {
					if (iScale < 0) {
						iCurScale = min(POWER10_MAX64, -iScale);
						goto HaveScale64;
					}
					break;
				}

				// We need to unscale if and only if we have a non-zero remainder
				fUnscale = TRUE;

				// Remainder is non-zero.  Scale up quotient and remainder by 
				// powers of 10 so we can compute more significant bits.
				// 
				iCurScale = SearchScale64(rgulQuo, iScale);
				if (iCurScale == 0) {
					// No more scaling to be done, but remainder is non-zero.
					// Round quotient.
					//
					ullTmp64 = rgullRem[0];
					if (ullTmp64 >= 0x8000000000000000Ui64 || (ullTmp64 <<= 1) > ullDivisor ||
						(ullTmp64 == ullDivisor && (rgulQuo[0] & 1)))
						goto RoundUp;
					break;
				}

				if (iCurScale == -1)
					return DISP_E_OVERFLOW;

			HaveScale64:
				iCurScale = min(iCurScale, POWER10_MAX32);
				DWORD32 ullPwr32 = (DWORD32)rgulPower10_64[iCurScale];
				iScale += iCurScale;

				if (IncreaseScale96By32(rgulQuo, ullPwr32) != 0)
					return DISP_E_OVERFLOW;

				// Reminder is at most 64bits, a single multiply is needed to IncreaseScale
				// result is up to 96 bits
				rgullRem[0] = _umul64by32(rgullRem[0], ullPwr32, &low32(rgullRem[1]));
				DWORD32 tmp32 = Div96By64_x64(rgullRem, ullDivisor);
				Add96(rgulQuo, tmp32);
			} // for (;;)
		}
		else {
			// Have a 96-bit divisor in rgulDivisor[].
			//
			// Start by finishing the shift left by iCurScale.
			//
			rgullDivisor[1] = ShiftLeft128(rgullDivisor[0], rgullDivisor[1], (BYTE)iCurScale);
			rgullDivisor[0] = ullDivisor;

			// The remainder (currently 96 bits spread over 4 ULONGs) 
			// will be < divisor.
			// 
			rgulQuo[2] = 0;
			rgullQuo[0] = Div128By96_x64(rgulRem, rgulDivisor);

			for (;;) {
				if ((rgullRem[0] | rgulRem[2]) == 0) {
					if (iScale < 0) {
						iCurScale = min(POWER10_MAX64, -iScale);
						goto HaveScale96;
					}
					break;
				}

				// We need to unscale if and only if we have a non-zero remainder
				fUnscale = TRUE;

				// Remainder is non-zero.  Scale up quotient and remainder by 
				// powers of 10 so we can compute more significant bits.
				// 
				iCurScale = SearchScale64(rgulQuo, iScale);
				if (iCurScale == 0) {
					// No more scaling to be done, but remainder is non-zero.
					// Round quotient.
					//
					if (rgulRem[2] >= 0x80000000)
						goto RoundUp;

					// multiply reminder by 2, was "shift 1" but add/adc instruction is faster for a wider range of CPU's
					// and is recommended especially for older CPUs
					auto carry = _addcarry_u64(0, rgullRem[0], rgullRem[0], &rgullRem[0]);
					_addcarry_u32(carry, rgulRem[2], rgulRem[2], (unsigned int*)&rgulRem[2]);

					if (rgulRem[2] > rgulDivisor[2] || (rgulRem[2] == rgulDivisor[2] &&
						(rgullRem[0] > rgullDivisor[0] || (rgullRem[0] == rgullDivisor[0] &&
						(rgulQuo[0] & 1)))))
						goto RoundUp;
					break;
				}

				if (iCurScale == -1)
					return DISP_E_OVERFLOW;

			HaveScale96:
				DWORD64 quo;
#ifdef NO_UDIV128
				iCurScale = min(iCurScale, POWER10_MAX32);
				DWORD32 ullPwr32 = (DWORD32)rgulPower10_64[iCurScale];
				iScale += iCurScale;

				if (IncreaseScale96By32(rgulQuo, ullPwr32) != 0)
					return DISP_E_OVERFLOW;

				rgulRem[3] = IncreaseScale96By32(rgulRem, ullPwr32);
				quo = Div128By96_x64(rgulRem, rgulDivisor);
#else
				DWORD64 ullPwr64 = rgulPower10_64[iCurScale];
				iScale += iCurScale;

				if (IncreaseScale96By64(rgulQuo, ullPwr64) != 0)
					return DISP_E_OVERFLOW;

				DWORD64 tmp64 = IncreaseScale96By64(rgulRem, ullPwr64);
				*(DWORD64*)(&rgulRem[3]) = tmp64;
				quo = Div160By96_x64(rgulRem, rgulDivisor);
#endif
				Add96(rgulQuo, quo);
			} // for (;;)
		}
	}

	// We need to unscale if and only if we have a non-zero remainder
	if (fUnscale) {

		// No more remainder.  Try extracting any extra powers of 10 we may have 
		// added.  We do this by trying to divide out 10^8, 10^4, 10^2, and 10^1.
		// If a division by one of these powers returns a zero remainder, then
		// we keep the quotient.  If the remainder is not zero, then we restore
		// the previous value.
		// 
		// Since 10 = 2 * 5, there must be a factor of 2 for every power of 10
		// we can extract.  We use this as a quick test on whether to try a
		// given power.
		// 
		while ((rgullQuo[0] & 0xFF) == 0 && iScale >= 8) {
			rgullQuoSave[0] = rgullQuo[0];
			rgullQuoSave[1] = rgullQuo[1];

			if (Div96By32_x64(rgullQuoSave, 100000000) == 0) {
				rgullQuo[0] = rgullQuoSave[0];
				rgullQuo[1] = rgullQuoSave[1];
				iScale -= 8;
			}
			else
				break;
		}

		if ((rgullQuo[0] & 0xF) == 0 && iScale >= 4) {
			rgullQuoSave[0] = rgullQuo[0];
			rgullQuoSave[1] = rgullQuo[1];

			if (Div96By32_x64(rgullQuoSave, 10000) == 0) {
				rgullQuo[0] = rgullQuoSave[0];
				rgullQuo[1] = rgullQuoSave[1];
				iScale -= 4;
			}
		}

		if ((rgullQuo[0] & 3) == 0 && iScale >= 2) {
			rgullQuoSave[0] = rgullQuo[0];
			rgullQuoSave[1] = rgullQuo[1];

			if (Div96By32_x64(rgullQuoSave, 100) == 0) {
				rgullQuo[0] = rgullQuoSave[0];
				rgullQuo[1] = rgullQuoSave[1];
				iScale -= 2;
			}
		}

		if ((rgullQuo[0] & 1) == 0 && iScale >= 1) {
			rgullQuoSave[0] = rgullQuo[0];
			rgullQuoSave[1] = rgullQuo[1];

			if (Div96By32_x64(rgullQuoSave, 10) == 0) {
				rgullQuo[0] = rgullQuoSave[0];
				rgullQuo[1] = rgullQuoSave[1];
				iScale -= 1;
			}
		}
	}

	pdecRes->Hi32 = (DWORD32)rgullQuo[1];
	pdecRes->Lo64 = rgullQuo[0];
	pdecRes->scale = (BYTE)iScale;
	pdecRes->sign = pdecL->sign ^ pdecR->sign;
	pdecRes->wReserved = 0; // not part of oleauto impl, only in decimal.cpp from classlibnative in corecrl
	return NOERROR;
}


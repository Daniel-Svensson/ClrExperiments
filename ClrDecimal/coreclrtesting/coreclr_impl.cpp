// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//

//
// ===========================================================================
// File: decarith.cpp
// 
// ===========================================================================
/***
*
*Purpose:
*  Implement arithmetic for Decimal data type.
*
*Implementation Notes:
*
*****************************************************************************/

#include "stdafx.h"

static const ULONG ulTenToTenDiv4 = 2500000000U;
static const ULONG ulTenToNine = 1000000000U;
//#include "..\..\coreclr\src\palrt\common.h"
//#include <oleauto.h>
//#include "..\..\coreclr\src\palrt\convert.h"


//***********************************************************************
//
// Additional Decimal and Int64 definitions
//
#define COPYDEC(dest, src) {DECIMAL_SIGNSCALE(dest) = DECIMAL_SIGNSCALE(src); DECIMAL_HI32(dest) = DECIMAL_HI32(src); \
    DECIMAL_MID32(dest) = DECIMAL_MID32(src); DECIMAL_LO32(dest) = DECIMAL_LO32(src); }

#define DEC_SCALE_MAX   28
#define POWER10_MAX     9

// The following functions are defined in the classlibnative\bcltype\decimal.cpp
ULONG Div96By32(ULONG *rgulNum, ULONG ulDen);
ULONG Div96By64(ULONG *rgulNum, SPLIT64 sdlDen);
ULONG Div128By96(ULONG *rgulNum, ULONG *rgulDen);
ULONG IncreaseScale(ULONG *rgulNum, ULONG ulPwr);

//***********************************************************************
//
// Data tables
//

static ULONG rgulPower10[POWER10_MAX + 1] = { 1, 10, 100, 1000, 10000, 100000, 1000000,
10000000, 100000000, 1000000000 };

struct DECOVFL
{
	ULONG Hi;
	ULONG Mid;
};

static DECOVFL PowerOvfl[] = {
	// This is a table of the largest values that can be in the upper two
	// ULONGs of a 96-bit number that will not overflow when multiplied
	// by a given power.  For the upper word, this is a table of 
	// 2^32 / 10^n for 1 <= n <= 9.  For the lower word, this is the
	// remaining fraction part * 2^32.  2^32 = 4294967296.
	//
	{ 429496729UL, 2576980377UL }, // 10^1 remainder 0.6
	{ 42949672UL,  4123168604UL }, // 10^2 remainder 0.16
	{ 4294967UL,   1271310319UL }, // 10^3 remainder 0.616
	{ 429496UL,    3133608139UL }, // 10^4 remainder 0.1616
	{ 42949UL,     2890341191UL }, // 10^5 remainder 0.51616
	{ 4294UL,      4154504685UL }, // 10^6 remainder 0.551616
	{ 429UL,       2133437386UL }, // 10^7 remainder 0.9551616
	{ 42UL,        4078814305UL }, // 10^8 remainder 0.09991616
								   //  { 4UL,         1266874889UL }, // 10^9 remainder 0.709551616
};

#define OVFL_MAX_9_HI   4
#define OVFL_MAX_9_MID  1266874889

#define OVFL_MAX_5_HI   42949
#define OVFL_MAX_5_MID  2890341191

#define OVFL_MAX_1_HI   429496729



//***********************************************************************
//
// static helper functions
//

/***
* ScaleResult
*
* Entry:
*   rgulRes - Array of ULONGs with value, least-significant first.
*   iHiRes  - Index of last non-zero value in rgulRes.
*   iScale  - Scale factor for this value, range 0 - 2 * DEC_SCALE_MAX
*
* Purpose:
*   See if we need to scale the result to fit it in 96 bits.
*   Perform needed scaling.  Adjust scale factor accordingly.
*
* Exit:
*   rgulRes updated in place, always 3 ULONGs.
*   New scale factor returned, -1 if overflow error.
*
***********************************************************************/

int ScaleResult_CORECLR(ULONG *rgulRes, int iHiRes, int iScale)
{
	LIMITED_METHOD_CONTRACT;

	int     iNewScale;
	int     iCur;
	ULONG   ulPwr;
	ULONG   ulTmp;
	ULONG   ulSticky;
	SPLIT64 sdlTmp;

	// See if we need to scale the result.  The combined scale must
	// be <= DEC_SCALE_MAX and the upper 96 bits must be zero.
	// 
	// Start by figuring a lower bound on the scaling needed to make
	// the upper 96 bits zero.  iHiRes is the index into rgulRes[]
	// of the highest non-zero ULONG.
	// 
	iNewScale = iHiRes * 32 - 64 - 1;
	if (iNewScale > 0) {

		// Find the MSB.
		//
		ulTmp = rgulRes[iHiRes];
		if (!(ulTmp & 0xFFFF0000)) {
			iNewScale -= 16;
			ulTmp <<= 16;
		}
		if (!(ulTmp & 0xFF000000)) {
			iNewScale -= 8;
			ulTmp <<= 8;
		}
		if (!(ulTmp & 0xF0000000)) {
			iNewScale -= 4;
			ulTmp <<= 4;
		}
		if (!(ulTmp & 0xC0000000)) {
			iNewScale -= 2;
			ulTmp <<= 2;
		}
		if (!(ulTmp & 0x80000000)) {
			iNewScale--;
			ulTmp <<= 1;
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
		ulSticky = 0;
		sdlTmp.u.Hi = 0; // initialize remainder

		for (;;) {

			ulSticky |= sdlTmp.u.Hi; // record remainder as sticky bit

			if (iNewScale > POWER10_MAX)
				ulPwr = ulTenToNine;
			else
				ulPwr = rgulPower10[iNewScale];

			// Compute first quotient.
			// DivMod64by32 returns quotient in Lo, remainder in Hi.
			//
			sdlTmp.int64 = DivMod64by32(rgulRes[iHiRes], ulPwr);
			rgulRes[iHiRes] = sdlTmp.u.Lo;
			iCur = iHiRes - 1;

			if (iCur >= 0) {
				// If first quotient was 0, update iHiRes.
				//
				if (sdlTmp.u.Lo == 0)
					iHiRes--;

				// Compute subsequent quotients.
				//
				do {
					sdlTmp.u.Lo = rgulRes[iCur];
					sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulPwr);
					rgulRes[iCur] = sdlTmp.u.Lo;
					iCur--;
				} while (iCur >= 0);

			}

			iNewScale -= POWER10_MAX;
			if (iNewScale > 0)
				continue; // scale some more

						  // If we scaled enough, iHiRes would be 2 or less.  If not,
						  // divide by 10 more.
						  //
			if (iHiRes > 2) {
				iNewScale = 1;
				iScale--;
				continue; // scale by 10
			}

			// Round final result.  See if remainder >= 1/2 of divisor.
			// If remainder == 1/2 divisor, round up if odd or sticky bit set.
			//
			ulPwr >>= 1;  // power of 10 always even
			if (ulPwr <= sdlTmp.u.Hi && (ulPwr < sdlTmp.u.Hi ||
				((rgulRes[0] & 1) | ulSticky))) {
				iCur = -1;
				while (++rgulRes[++iCur] == 0);

				if (iCur > 2) {
					// The rounding caused us to carry beyond 96 bits. 
					// Scale by 10 more.
					//
					iHiRes = iCur;
					ulSticky = 0;  // no sticky bit
					sdlTmp.u.Hi = 0; // or remainder
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

/***
* FullDiv64By32
*
* Entry:
*   pdlNum  - Pointer to 64-bit dividend
*   ulDen   - 32-bit divisor
*
* Purpose:
*   Do full divide, yielding 64-bit result and 32-bit remainder.
*
* Exit:
*   Quotient overwrites dividend.
*   Returns remainder.
*
* Exceptions:
*   None.
*
***********************************************************************/

static ULONG FullDiv64By32(DWORDLONG *pdlNum, ULONG ulDen)
{
	SPLIT64  sdlTmp;
	SPLIT64  sdlRes;

	sdlTmp.int64 = *pdlNum;
	sdlRes.u.Hi = 0;

	if (sdlTmp.u.Hi >= ulDen) {
		// DivMod64by32 returns quotient in Lo, remainder in Hi.
		//
		sdlRes.u.Lo = sdlTmp.u.Hi;
		sdlRes.int64 = DivMod64by32(sdlRes.int64, ulDen);
		sdlTmp.u.Hi = sdlRes.u.Hi;
		sdlRes.u.Hi = sdlRes.u.Lo;
	}

	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulDen);
	sdlRes.u.Lo = sdlTmp.u.Lo;
	*pdlNum = sdlRes.int64;
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
*   Determine the max power of 10, <= 9, that the quotient can be scaled
*   up by and still fit in 96 bits.
*
* Exit:
*   Returns power of 10 to scale by, -1 if overflow error.
*
***********************************************************************/

int SearchScale(ULONG ulResHi, ULONG ulResLo, int iScale)
{
	int   iCurScale;

	// Quick check to stop us from trying to scale any more.
	//
	if (ulResHi > OVFL_MAX_1_HI || iScale >= DEC_SCALE_MAX) {
		iCurScale = 0;
		goto HaveScale;
	}

	if (iScale > DEC_SCALE_MAX - 9) {
		// We can't scale by 10^9 without exceeding the max scale factor.
		// See if we can scale to the max.  If not, we'll fall into
		// standard search for scale factor.
		//
		iCurScale = DEC_SCALE_MAX - iScale;
		if (ulResHi < PowerOvfl[iCurScale - 1].Hi)
			goto HaveScale;

		if (ulResHi == PowerOvfl[iCurScale - 1].Hi) {
		UpperEq:
			if (ulResLo >= PowerOvfl[iCurScale - 1].Mid)
				iCurScale--;
			goto HaveScale;
		}
	}
	else if (ulResHi < OVFL_MAX_9_HI || (ulResHi == OVFL_MAX_9_HI &&
		ulResLo < OVFL_MAX_9_MID))
		return 9;

	// Search for a power to scale by < 9.  Do a binary search
	// on PowerOvfl[].
	//
	iCurScale = 5;
	if (ulResHi < OVFL_MAX_5_HI)
		iCurScale = 7;
	else if (ulResHi > OVFL_MAX_5_HI)
		iCurScale = 3;
	else
		goto UpperEq;

	// iCurScale is 3 or 7.
	//
	if (ulResHi < PowerOvfl[iCurScale - 1].Hi)
		iCurScale++;
	else if (ulResHi > PowerOvfl[iCurScale - 1].Hi)
		iCurScale--;
	else
		goto UpperEq;

	// iCurScale is 2, 4, 6, or 8.
	//
	// In all cases, we already found we could not use the power one larger.
	// So if we can use this power, it is the biggest, and we're done.  If
	// we can't use this power, the one below it is correct for all cases 
	// unless it's 10^1 -- we might have to go to 10^0 (no scaling).
	// 
	if (ulResHi > PowerOvfl[iCurScale - 1].Hi)
		iCurScale--;

	if (ulResHi == PowerOvfl[iCurScale - 1].Hi)
		goto UpperEq;

HaveScale:
	// iCurScale = largest power of 10 we can scale by without overflow, 
	// iCurScale < 9.  See if this is enough to make scale factor 
	// positive if it isn't already.
	// 
	if (iCurScale + iScale < 0)
		iCurScale = -1;

	return iCurScale;
}

/***
* DecFixInt
*
* Entry:
*   pdecRes - Pointer to Decimal result location
*   pdecIn  - Pointer to Decimal operand
*
* Purpose:
*   Chop the value to integer.  Return remainder so Int() function
*   can round down if non-zero.
*
* Exit:
*   Returns remainder.
*
* Exceptions:
*   None.
*
***********************************************************************/

ULONG DecFixInt(LPDECIMAL pdecRes, LPDECIMAL pdecIn)
{
	ULONG   rgulNum[3];
	ULONG   ulRem;
	ULONG   ulPwr;
	int     iScale;

	if (pdecIn->scale > 0) {
		rgulNum[0] = pdecIn->Lo32;
		rgulNum[1] = pdecIn->Mid32;
		rgulNum[2] = pdecIn->Hi32;
		iScale = pdecIn->scale;
		pdecRes->sign = pdecIn->sign;
		ulRem = 0;

		do {
			if (iScale > POWER10_MAX)
				ulPwr = ulTenToNine;
			else
				ulPwr = rgulPower10[iScale];

			ulRem |= Div96By32(rgulNum, ulPwr);
			iScale -= 9;
		} while (iScale > 0);

		pdecRes->Lo32 = rgulNum[0];
		pdecRes->Mid32 = rgulNum[1];
		pdecRes->Hi32 = rgulNum[2];
		pdecRes->scale = 0;

		return ulRem;
	}

	COPYDEC(*pdecRes, *pdecIn)
		return 0;
}


//***********************************************************************
//
// 
//

//**********************************************************************
//
// VarDecMul - Decimal Multiply
//
//**********************************************************************

STDAPI VarDecMul_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	SPLIT64 sdlTmp;
	SPLIT64 sdlTmp2;
	SPLIT64 sdlTmp3;
	int     iScale;
	int     iHiProd;
	ULONG   ulPwr;
	ULONG   ulRemLo;
	ULONG   ulRemHi;
	ULONG   rgulProd[6];

	iScale = pdecL->scale + pdecR->scale;

	if ((pdecL->Hi32 | pdecL->Mid32 | pdecR->Hi32 | pdecR->Mid32) == 0)
	{
		// Upper 64 bits are zero.
		//
		sdlTmp.int64 = UInt32x32To64(pdecL->Lo32, pdecR->Lo32);
		if (iScale > DEC_SCALE_MAX)
		{
			// Result iScale is too big.  Divide result by power of 10 to reduce it.
			// If the amount to divide by is > 19 the result is guaranteed
			// less than 1/2.  [max value in 64 bits = 1.84E19]
			//
			iScale -= DEC_SCALE_MAX;
			if (iScale > 19)
			{
			ReturnZero:
				DECIMAL_SETZERO(*pdecRes);
				return NOERROR;
			}
			if (iScale > POWER10_MAX)
			{
				// Divide by 1E10 first, to get the power down to a 32-bit quantity.
				// 1E10 itself doesn't fit in 32 bits, so we'll divide by 2.5E9 now
				// then multiply the next divisor by 4 (which will be a max of 4E9).
				// 
				ulRemLo = FullDiv64By32(&sdlTmp.int64, ulTenToTenDiv4);
				ulPwr = rgulPower10[iScale - 10] << 2;
			}
			else
			{
				ulPwr = rgulPower10[iScale];
				ulRemLo = 0;
			}

			// Power to divide by fits in 32 bits.
			//
			ulRemHi = FullDiv64By32(&sdlTmp.int64, ulPwr);

			// Round result.  See if remainder >= 1/2 of divisor.
			// Divisor is a power of 10, so it is always even.
			//
			ulPwr >>= 1;
			if (ulRemHi >= ulPwr && (ulRemHi > ulPwr || (ulRemLo | (sdlTmp.u.Lo & 1))))
				sdlTmp.int64++;

			iScale = DEC_SCALE_MAX;
		}
		DECIMAL_LO32(*pdecRes) = sdlTmp.u.Lo;
		DECIMAL_MID32(*pdecRes) = sdlTmp.u.Hi;
		DECIMAL_HI32(*pdecRes) = 0;
	}
	else
	{

		// At least one operand has bits set in the upper 64 bits.
		//
		// Compute and accumulate the 9 partial products into a 
		// 192-bit (24-byte) result.
		//
		//                [l-h][l-m][l-l]   left high, middle, low
		//             x  [r-h][r-m][r-l]   right high, middle, low
		// ------------------------------
		//
		//                     [0-h][0-l]   l-l * r-l
		//                [1ah][1al]        l-l * r-m
		//                [1bh][1bl]        l-m * r-l
		//           [2ah][2al]             l-m * r-m
		//           [2bh][2bl]             l-l * r-h
		//           [2ch][2cl]             l-h * r-l
		//      [3ah][3al]                  l-m * r-h
		//      [3bh][3bl]                  l-h * r-m
		// [4-h][4-l]                       l-h * r-h
		// ------------------------------
		// [p-5][p-4][p-3][p-2][p-1][p-0]   prod[] array
		//
		sdlTmp.int64 = UInt32x32To64(pdecL->Lo32, pdecR->Lo32);
		rgulProd[0] = sdlTmp.u.Lo;

		sdlTmp2.int64 = UInt32x32To64(pdecL->Lo32, pdecR->Mid32) + sdlTmp.u.Hi;

		sdlTmp.int64 = UInt32x32To64(pdecL->Mid32, pdecR->Lo32);
		sdlTmp.int64 += sdlTmp2.int64; // this could generate carry
		rgulProd[1] = sdlTmp.u.Lo;
		if (sdlTmp.int64 < sdlTmp2.int64) // detect carry
			sdlTmp2.u.Hi = 1;
		else
			sdlTmp2.u.Hi = 0;
		sdlTmp2.u.Lo = sdlTmp.u.Hi;

		sdlTmp.int64 = UInt32x32To64(pdecL->Mid32, pdecR->Mid32) + sdlTmp2.int64;

		if (pdecL->Hi32 | pdecR->Hi32) {
			// Highest 32 bits is non-zero.  Calculate 5 more partial products.
			//
			sdlTmp2.int64 = UInt32x32To64(pdecL->Lo32, pdecR->Hi32);
			sdlTmp.int64 += sdlTmp2.int64; // this could generate carry
			if (sdlTmp.int64 < sdlTmp2.int64) // detect carry
				sdlTmp3.u.Hi = 1;
			else
				sdlTmp3.u.Hi = 0;

			sdlTmp2.int64 = UInt32x32To64(pdecL->Hi32, pdecR->Lo32);
			sdlTmp.int64 += sdlTmp2.int64; // this could generate carry
			rgulProd[2] = sdlTmp.u.Lo;
			if (sdlTmp.int64 < sdlTmp2.int64) // detect carry
				sdlTmp3.u.Hi++;
			sdlTmp3.u.Lo = sdlTmp.u.Hi;

			sdlTmp.int64 = UInt32x32To64(pdecL->Mid32, pdecR->Hi32);
			sdlTmp.int64 += sdlTmp3.int64; // this could generate carry
			if (sdlTmp.int64 < sdlTmp3.int64) // detect carry
				sdlTmp3.u.Hi = 1;
			else
				sdlTmp3.u.Hi = 0;

			sdlTmp2.int64 = UInt32x32To64(pdecL->Hi32, pdecR->Mid32);
			sdlTmp.int64 += sdlTmp2.int64; // this could generate carry
			rgulProd[3] = sdlTmp.u.Lo;
			if (sdlTmp.int64 < sdlTmp2.int64) // detect carry
				sdlTmp3.u.Hi++;
			sdlTmp3.u.Lo = sdlTmp.u.Hi;

			sdlTmp.int64 = UInt32x32To64(pdecL->Hi32, pdecR->Hi32) + sdlTmp3.int64;
			rgulProd[4] = sdlTmp.u.Lo;
			rgulProd[5] = sdlTmp.u.Hi;

			iHiProd = 5;
		}
		else {
			rgulProd[2] = sdlTmp.u.Lo;
			rgulProd[3] = sdlTmp.u.Hi;
			iHiProd = 3;
		}

		// Check for leading zero ULONGs on the product
		//
		while (rgulProd[iHiProd] == 0) {
			iHiProd--;
			if (iHiProd < 0)
				goto ReturnZero;
		}

		iScale = ScaleResult_CORECLR(rgulProd, iHiProd, iScale);
		if (iScale == -1)
			return DISP_E_OVERFLOW;

		pdecRes->Lo32 = rgulProd[0];
		pdecRes->Mid32 = rgulProd[1];
		pdecRes->Hi32 = rgulProd[2];
	}

	pdecRes->sign = pdecR->sign ^ pdecL->sign;
	pdecRes->scale = (char)iScale;
	return NOERROR;
}


//**********************************************************************
//
// VarDecAdd - Decimal Addition
// VarDecSub - Decimal Subtraction
//
//**********************************************************************

static HRESULT DecAddSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes, char bSign);

STDAPI VarDecAdd_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	return DecAddSub_PALRT(pdecL, pdecR, pdecRes, 0);
}


STDAPI VarDecSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	return DecAddSub_PALRT(pdecL, pdecR, pdecRes, DECIMAL_NEG);
}


static HRESULT DecAddSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes, char bSign)
{
	ULONG     rgulNum[6];
	ULONG     ulPwr;
	int       iScale;
	int       iHiProd;
	int       iCur;
	SPLIT64   sdlTmp;
	DECIMAL   decRes;
	DECIMAL   decTmp;
	LPDECIMAL pdecTmp;

	bSign ^= (pdecR->sign ^ pdecL->sign) & DECIMAL_NEG;

	if (pdecR->scale == pdecL->scale) {
		// Scale factors are equal, no alignment necessary.
		//
		decRes.signscale = pdecL->signscale;

	AlignedAdd:
		if (bSign) {
			// Signs differ - subtract
			//
			DECIMAL_LO64_SET(decRes, DECIMAL_LO64_GET(*pdecL) - DECIMAL_LO64_GET(*pdecR));
			DECIMAL_HI32(decRes) = DECIMAL_HI32(*pdecL) - DECIMAL_HI32(*pdecR);

			// Propagate carry
			//
			if (DECIMAL_LO64_GET(decRes) > DECIMAL_LO64_GET(*pdecL)) {
				decRes.Hi32--;
				if (decRes.Hi32 >= pdecL->Hi32)
					goto SignFlip;
			}
			else if (decRes.Hi32 > pdecL->Hi32) {
				// Got negative result.  Flip its sign.
				//
			SignFlip:
				DECIMAL_LO64_SET(decRes, -(LONGLONG)DECIMAL_LO64_GET(decRes));
				decRes.Hi32 = ~decRes.Hi32;
				if (DECIMAL_LO64_GET(decRes) == 0)
					decRes.Hi32++;
				decRes.sign ^= DECIMAL_NEG;
			}

		}
		else {
			// Signs are the same - add
			//
			DECIMAL_LO64_SET(decRes, DECIMAL_LO64_GET(*pdecL) + DECIMAL_LO64_GET(*pdecR));
			decRes.Hi32 = pdecL->Hi32 + pdecR->Hi32;

			// Propagate carry
			//
			if (DECIMAL_LO64_GET(decRes) < DECIMAL_LO64_GET(*pdecL)) {
				decRes.Hi32++;
				if (decRes.Hi32 <= pdecL->Hi32)
					goto AlignedScale;
			}
			else if (decRes.Hi32 < pdecL->Hi32) {
			AlignedScale:
				// The addition carried above 96 bits.  Divide the result by 10,
				// dropping the scale factor.
				//
				if (decRes.scale == 0)
					return DISP_E_OVERFLOW;
				decRes.scale--;

				sdlTmp.u.Lo = decRes.Hi32;
				sdlTmp.u.Hi = 1;
				sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10);
				decRes.Hi32 = sdlTmp.u.Lo;

				sdlTmp.u.Lo = decRes.Mid32;
				sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10);
				decRes.Mid32 = sdlTmp.u.Lo;

				sdlTmp.u.Lo = decRes.Lo32;
				sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10);
				decRes.Lo32 = sdlTmp.u.Lo;

				// See if we need to round up.
				//
				if (sdlTmp.u.Hi >= 5 && (sdlTmp.u.Hi > 5 || (decRes.Lo32 & 1))) {
					DECIMAL_LO64_SET(decRes, DECIMAL_LO64_GET(decRes) + 1);
					if (DECIMAL_LO64_GET(decRes) == 0)
						decRes.Hi32++;
				}
			}
		}
	}
	else {
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
			pdecTmp = pdecR;
			pdecR = pdecL;
			pdecL = pdecTmp;
		}

		// *pdecL will need to be multiplied by 10^iScale so
		// it will have the same scale as *pdecR.  We could be
		// extending it to up to 192 bits of precision.
		//
		if (iScale <= POWER10_MAX) {
			// Scaling won't make it larger than 4 ULONGs
			//
			ulPwr = rgulPower10[iScale];
			DECIMAL_LO64_SET(decTmp, UInt32x32To64(pdecL->Lo32, ulPwr));
			sdlTmp.int64 = UInt32x32To64(pdecL->Mid32, ulPwr);
			sdlTmp.int64 += decTmp.Mid32;
			decTmp.Mid32 = sdlTmp.u.Lo;
			decTmp.Hi32 = sdlTmp.u.Hi;
			sdlTmp.int64 = UInt32x32To64(pdecL->Hi32, ulPwr);
			sdlTmp.int64 += decTmp.Hi32;
			if (sdlTmp.u.Hi == 0) {
				// Result fits in 96 bits.  Use standard aligned add.
				//
				decTmp.Hi32 = sdlTmp.u.Lo;
				pdecL = &decTmp;
				goto AlignedAdd;
			}
			rgulNum[0] = decTmp.Lo32;
			rgulNum[1] = decTmp.Mid32;
			rgulNum[2] = sdlTmp.u.Lo;
			rgulNum[3] = sdlTmp.u.Hi;
			iHiProd = 3;
		}
		else {
			// Have to scale by a bunch.  Move the number to a buffer
			// where it has room to grow as it's scaled.
			//
			rgulNum[0] = pdecL->Lo32;
			rgulNum[1] = pdecL->Mid32;
			rgulNum[2] = pdecL->Hi32;
			iHiProd = 2;

			// Scan for zeros in the upper words.
			//
			if (rgulNum[2] == 0) {
				iHiProd = 1;
				if (rgulNum[1] == 0) {
					iHiProd = 0;
					if (rgulNum[0] == 0) {
						// Left arg is zero, return right.
						//
						DECIMAL_LO64_SET(decRes, DECIMAL_LO64_GET(*pdecR));
						decRes.Hi32 = pdecR->Hi32;
						decRes.sign ^= bSign;
						goto RetDec;
					}
				}
			}

			// Scaling loop, up to 10^9 at a time.  iHiProd stays updated
			// with index of highest non-zero ULONG.
			//
			for (; iScale > 0; iScale -= POWER10_MAX) {
				if (iScale > POWER10_MAX)
					ulPwr = ulTenToNine;
				else
					ulPwr = rgulPower10[iScale];

				sdlTmp.u.Hi = 0;
				for (iCur = 0; iCur <= iHiProd; iCur++) {
					sdlTmp.int64 = UInt32x32To64(rgulNum[iCur], ulPwr) + sdlTmp.u.Hi;
					rgulNum[iCur] = sdlTmp.u.Lo;
				}

				if (sdlTmp.u.Hi != 0)
					// We're extending the result by another ULONG.
					rgulNum[++iHiProd] = sdlTmp.u.Hi;
			}
		}

		// Scaling complete, do the add.  Could be subtract if signs differ.
		//
		sdlTmp.u.Lo = rgulNum[0];
		sdlTmp.u.Hi = rgulNum[1];

		if (bSign) {
			// Signs differ, subtract.
			//
			DECIMAL_LO64_SET(decRes, sdlTmp.int64 - DECIMAL_LO64_GET(*pdecR));
			decRes.Hi32 = rgulNum[2] - pdecR->Hi32;

			// Propagate carry
			//
			if (DECIMAL_LO64_GET(decRes) > sdlTmp.int64) {
				decRes.Hi32--;
				if (decRes.Hi32 >= rgulNum[2])
					goto LongSub;
			}
			else if (decRes.Hi32 > rgulNum[2]) {
			LongSub:
				// If rgulNum has more than 96 bits of precision, then we need to
				// carry the subtraction into the higher bits.  If it doesn't,
				// then we subtracted in the wrong order and have to flip the 
				// sign of the result.
				// 
				if (iHiProd <= 2)
					goto SignFlip;

				iCur = 3;
				while (rgulNum[iCur++]-- == 0);
				if (rgulNum[iHiProd] == 0)
					iHiProd--;
			}
		}
		else {
			// Signs the same, add.
			//
			DECIMAL_LO64_SET(decRes, sdlTmp.int64 + DECIMAL_LO64_GET(*pdecR));
			decRes.Hi32 = rgulNum[2] + pdecR->Hi32;

			// Propagate carry
			//
			if (DECIMAL_LO64_GET(decRes) < sdlTmp.int64) {
				decRes.Hi32++;
				if (decRes.Hi32 <= rgulNum[2])
					goto LongAdd;
			}
			else if (decRes.Hi32 < rgulNum[2]) {
			LongAdd:
				// Had a carry above 96 bits.
				//
				iCur = 3;
				do {
					if (iHiProd < iCur) {
						rgulNum[iCur] = 1;
						iHiProd = iCur;
						break;
					}
				} while (++rgulNum[iCur++] == 0);
			}
		}

		if (iHiProd > 2) {
			rgulNum[0] = decRes.Lo32;
			rgulNum[1] = decRes.Mid32;
			rgulNum[2] = decRes.Hi32;
			decRes.scale = ScaleResult_CORECLR(rgulNum, iHiProd, decRes.scale);
			if (decRes.scale == (BYTE)-1)
				return DISP_E_OVERFLOW;

			decRes.Lo32 = rgulNum[0];
			decRes.Mid32 = rgulNum[1];
			decRes.Hi32 = rgulNum[2];
		}
	}

RetDec:
	COPYDEC(*pdecRes, decRes)
		return NOERROR;
}


//**********************************************************************
//
// VarDecDiv - Decimal Divide
//
//**********************************************************************

STDAPI VarDecDiv_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	ULONG   rgulQuo[3];
	ULONG   rgulQuoSave[3];
	ULONG   rgulRem[4];
	ULONG   rgulDivisor[3];
	ULONG   ulPwr;
	ULONG   ulTmp;
	ULONG   ulTmp1;
	SPLIT64 sdlTmp;
	SPLIT64 sdlDivisor;
	int     iScale;
	int     iCurScale;
	// not part of oleauto impl, only in decimal.cpp from classlibnative in corecrl
	// TODO: Determine if this it improves x64 impl or not
	// for original code the perf diff is -1% - +4% (64bit / 64bit where 64bit*64it -> 64bit)
	// most other scenarios has a small perf inpact from it
	BOOL    fUnscale;

	iScale = pdecL->scale - pdecR->scale;
	fUnscale = FALSE;
	rgulDivisor[0] = pdecR->Lo32;
	rgulDivisor[1] = pdecR->Mid32;
	rgulDivisor[2] = pdecR->Hi32;

	if (rgulDivisor[1] == 0 && rgulDivisor[2] == 0) {
		// Divisor is only 32 bits.  Easy divide.
		//
		if (rgulDivisor[0] == 0)
			return DISP_E_DIVBYZERO;

		rgulQuo[0] = pdecL->Lo32;
		rgulQuo[1] = pdecL->Mid32;
		rgulQuo[2] = pdecL->Hi32;
		rgulRem[0] = Div96By32(rgulQuo, rgulDivisor[0]);

		for (;;) {
			if (rgulRem[0] == 0) {
				if (iScale < 0) {
					iCurScale = min(9, -iScale);
					goto HaveScale;
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
			// If it doesn't cause overflow, we'll loop scaling by 10^9 and 
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
			iCurScale = SearchScale(rgulQuo[2], rgulQuo[1], iScale);
			if (iCurScale == 0) {
				// No more scaling to be done, but remainder is non-zero.
				// Round quotient.
				//
				ulTmp = rgulRem[0] << 1;
				if (ulTmp < rgulRem[0] || (ulTmp >= rgulDivisor[0] &&
					(ulTmp > rgulDivisor[0] || (rgulQuo[0] & 1)))) {
				RoundUp:
					if (++rgulQuo[0] == 0)
						if (++rgulQuo[1] == 0)
							rgulQuo[2]++;
				}
				break;
			}

			if (iCurScale == -1)
				return DISP_E_OVERFLOW;

		HaveScale:
			ulPwr = rgulPower10[iCurScale];
			iScale += iCurScale;

			if (IncreaseScale(rgulQuo, ulPwr) != 0)
				return DISP_E_OVERFLOW;

			sdlTmp.int64 = DivMod64by32(UInt32x32To64(rgulRem[0], ulPwr), rgulDivisor[0]);
			rgulRem[0] = sdlTmp.u.Hi;

			rgulQuo[0] += sdlTmp.u.Lo;
			if (rgulQuo[0] < sdlTmp.u.Lo) {
				if (++rgulQuo[1] == 0)
					rgulQuo[2]++;
			}
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

		iCurScale = 0;
		if (!(ulTmp & 0xFFFF0000)) {
			iCurScale += 16;
			ulTmp <<= 16;
		}
		if (!(ulTmp & 0xFF000000)) {
			iCurScale += 8;
			ulTmp <<= 8;
		}
		if (!(ulTmp & 0xF0000000)) {
			iCurScale += 4;
			ulTmp <<= 4;
		}
		if (!(ulTmp & 0xC0000000)) {
			iCurScale += 2;
			ulTmp <<= 2;
		}
		if (!(ulTmp & 0x80000000)) {
			iCurScale++;
			ulTmp <<= 1;
		}

		// Shift both dividend and divisor left by iCurScale.
		// 
		sdlTmp.int64 = DECIMAL_LO64_GET(*pdecL) << iCurScale;
		rgulRem[0] = sdlTmp.u.Lo;
		rgulRem[1] = sdlTmp.u.Hi;
		sdlTmp.u.Lo = pdecL->Mid32;
		sdlTmp.u.Hi = pdecL->Hi32;
		sdlTmp.int64 <<= iCurScale;
		rgulRem[2] = sdlTmp.u.Hi;
		rgulRem[3] = (pdecL->Hi32 >> (31 - iCurScale)) >> 1;

		sdlDivisor.u.Lo = rgulDivisor[0];
		sdlDivisor.u.Hi = rgulDivisor[1];
		sdlDivisor.int64 <<= iCurScale;

		if (rgulDivisor[2] == 0) {
			// Have a 64-bit divisor in sdlDivisor.  The remainder
			// (currently 96 bits spread over 4 ULONGs) will be < divisor.
			//
			sdlTmp.u.Lo = rgulRem[2];
			sdlTmp.u.Hi = rgulRem[3];

			rgulQuo[2] = 0;
			rgulQuo[1] = Div96By64(&rgulRem[1], sdlDivisor);
			rgulQuo[0] = Div96By64(rgulRem, sdlDivisor);

			for (;;) {
				if ((rgulRem[0] | rgulRem[1]) == 0) {
					if (iScale < 0) {
						iCurScale = min(9, -iScale);
						goto HaveScale64;
					}
					break;
				}

				// We need to unscale if and only if we have a non-zero remainder
				fUnscale = TRUE;

				// Remainder is non-zero.  Scale up quotient and remainder by 
				// powers of 10 so we can compute more significant bits.
				// 
				iCurScale = SearchScale(rgulQuo[2], rgulQuo[1], iScale);
				if (iCurScale == 0) {
					// No more scaling to be done, but remainder is non-zero.
					// Round quotient.
					//
					sdlTmp.u.Lo = rgulRem[0];
					sdlTmp.u.Hi = rgulRem[1];
					if (sdlTmp.u.Hi >= 0x80000000 || (sdlTmp.int64 <<= 1) > sdlDivisor.int64 ||
						(sdlTmp.int64 == sdlDivisor.int64 && (rgulQuo[0] & 1)))
						goto RoundUp;
					break;
				}

				if (iCurScale == -1)
					return DISP_E_OVERFLOW;

			HaveScale64:
				ulPwr = rgulPower10[iCurScale];
				iScale += iCurScale;

				if (IncreaseScale(rgulQuo, ulPwr) != 0)
					return DISP_E_OVERFLOW;

				rgulRem[2] = 0;  // rem is 64 bits, IncreaseScale uses 96
				IncreaseScale(rgulRem, ulPwr);
				ulTmp = Div96By64(rgulRem, sdlDivisor);
				rgulQuo[0] += ulTmp;
				if (rgulQuo[0] < ulTmp)
					if (++rgulQuo[1] == 0)
						rgulQuo[2]++;

			} // for (;;)
		}
		else {
			// Have a 96-bit divisor in rgulDivisor[].
			//
			// Start by finishing the shift left by iCurScale.
			//
			sdlTmp.u.Lo = rgulDivisor[1];
			sdlTmp.u.Hi = rgulDivisor[2];
			sdlTmp.int64 <<= iCurScale;
			rgulDivisor[0] = sdlDivisor.u.Lo;
			rgulDivisor[1] = sdlDivisor.u.Hi;
			rgulDivisor[2] = sdlTmp.u.Hi;

			// The remainder (currently 96 bits spread over 4 ULONGs) 
			// will be < divisor.
			// 
			rgulQuo[2] = 0;
			rgulQuo[1] = 0;
			rgulQuo[0] = Div128By96(rgulRem, rgulDivisor);

			for (;;) {
				if ((rgulRem[0] | rgulRem[1] | rgulRem[2]) == 0) {
					if (iScale < 0) {
						iCurScale = min(9, -iScale);
						goto HaveScale96;
					}
					break;
				}

				// We need to unscale if and only if we have a non-zero remainder
				fUnscale = TRUE;

				// Remainder is non-zero.  Scale up quotient and remainder by 
				// powers of 10 so we can compute more significant bits.
				// 
				iCurScale = SearchScale(rgulQuo[2], rgulQuo[1], iScale);
				if (iCurScale == 0) {
					// No more scaling to be done, but remainder is non-zero.
					// Round quotient.
					//
					if (rgulRem[2] >= 0x80000000)
						goto RoundUp;

					ulTmp = rgulRem[0] > 0x80000000;
					ulTmp1 = rgulRem[1] > 0x80000000;
					rgulRem[0] <<= 1;
					rgulRem[1] = (rgulRem[1] << 1) + ulTmp;
					rgulRem[2] = (rgulRem[2] << 1) + ulTmp1;

					if (rgulRem[2] > rgulDivisor[2] || (rgulRem[2] == rgulDivisor[2] &&
						(rgulRem[1] > rgulDivisor[1] || (rgulRem[1] == rgulDivisor[1] &&
						(rgulRem[0] > rgulDivisor[0] || (rgulRem[0] == rgulDivisor[0] &&
							(rgulQuo[0] & 1)))))))
						goto RoundUp;
					break;
				}

				if (iCurScale == -1)
					return DISP_E_OVERFLOW;

			HaveScale96:
				ulPwr = rgulPower10[iCurScale];
				iScale += iCurScale;

				if (IncreaseScale(rgulQuo, ulPwr) != 0)
					return DISP_E_OVERFLOW;

				rgulRem[3] = IncreaseScale(rgulRem, ulPwr);
				ulTmp = Div128By96(rgulRem, rgulDivisor);
				rgulQuo[0] += ulTmp;
				if (rgulQuo[0] < ulTmp)
					if (++rgulQuo[1] == 0)
						rgulQuo[2]++;

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
		while ((rgulQuo[0] & 0xFF) == 0 && iScale >= 8) {
			rgulQuoSave[0] = rgulQuo[0];
			rgulQuoSave[1] = rgulQuo[1];
			rgulQuoSave[2] = rgulQuo[2];

			if (Div96By32(rgulQuoSave, 100000000) == 0) {
				rgulQuo[0] = rgulQuoSave[0];
				rgulQuo[1] = rgulQuoSave[1];
				rgulQuo[2] = rgulQuoSave[2];
				iScale -= 8;
			}
			else
				break;
		}

		if ((rgulQuo[0] & 0xF) == 0 && iScale >= 4) {
			rgulQuoSave[0] = rgulQuo[0];
			rgulQuoSave[1] = rgulQuo[1];
			rgulQuoSave[2] = rgulQuo[2];

			if (Div96By32(rgulQuoSave, 10000) == 0) {
				rgulQuo[0] = rgulQuoSave[0];
				rgulQuo[1] = rgulQuoSave[1];
				rgulQuo[2] = rgulQuoSave[2];
				iScale -= 4;
			}
		}

		if ((rgulQuo[0] & 3) == 0 && iScale >= 2) {
			rgulQuoSave[0] = rgulQuo[0];
			rgulQuoSave[1] = rgulQuo[1];
			rgulQuoSave[2] = rgulQuo[2];

			if (Div96By32(rgulQuoSave, 100) == 0) {
				rgulQuo[0] = rgulQuoSave[0];
				rgulQuo[1] = rgulQuoSave[1];
				rgulQuo[2] = rgulQuoSave[2];
				iScale -= 2;
			}
		}

		if ((rgulQuo[0] & 1) == 0 && iScale >= 1) {
			rgulQuoSave[0] = rgulQuo[0];
			rgulQuoSave[1] = rgulQuo[1];
			rgulQuoSave[2] = rgulQuo[2];

			if (Div96By32(rgulQuoSave, 10) == 0) {
				rgulQuo[0] = rgulQuoSave[0];
				rgulQuo[1] = rgulQuoSave[1];
				rgulQuo[2] = rgulQuoSave[2];
				iScale -= 1;
			}
		}
	}

	pdecRes->Hi32 = rgulQuo[2];
	pdecRes->Mid32 = rgulQuo[1];
	pdecRes->Lo32 = rgulQuo[0];
	pdecRes->scale = iScale;
	pdecRes->sign = pdecL->sign ^ pdecR->sign;
	pdecRes->wReserved = 0; // not part of oleauto impl, only in decimal.cpp from classlibnative in corecrl 
	return NOERROR;
}

//**********************************************************************
//
// VarDecAbs - Decimal Absolute Value
//
//**********************************************************************

STDAPI VarDecAbs(LPDECIMAL pdecOprd, LPDECIMAL pdecRes)
{
	COPYDEC(*pdecRes, *pdecOprd)
		pdecRes->sign &= ~DECIMAL_NEG;
	return NOERROR;
}


//**********************************************************************
//
// VarDecFix - Decimal Fix (chop to integer)
//
//**********************************************************************

STDAPI VarDecFix(LPDECIMAL pdecOprd, LPDECIMAL pdecRes)
{
	DecFixInt(pdecRes, pdecOprd);
	return NOERROR;
}


//**********************************************************************
//
// VarDecInt - Decimal Int (round down to integer)
//
//**********************************************************************

STDAPI VarDecInt(LPDECIMAL pdecOprd, LPDECIMAL pdecRes)
{
	if (DecFixInt(pdecRes, pdecOprd) != 0 && (pdecRes->sign & DECIMAL_NEG)) {
		// We have chopped off a non-zero amount from a negative value.  Since
		// we round toward -infinity, we must increase the integer result by
		// 1 to make it more negative.  This will never overflow because
		// in order to have a remainder, we must have had a non-zero scale factor.
		// Our scale factor is back to zero now.
		//
		DECIMAL_LO64_SET(*pdecRes, DECIMAL_LO64_GET(*pdecRes) + 1);
		if (DECIMAL_LO64_GET(*pdecRes) == 0)
			pdecRes->Hi32++;
	}
	return NOERROR;
}


//**********************************************************************
//
// VarDecNeg - Decimal Negate
//
//**********************************************************************

STDAPI VarDecNeg(LPDECIMAL pdecOprd, LPDECIMAL pdecRes)
{
	COPYDEC(*pdecRes, *pdecOprd)
		pdecRes->sign ^= DECIMAL_NEG;
	return NOERROR;
}


//**********************************************************************
//
// VarDecCmp - Decimal Compare
//
//**********************************************************************

STDAPI VarDecCmp(LPDECIMAL pdecL, LPDECIMAL pdecR)
{
	ULONG   ulSgnL;
	ULONG   ulSgnR;

	// First check signs and whether either are zero.  If both are
	// non-zero and of the same sign, just use subtraction to compare.
	//
	ulSgnL = pdecL->Lo32 | pdecL->Mid32 | pdecL->Hi32;
	ulSgnR = pdecR->Lo32 | pdecR->Mid32 | pdecR->Hi32;
	if (ulSgnL != 0)
		ulSgnL = (pdecL->sign & DECIMAL_NEG) | 1;

	if (ulSgnR != 0)
		ulSgnR = (pdecR->sign & DECIMAL_NEG) | 1;

	// ulSgnL & ulSgnR have values 1, 0, or 0x81 depending on if the left/right
	// operand is +, 0, or -.
	//
	if (ulSgnL == ulSgnR) {
		if (ulSgnL == 0)    // both are zero
			return VARCMP_EQ; // return equal

		DECIMAL decRes;

		DecAddSub_PALRT(pdecL, pdecR, &decRes, DECIMAL_NEG);
		if (DECIMAL_LO64_GET(decRes) == 0 && decRes.Hi32 == 0)
			return VARCMP_EQ;
		if (decRes.sign & DECIMAL_NEG)
			return VARCMP_LT;
		return VARCMP_GT;
	}

	// Signs are different.  Used signed byte compares
	//
	if ((char)ulSgnL > (char)ulSgnR)
		return VARCMP_GT;
	return VARCMP_LT;
}

STDAPI VarDecRound(LPDECIMAL pdecIn, int cDecimals, LPDECIMAL pdecRes)
{
	ULONG   rgulNum[3];
	ULONG   ulRem;
	ULONG   ulSticky;
	ULONG   ulPwr;
	int	    iScale;

	if (cDecimals < 0)
		return E_INVALIDARG;

	iScale = pdecIn->scale - cDecimals;
	if (iScale > 0)
	{
		rgulNum[0] = pdecIn->Lo32;
		rgulNum[1] = pdecIn->Mid32;
		rgulNum[2] = pdecIn->Hi32;
		pdecRes->sign = pdecIn->sign;
		ulRem = ulSticky = 0;

		do {
			ulSticky |= ulRem;
			if (iScale > POWER10_MAX)
				ulPwr = ulTenToNine;
			else
				ulPwr = rgulPower10[iScale];

			ulRem = Div96By32(rgulNum, ulPwr);
			iScale -= 9;
		} while (iScale > 0);

		// Now round.  ulRem has last remainder, ulSticky has sticky bits.
		// To do IEEE rounding, we add LSB of result to sticky bits so
		// either causes round up if remainder * 2 == last divisor.
		//
		ulSticky |= rgulNum[0] & 1;
		ulRem = (ulRem << 1) + (ulSticky != 0);
		if (ulPwr < ulRem &&
			++rgulNum[0] == 0 &&
			++rgulNum[1] == 0
			)
			++rgulNum[2];

		pdecRes->Lo32 = rgulNum[0];
		pdecRes->Mid32 = rgulNum[1];
		pdecRes->Hi32 = rgulNum[2];
		pdecRes->scale = cDecimals;
		return NOERROR;
	}

	COPYDEC(*pdecRes, *pdecIn)
		return NOERROR;
}

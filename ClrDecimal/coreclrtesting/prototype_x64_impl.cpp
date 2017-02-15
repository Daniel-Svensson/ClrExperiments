#include "stdafx.h"

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
#define FullDiv128y32 _udiv128_v2;

#define DEC_SCALE_MAX   28
#define POWER10_MAX64 19
static const DWORD64 POWER10_MAX_VALUE64 = 10000000000000000000;
static const DWORD64 rgulPower10_64[POWER10_MAX64 + 1] = { 1, 10, 100, 1000, 10000, 100000, 1000000,
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



// Divides a 64bit ulong by 32bit, returns 32bit remainder
// This translates to a single div instruction on x64 platforms
DWORD64 FullDiv64By64(DWORD64 *pdlNum, DWORD64 ulDen)
{
	auto mod = *pdlNum % ulDen;
	auto res = *pdlNum / ulDen;

	*pdlNum = res;
	return mod;
}

// Divides a 64bit ulong by 32bit, returns 32bit remainder
// This translates to a single div instruction on x64 platforms
static DWORD32 FullDiv64By32_x64(DWORD64* pdlNum, DWORD32 ulDen)
{
	auto mod = DWORD32(*pdlNum % ulDen);
	auto res = *pdlNum / ulDen;

	*pdlNum = res;
	return mod;
}

// Divides a 96bit ulong by 32bit, returns 32bit remainder
static DWORD32 FullDiv96By32(DWORD32 *pdlNum, DWORD32 ulDen)
{
	// Upper 64bit
	DWORD64* hiPtr = (DWORD64*)(pdlNum + 1);
	DWORD64 lopart = ((DWORD64)FullDiv64By32_x64(hiPtr, ulDen) << 32) + *pdlNum;
	DWORD32 remainder = FullDiv64By32_x64(&lopart, ulDen);
	*pdlNum = (DWORD32)lopart;

	return remainder;
}


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
// Part of decimal.cpp
#define DEC_SCALE_MAX   28
#define POWER10_MAX     9

static ULONG rgulPower10[POWER10_MAX + 1] = { 1, 10, 100, 1000, 10000, 100000, 1000000,
10000000, 100000000, 1000000000 };
static const ULONG ulTenToNine = 1000000000U;

#define SCALERESULT_VERSION 3
#if SCALERESULT_VERSION == 1
int ScaleResult(ULONG *rgulRes, int iHiRes, int iScale)
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
	//iNewScale = iHiRes * 32 - 64 - 1;
	// if (iNewScale > 0) {
	if (iHiRes > 2) {
		BOOLEAN found = BitScanReverse(&ulTmp, rgulRes[iHiRes]);
		assert(found);
		// msb will be in rance [31,0]
		iNewScale = iHiRes * 32 - 64 - 1;
		iNewScale = iNewScale - (31 - ulTmp);

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
#elif SCALERESULT_VERSION == 3
int ScaleResult_x64(DWORD64 *rgulRes, int iHiRes, int iScale)
{
	LIMITED_METHOD_CONTRACT;

	int     iNewScale;
	int     iCur;
	DWORD   ulMsb;
	DWORD64 ulSticky;
	DWORD64 ulPwr;
	DWORD64 remainder;


	// See if we need to scale the result.  The combined scale must
	// be <= DEC_SCALE_MAX and the upper 96 bits must be zero.
	// 
	// Start by figuring a lower bound on the scaling needed to make
	// the upper 96 bits zero.  iHiRes is the index into rgulRes[]
	// of the highest non-zero ULONG.
	// 
	
	if (iHiRes > 1) {
		BOOLEAN found = BitScanReverse64(&ulMsb, rgulRes[iHiRes]);
		assert(found);
		// msb will be in rance [63,0]
		iNewScale = iHiRes * 64 - 64 - 1;
		iNewScale = iNewScale - (63 - ulMsb);

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
		remainder = 0;

		for (;;) {

			ulSticky |= remainder; // record remainder as sticky bit

			if (iNewScale > POWER10_MAX64)
				ulPwr = POWER10_MAX_VALUE64;
			else
				ulPwr = rgulPower10_64[iNewScale];

			// Compute first quotient.
			// DivMod64by32 returns quotient in Lo, remainder in Hi.
			//
			//rgulRes[iHiRes] = _udiv128(rgulRes[iHiRes], 0, ulPwr, &remainder);
			remainder = _udiv128_v2(&rgulRes[iHiRes], 0, ulPwr);
			iCur = iHiRes - 1;

			if (iCur >= 0) {
				// If first quotient was 0, update iHiRes.
				// TODO: See if this really increase perf
				if (rgulRes[iHiRes] == 0)
					iHiRes--;

				// Compute subsequent quotients.
				//
				do {
					//rgulRes[iCur] = _udiv128(rgulRes[iCur], remainder, ulPwr, &remainder);
					remainder = _udiv128_v2(&rgulRes[iCur], remainder, ulPwr);
					iCur--;
				} while (iCur >= 0);
			}

			// TODO: / Remarks we can subtract with actual scale, but then we need to store it 
			iNewScale -= POWER10_MAX64;
			if (iNewScale > 0)
				continue; // scale some more

						  // If we scaled enough, iHiRes would be 2 or less.  If not,
						  // divide by 10 more.
						  //
			if (iCur > 1 || (iCur == 1 && (rgulRes[iCur] & 0xffffffff00000000))) {
				iNewScale = 1;
				iScale--;
				continue; // scale by 10
			}

			// Round final result.  See if remainder >= 1/2 of divisor.
			// If remainder == 1/2 divisor, round up if odd or sticky bit set.
			ulPwr >>= 1;  // power of 10 always even
			if (ulPwr < remainder || (ulPwr == remainder && ((rgulRes[0] & 1) | ulSticky))) {
				iCur = -1;
				while (++rgulRes[++iCur] == 0);

				if (iCur > 1 || (iCur == 1 && (rgulRes[iCur] & 0xffffffff00000000))) {
					// The rounding caused us to carry beyond 96 bits. 
					// Scale by 10 more.
					//
					iHiRes = iCur;
					ulSticky = 0;  // no sticky bit
					remainder = 0; // or remainder
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
#endif

__declspec(noinline) STDAPI VarDecMul_x64(DECIMAL* pdecL, DECIMAL *pdecR, DECIMAL *res)
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
		//if ((hi & 0xffffffff00000000) == 0)
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

				// iScale <= 19 = POWER10_MAX64
				ullPwr = rgulPower10_64[iScale];
				ullRem = FullDiv64By64(&lo, ullPwr);

				// Round result.  See if remainder >= 1/2 of divisor.
				// Divisor is a power of 10, so it is always even.
				ullPwr >>= 1;

				// round towards even
				if (ullRem > ullPwr || (ullRem == ullPwr && (lo & 1)))
					lo++;

				iScale = DEC_SCALE_MAX;
			}

			res->Hi32 = 0;
			res->Lo64 = lo;
		}
		else
		{
#if SCALERESULT_VERSION < 3
			// Bits in upper 64 bit
			// 
			// CONVERT to old representation
			ULONG tmpSpace[4];
			((DWORD64*)&tmpSpace)[0] = lo;
			((DWORD64*)&tmpSpace)[1] = hi;
			//tmpSpace[0] = ((SPLIT64*)&lo)->u.Lo;
			//tmpSpace[1] = ((SPLIT64*)&lo)->u.Hi;
			//tmpSpace[2] = ((SPLIT64*)&hi)->u.Lo;
			//tmpSpace[3] = ((SPLIT64*)&hi)->u.Hi;

			// Check for leading zero ULONGs on the product
			//
			int iHiProd = 3;
			while (tmpSpace[iHiProd] == 0) {
				iHiProd--;
				if (iHiProd < 0)
					goto ReturnZero;
			}
			iScale = ScaleResult(tmpSpace, iHiProd, iScale);
#else
			DWORD64 tmpSpace[2];
			tmpSpace[0] = lo;
			tmpSpace[1] = hi;
			iScale = ScaleResult_x64(tmpSpace, 1, iScale);
#endif
			if (iScale == -1)
				return DISP_E_OVERFLOW;

#if SCALERESULT_VERSION < 3
			res->Lo32 = tmpSpace[0];
			res->Mid32 = tmpSpace[1];
			res->Hi32 = tmpSpace[2];
#else
			res->Lo64 = tmpSpace[0];
			res->Hi32 = (DWORD32)tmpSpace[1];
#endif
		}
	}
	else
	{
		// TODO: Testwithout tmpSum variable
		DWORD64 tmpSum;
		DWORD64 tmpLo2;
		DWORD64 tmpHi2;
		DWORD64 rgulProd[3];

		// At least one operand has bits set in the upper 64 bits.
		//
		// Compute and accumulate the 9 partial products into a 
		// 192-bit (24-byte) result.
		//
		//                [l-h][l-lo]   left high32, lo64
		//             x  [r-h][r-lo]   right high32, lo64
		// ------------------------------
		//
		//                [0-h][0-l]   l-lo * r-lo
		//           [1ah][1al]        l-lo * r-hi
		//           [1bh][1bl]        l-hi * r-lo
		//      [2ah][2al]             l-hi * r-hi
		// ------------------------------
		//      [p-3][p-2][p-1][p-0]   prod[] array

		// TODO: must do 4 crosswise mult
		rgulProd[0] = _umul128(pdecL->Lo64, pdecR->Lo64, &tmpSum);

		// Both will generate a 96 bit results
		lo = _umul128(pdecL->Lo64, (DWORD64)pdecR->Hi32, &hi);
		char carry1 = _addcarry_u64(0, lo, tmpSum, &tmpSum);

		tmpLo2 = _umul128((DWORD64)pdecL->Hi32, pdecR->Lo64, &tmpHi2);
		char carry2 = _addcarry_u64(0, tmpLo2, tmpSum, &tmpSum);
	
		rgulProd[1] = tmpSum;

		// We can add 2 32bit numers to a 32bit*32bit result withaout overflow / carry
		//  The maximum value of the "hi" result of the middle products are MAXDWORD32-1 each
		//  so adding their carry to the addition will only result in a 32bit value at most MAXDWORD32
		//  so this will never generate a carry.		
		rgulProd[2] = ((DWORD64)pdecL->Hi32 * (DWORD64)pdecR->Hi32);
		_addcarry_u64(carry1, hi, rgulProd[2], &rgulProd[2]);
		_addcarry_u64(carry2, tmpHi2, rgulProd[2], &rgulProd[2]);

#if SCALERESULT_VERSION < 2
		// CONVERT to old representation
		ULONG* tmpSpace = (ULONG*)&rgulProd[0];
		// Check for leading zero ULONGs on the product
		//
		int iHiProd = 5;
		while (tmpSpace[iHiProd] == 0) {
			iHiProd--;
			if (iHiProd < 0)
				goto ReturnZero;
		}
		iScale = ScaleResult(tmpSpace, iHiProd, iScale);
#else
		// Check for leading zero ULONGs on the product
		//
		int iHiProd = 2;
		while (rgulProd[iHiProd] == 0) {
			iHiProd--;
			if (iHiProd < 0)
				goto ReturnZero;
		}
		iScale = ScaleResult_x64(rgulProd, iHiProd, iScale);
#endif
		if (iScale == -1)
			return DISP_E_OVERFLOW;

#if SCALERESULT_VERSION < 2
		res->Lo64 = rgulProd[0];
		res->Hi32 = tmpSpace[2];
#else
		res->Lo64 = rgulProd[0];
		res->Hi32 = (DWORD32)rgulProd[1];
#endif
	}

	res->sign = pdecL->sign ^ pdecR->sign;
	res->scale = (BYTE)iScale;
	return NOERROR;
}
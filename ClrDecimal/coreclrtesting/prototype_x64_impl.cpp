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
static const DWORD64 rgulPower10_64[POWER10_MAX64 + 1] = {
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

// ScaleResult is called from AddSub as well as multiply
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
	BOOLEAN found = BitScanReverse64(&ulMsb, rgulRes[iHiRes]);
	assert(found);
	// msb will be in rance [63,0]
	//iNewScale = iHiRes * 64 - 64 - 1;
	//iNewScale = iNewScale - (63 - ulMsb);

	iNewScale = iHiRes * 64 + ulMsb - 96;

	if (iNewScale >= 0) {
		// actual MSB = iHiRes*64 + ulMsb
		// iNewScale = iHiRes * 64 - 64 - 1 - 63 + ulMsb = MSB - 128

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

			if (iNewScale < POWER10_MAX64) // TODO: inkludera POWER10_MAX64 eller inte ???
				ulPwr = rgulPower10_64[iNewScale];
			else
				ulPwr = POWER10_MAX_VALUE64;

			// Compute first quotient.
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

			iNewScale -= POWER10_MAX64;
			if (iNewScale > 0)
				continue; // scale some more

			// If we scaled enough, iHiRes would be 0 or 1 without anythin in upper 32bits.  If not,
			// divide by 10 more.
			//
			if (iHiRes > 1 || (iHiRes == 1 && (((SPLIT64*)&rgulRes[iHiRes])->u.Hi != 0))) {
				iNewScale = 1;
				iScale--;
				continue; // scale by 10
			}

			// Round final result.  See if remainder >= 1/2 of divisor.
			// If remainder == 1/2 divisor, round up if odd or sticky bit set.
			ulPwr >>= 1;  // power of 10 always even
			if (remainder > ulPwr || (ulPwr == remainder && ((rgulRes[0] & 1) | ulSticky))) {
				// if (remainder >= ulPwr && (ulPwr > remainder || ((rgulRes[0] & 1) | ulSticky))) {
				iCur = -1;
				while (++rgulRes[++iCur] == 0);

				if (iCur > 1 || (iCur == 1 && (((SPLIT64*)&rgulRes[iCur])->u.Hi != 0))) {
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


STDAPI VarDecMul_x64(DECIMAL* pdecL, DECIMAL *pdecR, DECIMAL *res)
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
		unsigned char carry1 = _addcarry_u64(0, lo, tmpSum, &tmpSum);

		tmpLo2 = _umul128((DWORD64)pdecL->Hi32, pdecR->Lo64, &tmpHi2);
		unsigned char carry2 = _addcarry_u64(0, tmpLo2, tmpSum, &tmpSum);

		rgulProd[1] = tmpSum;

		// We can add 2 32bit numers to a 32bit*32bit result withaout overflow / carry
		//  The maximum value of the "hi" result of the middle products are MAXDWORD32-1 each
		//  so adding their carry to the addition will only result in a 32bit value at most MAXDWORD32
		//  so this will never generate a carry.		
		rgulProd[2] = ((DWORD64)pdecL->Hi32 * (DWORD64)pdecR->Hi32);
		_addcarry_u64(carry1, hi, rgulProd[2], &rgulProd[2]);
		_addcarry_u64(carry2, tmpHi2, rgulProd[2], &rgulProd[2]);

		// Check for leading zero ULONGs on the product
		//
		int iHiProd = 2;
		while (rgulProd[iHiProd] == 0) {
			iHiProd--;
			if (iHiProd < 0)
				goto ReturnZero;
		}
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


static HRESULT DecAddSub_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes, char bSign);

STDAPI VarDecAdd_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	return DecAddSub_x64(pdecL, pdecR, pdecRes, 0);
}


STDAPI VarDecSub_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
{
	return DecAddSub_x64(pdecL, pdecR, pdecRes, DECIMAL_NEG);
}
#define DECIMAL_LO64_SET(dec, value) ((dec).Lo64 = (value));
#define DECIMAL_LO64_GET(dec, value) ((dec).Lo64)
#define DECIMAL_HI32(dec, value) (dec).Hi32
inline void COPYDEC(DECIMAL &to, const DECIMAL &from)
{
	(to).Hi32 = (from).Hi32; 
	(to).Lo64 = (from).Lo64; 
	(to).signscale = (from).signscale;
}


static HRESULT DecAddSub_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes, char bSign)
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
					DECIMAL_LO64_SET(decRes, DECIMAL_LO64_GET(decRes) + 1)
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
			{
				// Zero out upper 32bit of 64 bit value if required
				if ((iHiProd & 1) == 0)
				{
					rgulNum[iHiProd + 1] = 0;
				}
			}
			decRes.scale = ScaleResult_x64((DWORD64*)rgulNum, iHiProd / 2, decRes.scale);
			if (decRes.scale == (BYTE)-1)
				return DISP_E_OVERFLOW;

			decRes.Lo32 = rgulNum[0];
			decRes.Mid32 = rgulNum[1];
			decRes.Hi32 = rgulNum[2];
		}
	}

RetDec:
	COPYDEC(*pdecRes, decRes);
	return NOERROR;
}

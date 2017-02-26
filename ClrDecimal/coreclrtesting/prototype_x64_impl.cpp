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

// Divides a 64bit ulong by 64bit, returns 64bit remainder
// This translates to a single div instruction on x64 platforms
static DWORD64 FullDiv64By64(DWORD64 *pdlNum, DWORD64 ulDen)
{
	auto mod = *pdlNum % ulDen;
	auto res = *pdlNum / ulDen;

	*pdlNum = res;
	return mod;
}

static inline DWORD32 FullDiv64By32(DWORD64 pdlNum, DWORD32 ulDen, DWORD64* pRemainder)
{
	auto mod = pdlNum % ulDen;
	auto res = pdlNum / ulDen;

	*pRemainder = mod;
	return (DWORD32)res;
}

// Divides a 64bit ulong by 32bit, returns 32bit remainder
// This translates to a single div instruction on x64 platforms
static inline DWORD32 FullDiv64By32_x64(DWORD64* pdlNum, DWORD32 ulDen)
{
	auto mod = DWORD32(*pdlNum % ulDen);
	auto res = *pdlNum / ulDen;

	*pdlNum = res;
	return mod;
}


// Divides a 96bit ulong by 32bit, returns 32bit remainder
static DWORD32 Div96By32_x64(ULONG *pdlNum, DWORD32 ulDen)
{
	// Upper 64bit
	DWORD64* hiPtr = (DWORD64*)(pdlNum + 1);
	DWORD64 lopart = (FullDiv64By64(hiPtr, ulDen) << 32) + *pdlNum;
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

/***
* ScaleResult
*
* Entry:
*   rgulRes - Array of DWORD64 with value, least-significant first.
*   iHiRes  - Index of last non-zero value in rgulRes, Max 2.
*   iScale  - Scale factor for this value, range 0 - 2 * DEC_SCALE_MAX
*
* Purpose:
*   See if we need to scale the result to fit it in 96 bits.
*   Perform needed scaling.  Adjust scale factor accordingly.
*
* Exit:
*   rgulRes updated in place, all items with index <= iHiRes are updated.
*   New scale factor returned, -1 if overflow error.
*
***********************************************************************/
// ScaleResult is called from AddSub as well as multiply
int ScaleResult_x64(DWORD64 *rgullRes, int iHiRes, int iScale)
{
	LIMITED_METHOD_CONTRACT;

	int     iNewScale;
	int     iCur;
	DWORD   ulMsb;
	DWORD64 ullSticky;
	DWORD64 ullPwr;
	DWORD64 remainder;

	// See if we need to scale the result.  The combined scale must
	// be <= DEC_SCALE_MAX and the upper 96 bits must be zero.
	// 
	// Start by figuring a lower bound on the scaling needed to make
	// all but the lower 96 bits zero.  iHiRes is the index into rgulRes[]
	// of the highest non-zero element.
	// 
	BOOLEAN found = BitScanReverse64(&ulMsb, rgullRes[iHiRes]);
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
		assert(iHiRes >= 1);

		// Scale by the power of 10 given by iNewScale.  Note that this is 
		// NOT guaranteed to bring the number within 96 bits -- it could 
		// be 1 power of 10 short.
		//
		iScale -= iNewScale;
		ullSticky = 0;
		remainder = 0;

		for (;;) {

			ullSticky |= remainder; // record remainder as sticky bit

			if (iNewScale < POWER10_MAX64)
				ullPwr = rgulPower10_64[iNewScale];
			else
				ullPwr = POWER10_MAX_VALUE64;

			// Compute first quotient.
			//
			remainder = _udiv128_v2(&rgullRes[iHiRes], 0, ullPwr);
			iCur = iHiRes - 1;

			if (iCur >= 0) {
				// If first quotient was 0, update iHiRes.
				if (rgullRes[iHiRes] == 0)
					iHiRes--;

				// Compute subsequent quotients.
				//
				do {
					remainder = _udiv128_v2(&rgullRes[iCur], remainder, ullPwr);
					iCur--;
				} while (iCur >= 0);
			}

			iNewScale -= POWER10_MAX64;
			if (iNewScale > 0)
				continue; // scale some more

			// If we scaled enough, iHiRes would be 0 or 1 without anything above first 96bits.  If not,
			// divide by 10 more.
			//
			if (iHiRes > 1 || (((SPLIT64*)&rgullRes[1])->u.Hi != 0)) {
				iNewScale = 1;
				iScale--;
				continue; // scale by 10
			}

			// Round final result.  See if remainder >= 1/2 of divisor.
			// If remainder == 1/2 divisor, round up if odd or sticky bit set.
			//
			ullPwr >>= 1;  // power of 10 always even
			if (remainder > ullPwr || (ullPwr == remainder && ((rgullRes[0] & 1) | ullSticky))) {
				// if (remainder >= ulPwr && (ulPwr > remainder || ((rgulRes[0] & 1) | ulSticky))) {

				// Add 1 to first 96 bit word and check for overflow.
				// We only scale if iHiRes was originally >= 1 so rgulRes[1] is already initalized.
				// 
				auto carry = _addcarry_u64(0, rgullRes[0], 1, &rgullRes[0]);
				carry = _addcarry_u32(carry, (DWORD32)rgullRes[1], 0, (DWORD32*)&rgullRes[1]);

				if (carry != 0) {
					// The rounding caused us to carry beyond 96 bits. 
					// Scale by 10 more.
					// We know that (SPLIT64*)&rgulRes[1])->u.Hi == 0 before rounding up
					// so adding the carry results in 1.
					//
					assert(iHiRes == 1);
					((SPLIT64*)&rgullRes[1])->u.Hi = 1;
					ullSticky = 0;  // no sticky bit
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
		_addcarry_u64(carry2, hi, rgulProd[2], &rgulProd[2]);
		_addcarry_u64(carry1, tmpHi2, rgulProd[2], &rgulProd[2]);

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

// Add one to the decimal, returns carry
static inline unsigned char INC96(DECIMAL *pDec)
{
	auto carry = _addcarry_u64(0, pDec->Lo64, 1, &pDec->Lo64);
	return _addcarry_u32(carry, pDec->Hi32, 0, (DWORD32*)&pDec->Hi32);
}

static HRESULT DecAddSub_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes, char bSign)
{
	DECIMAL   decTmp;
	DECIMAL   decRes;
	DWORD64   rgulNum[3];
#if DEBUG
	rgulNum[0] = rgulNum[1] = rgulNum[2] = 0;
#endif

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
			carry = _subborrow_u32(carry, pdecL->Hi32, pdecR->Hi32,(unsigned int*)&decRes.Hi32);

			// Propagate carry
			//
			if (carry != 0) {
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
				DWORD64 remainder;
				decRes.Hi32 = FullDiv64By32((DWORD64)decRes.Hi32 + (1Ui64 << 32), 10, &remainder);
				//decRes.Lo64 = _udiv128(decRes.Lo64, remainder, 10, &remainder);
				remainder = _udiv128_v2(&decRes.Lo64, remainder, 10);

				// See if we need to round up.
				//
				if (remainder >= 5 && (remainder > 5 || (decRes.Lo32 & 1))) {
					// Add one, will never overflow since we divided by 10
					INC96(&decRes);
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
		if (iScale <= POWER10_MAX) {
			// Scaling won't make it larger than 96 + log2(10^9) < 126 bits so it will 
			// fit in 128 bits (2*DWORD64)
			//
			ullPwr = rgulPower10[iScale];

			DWORD64 hi;
			// hi is at most 32bit so we can add Hi32 without any risk for overflow
			rgulNum[0] = _umul128(pdecL->Lo64, ullPwr, &hi);
			rgulNum[1] = UInt32x32To64(pdecL->Hi32, ullPwr) + hi;
			if (((SPLIT64*)&rgulNum[1])->u.Hi == 0) {
				// Result fits in 96 bits.  Use standard aligned add.
				//
				decTmp.Lo64 = rgulNum[0];
				decTmp.Hi32 = (DWORD32)rgulNum[1]; // ((SPLIT64*)&rgulNum[1])->u.Lo
				pdecL = &decTmp;
				goto AlignedAdd;
			}
			iHiProd = 1;
		}
		else {
			// Have to scale by a bunch.  Move the number to a buffer
			// where it has room to grow as it's scaled.
			//
			rgulNum[0] = pdecL->Lo64;
			rgulNum[1] = pdecL->Hi32;
			// TODO: rgulNum[2] = 0 to simplify rest of the logic
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

				// TODO: Try loop unrolling for 2 or 3
				DWORD64 mul_carry = 0;
				unsigned char add_carry = 0;
				for (int iCur = 0; iCur <= iHiProd; iCur++) {
					DWORD64 tmp = mul_carry;
					DWORD64 product = _umul128(ullPwr, rgulNum[iCur], &mul_carry);
					add_carry = _addcarry_u64(add_carry, tmp, product, &rgulNum[iCur]);
				}

				// We're extending the result by another element.
				// Hi is at least 1 away from it's max value so we can add carry without overflow.
				// ex: 0xffff*0xffff => "fffe0001", and it is the same pattern for all bitlenghts
				//
				if (mul_carry != 0 || add_carry != 0)
					_addcarry_u64(add_carry, mul_carry, 0, &rgulNum[++iHiProd]);
			}
		}

		// Scaling complete, do the add.  Could be subtract if signs differ.
		//
		if (bSign) {
			// Signs differ, subtract.
			//
			auto carry = _subborrow_u64(0, rgulNum[0], pdecR->Lo64, &decRes.Lo64);
			// TODO: Handle special case where overflowing 96bits
			// 1. There are high bits to use => continue subtraction, bit SignFlip cannot be used
			// 2. There are high bits to use (left is only 96 bits) => if we get carry beyond 32bit then we can use SignFlip
			//   (left is only 96 bits) => decTmp.Mid32 == 0xffffffff && iHiProd <= 1 (carry == 1)
			carry = _subborrow_u64(carry, rgulNum[1], pdecR->Hi32, &decTmp.Lo64);
			rgulNum[1] = decTmp.Lo64;
			decRes.Hi32 = (DWORD32)decTmp.Lo32;

			// Propagate carry
			//
			if (carry != 0) {
				// If rgulNum has more than 96 bits of precision, then we need to
				// carry the subtraction into the higher bits.  If it doesn't,
				// then we subtracted in the wrong order and have to flip the 
				// sign of the result.
				// 

				if (iHiProd <= 1 && decTmp.Mid32 == 0xffffffff)
					goto SignFlip; // Result placed already placed in decRes 

				// > 96 bits, TODO: CHECK INTO THIS, make sure tests covers this area, 
				// iCur should never be greater than 2 so se if we can remove loop				
				if (iHiProd == 1) {
					iHiProd = 2;
					rgulNum[2] = MAXDWORD64;
				}
				else {
					--rgulNum[2];
				}

				if (rgulNum[iHiProd] == 0)
					iHiProd--;
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
				// then set it as 1 otherwise increase. There is not risk
				// for overflow
				// 
				if (iHiProd < 2) {
					rgulNum[2] = 1;
					iHiProd = 2;
				}
				else  {
					++rgulNum[2];
				}
			}
		}

		// decRes contains the lower 96 bits of the result
		// but at the same time the complete result apart from first element (rgulNum[0])
		// is in rgulNum[1..2]. 
		assert(decRes.Hi32 == ((SPLIT64*)&rgulNum[1])->u.Lo);

		// TODO: consider refactoring if statement as "ResultFitsIn96Bit" or similar
		if (iHiProd > 1 || (iHiProd == 1 && ((SPLIT64*)&rgulNum[1])->u.Hi != 0)) {
			rgulNum[0] = decRes.Lo64;
			
			decRes.scale = (BYTE)ScaleResult_x64(rgulNum, iHiProd, decRes.scale);
			if (decRes.scale == (BYTE)-1)
				return DISP_E_OVERFLOW;

			decRes.Lo64 = rgulNum[0];
			decRes.Hi32 = (DWORD32)rgulNum[1];
			assert(0 == ((SPLIT64*)&rgulNum[1])->u.Hi);
		}
	}

RetDec:
	COPYDEC(*pdecRes, decRes);
	return NOERROR;
}
// ********************** DIVIDE **************************************
// Divides a 96bit ulong by 32bit, returns 32bit remainder


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


//**********************************************************************
//
// VarDecDiv - Decimal Divide
//
//**********************************************************************


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

ULONG IncreaseScale(ULONG *rgulNum, ULONG ulPwr);
ULONG IncreaseScale_x64(ULONG *rgulNum, ULONG ulPwr)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64   sdlTmp;

	sdlTmp.int64 = UInt32x32To64(rgulNum[0], ulPwr);
	rgulNum[0] = sdlTmp.u.Lo;
	sdlTmp.int64 = UInt32x32To64(rgulNum[1], ulPwr) + sdlTmp.u.Hi;
	rgulNum[1] = sdlTmp.u.Lo;
	sdlTmp.int64 = UInt32x32To64(rgulNum[2], ulPwr) + sdlTmp.u.Hi;
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
*   Determine the max power of 10, <= 9, that the quotient can be scaled
*   up by and still fit in 96 bits.
*
* Exit:
*   Returns power of 10 to scale by, -1 if overflow error.
*
***********************************************************************/

int SearchScale(ULONG ulResHi, ULONG ulResLo, int iScale); // coreclr_impl
/*int SearchScale(ULONG ulResHi, ULONG ulResLo, int iScale)
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
}*/


/***
* Div128By96
*
* Entry:
*   rgulNum - Pointer to 128-bit dividend as array of ULONGs, least-sig first
*   rgulDen - Pointer to 96-bit divisor.
*
* Purpose:
*   Do partial divide, yielding 32-bit result and 96-bit remainder.
*   Top divisor ULONG must be larger than top dividend ULONG.  This is
*   assured in the initial call because the divisor is normalized
*   and the dividend can't be.  In subsequent calls, the remainder
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
ULONG Div128By96(ULONG *rgulNum, ULONG *rgulDen);
ULONG Div128By96_x64(ULONG *rgulNum, ULONG *rgulDen)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64 sdlQuo;
	SPLIT64 sdlNum;
	SPLIT64 sdlProd1;
	SPLIT64 sdlProd2;

	sdlNum.u.Lo = rgulNum[0];
	sdlNum.u.Hi = rgulNum[1];

	if (rgulNum[3] == 0 && rgulNum[2] < rgulDen[2])
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;

	// DivMod64by32 returns quotient in Lo, remainder in Hi.
	//
	sdlQuo.u.Lo = rgulNum[2];
	sdlQuo.u.Hi = rgulNum[3];
	sdlQuo.int64 = DivMod64by32(sdlQuo.int64, rgulDen[2]);

	// Compute full remainder, rem = dividend - (quo * divisor).
	//
	sdlProd1.int64 = UInt32x32To64(sdlQuo.u.Lo, rgulDen[0]); // quo * lo divisor
	sdlProd2.int64 = UInt32x32To64(sdlQuo.u.Lo, rgulDen[1]); // quo * mid divisor
	sdlProd2.int64 += sdlProd1.u.Hi;
	sdlProd1.u.Hi = sdlProd2.u.Lo;

	sdlNum.int64 -= sdlProd1.int64;
	rgulNum[2] = sdlQuo.u.Hi - sdlProd2.u.Hi; // sdlQuo.Hi is remainder

											  // Propagate carries
											  //
	if (sdlNum.int64 > ~sdlProd1.int64) {
		rgulNum[2]--;
		if (rgulNum[2] >= ~sdlProd2.u.Hi)
			goto NegRem;
	}
	else if (rgulNum[2] > ~sdlProd2.u.Hi) {
	NegRem:
		// Remainder went negative.  Add divisor back in until it's positive,
		// a max of 2 times.
		//
		sdlProd1.u.Lo = rgulDen[0];
		sdlProd1.u.Hi = rgulDen[1];

		for (;;) {
			sdlQuo.u.Lo--;
			sdlNum.int64 += sdlProd1.int64;
			rgulNum[2] += rgulDen[2];

			if (sdlNum.int64 < sdlProd1.int64) {
				// Detected carry. Check for carry out of top
				// before adding it in.
				//
				if (rgulNum[2]++ < rgulDen[2])
					break;
			}
			if (rgulNum[2] < rgulDen[2])
				break; // detected carry
		}
	}

	rgulNum[0] = sdlNum.u.Lo;
	rgulNum[1] = sdlNum.u.Hi;
	return sdlQuo.u.Lo;
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

//ULONG Div96By64(ULONG *rgulNum, SPLIT64 sdlDen); // decimal.cpp
ULONG Div96By64_x64(ULONG *rgulNum, SPLIT64 sdlDen)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64 sdlQuo;
	SPLIT64 sdlNum;
	SPLIT64 sdlProd;

	if (rgulNum[2] >= sdlDen.u.Hi) {
		// Divide would overflow.  Assume a quotient of 2^32, and set
		// up remainder accordingly.  Then jump to loop which reduces
		// the quotient.
		//
		sdlNum.u.Hi = rgulNum[1] - sdlDen.u.Lo;
		sdlNum.u.Lo = rgulNum[0];
		sdlQuo.u.Lo = 0;
		goto NegRem;
	}

	// Hardware divide won't overflow
	//
	if (rgulNum[2] == 0 && rgulNum[1] < sdlDen.u.Hi)
	{
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;
	}
/*
	DWORD64* rgullNum = (DWORD64*)rgulNum;
	DWORD64 quotient = _udiv128(rgullNum[0], rgulNum[2], sdlDen.int64, &rgullNum[0]);
	assert(((SPLIT64*)&quotient)->u.Hi == 0);
	return (DWORD32)quotient;
*/
	sdlNum.u.Lo = rgulNum[0];
	// DivMod64by32 returns quotient in Lo, remainder in Hi.
	//
	sdlQuo.u.Lo = rgulNum[1];
	sdlQuo.u.Hi = rgulNum[2];
	sdlQuo.int64 = DivMod64by32(sdlQuo.int64, sdlDen.u.Hi);
	sdlNum.u.Hi = sdlQuo.u.Hi; // remainder

							   // Compute full remainder, rem = dividend - (quo * divisor).
							   //
	sdlProd.int64 = UInt32x32To64(sdlQuo.u.Lo, sdlDen.u.Lo); // quo * lo divisor
	sdlNum.int64 -= sdlProd.int64;

	if (sdlNum.int64 > ~sdlProd.int64) {
	NegRem:
		// Remainder went negative.  Add divisor back in until it's positive,
		// a max of 2 times.
		//
		do {
			sdlQuo.u.Lo--;
			sdlNum.int64 += sdlDen.int64;
		} while (sdlNum.int64 >= sdlDen.int64);
	}

	rgulNum[0] = sdlNum.u.Lo;
	rgulNum[1] = sdlNum.u.Hi;
	return sdlQuo.u.Lo;
}

STDAPI VarDecDiv_x64(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes)
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
	// for original code the perf diff is -1% to +3% (where later is in case 64bit / 64bit where 64bit*64it -> 64bit)
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
		rgulRem[0] = Div96By32_x64(rgulQuo, rgulDivisor[0]);

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

		DWORD msb;
		BOOL found = BitScanReverse(&msb, ulTmp);
		iCurScale = 31 - msb;
		assert(found);

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
			rgulQuo[1] = Div96By64_x64(&rgulRem[1], sdlDivisor);
			rgulQuo[0] = Div96By64_x64(rgulRem, sdlDivisor);

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
				ulTmp = Div96By64_x64(rgulRem, sdlDivisor);
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

			if (Div96By32_x64(rgulQuoSave, 100000000) == 0) {
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

			if (Div96By32_x64(rgulQuoSave, 10000) == 0) {
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

			if (Div96By32_x64(rgulQuoSave, 100) == 0) {
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

			if (Div96By32_x64(rgulQuoSave, 10) == 0) {
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


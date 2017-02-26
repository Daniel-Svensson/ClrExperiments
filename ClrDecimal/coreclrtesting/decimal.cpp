#include "stdafx.h"

/***
* Div96By32
*
* Entry:
*   rgulNum - Pointer to 96-bit dividend as array of ULONGs, least-sig first
*   ulDen   - 32-bit divisor.
*
* Purpose:
*   Do full divide, yielding 96-bit result and 32-bit remainder.
*
* Exit:
*   Quotient overwrites dividend.
*   Returns remainder.
*
* Exceptions:
*   None.
*
***********************************************************************/

ULONG Div96By32(ULONG *rgulNum, ULONG ulDen)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64  sdlTmp;

	sdlTmp.u.Hi = 0;

	if (rgulNum[2] != 0)
		goto Div3Word;

	if (rgulNum[1] >= ulDen)
		goto Div2Word;

	sdlTmp.u.Hi = rgulNum[1];
	rgulNum[1] = 0;
	goto Div1Word;

Div3Word:
	sdlTmp.u.Lo = rgulNum[2];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulDen);
	rgulNum[2] = sdlTmp.u.Lo;
Div2Word:
	sdlTmp.u.Lo = rgulNum[1];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulDen);
	rgulNum[1] = sdlTmp.u.Lo;
Div1Word:
	sdlTmp.u.Lo = rgulNum[0];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulDen);
	rgulNum[0] = sdlTmp.u.Lo;
	return sdlTmp.u.Hi;
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

ULONG Div96By64(ULONG *rgulNum, SPLIT64 sdlDen)
{
	LIMITED_METHOD_CONTRACT;

	SPLIT64 sdlQuo;
	SPLIT64 sdlNum;
	SPLIT64 sdlProd;

	sdlNum.u.Lo = rgulNum[0];

	if (rgulNum[2] >= sdlDen.u.Hi) {
		// Divide would overflow.  Assume a quotient of 2^32, and set
		// up remainder accordingly.  Then jump to loop which reduces
		// the quotient.
		//
		sdlNum.u.Hi = rgulNum[1] - sdlDen.u.Lo;
		sdlQuo.u.Lo = 0;
		goto NegRem;
	}

	// Hardware divide won't overflow
	//
	if (rgulNum[2] == 0 && rgulNum[1] < sdlDen.u.Hi)
		// Result is zero.  Entire dividend is remainder.
		//
		return 0;

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

ULONG Div128By96(ULONG *rgulNum, ULONG *rgulDen)
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

// Add a 32 bit unsigned long to an array of 3 unsigned longs representing a 96 integer
// Returns FALSE if there is an overflow
BOOL Add32To96(ULONG *rgulNum, ULONG ulValue) {
	rgulNum[0] += ulValue;
	if (rgulNum[0] < ulValue) {
		if (++rgulNum[1] == 0) {
			if (++rgulNum[2] == 0) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

// Adjust the quotient to deal with an overflow. We need to divide by 10, 
// feed in the high bit to undo the overflow and then round as required, 
void OverflowUnscale(ULONG *rgulQuo, BOOL fRemainder) {
	LIMITED_METHOD_CONTRACT;

	SPLIT64  sdlTmp;

	// We have overflown, so load the high bit with a one.
	sdlTmp.u.Hi = 1u;
	sdlTmp.u.Lo = rgulQuo[2];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10u);
	rgulQuo[2] = sdlTmp.u.Lo;
	sdlTmp.u.Lo = rgulQuo[1];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10u);
	rgulQuo[1] = sdlTmp.u.Lo;
	sdlTmp.u.Lo = rgulQuo[0];
	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, 10u);
	rgulQuo[0] = sdlTmp.u.Lo;
	// The remainder is the last digit that does not fit, so we can use it to work out if we need to round up
	if ((sdlTmp.u.Hi > 5) || ((sdlTmp.u.Hi == 5) && (fRemainder || (rgulQuo[0] & 1)))) {
		Add32To96(rgulQuo, 1u);
	}
}


ULONG IncreaseScale(ULONG *rgulNum, ULONG ulPwr)
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
#include "compare.h"
#include <stdio.h>

void InitializeDecimal(DECIMAL &dec, const uint64_t &low64, const uint32_t &hi32, const uint8_t &scale, uint8_t sign)
{
	dec.Lo64 = low64;
	dec.Hi32 = hi32;
	dec.scale = scale;
	dec.sign = (sign != 0) ? DECIMAL_NEG : 0;
}

void ValidateResult(
	decimal_func func,
	uint64_t lhs_low64, uint32_t lhs_hi32, uint8_t lhs_scale, BYTE lhs_sign,
	uint64_t rhs_low64, uint32_t rhs_hi32, uint8_t rhs_scale, BYTE rhs_sign,
	HRESULT expected_result,
	uint64_t expected_low64, uint32_t expected_hi32, uint8_t expected_scale, BYTE expected_sign)
{
	DECIMAL lhs, rhs, actual, expected;
	InitializeDecimal(lhs, lhs_low64, lhs_hi32, lhs_scale, lhs_sign);
	InitializeDecimal(rhs, rhs_low64, rhs_hi32, rhs_scale, rhs_sign);
	InitializeDecimal(expected, expected_low64, expected_hi32, expected_scale, expected_sign);

	HRESULT actual_result = func(&lhs, &rhs, &actual);

	if (actual_result != expected_result
		||
		(actual_result == 0
			&& (actual.Lo64 != expected.Lo64
				|| actual.Hi32 != expected.Hi32
				|| actual.scale != expected.scale
				|| actual.sign != expected.sign)
			))
	{
		printf("(%i) %lu %I64u / 10^%i  * (%i) %lu %I64u / 10^%i: \n",
			lhs.sign, lhs.Hi32, lhs.Lo64, lhs.scale,
			rhs.sign, rhs.Hi32, rhs.Lo64, rhs.scale);

		if (expected_result != actual_result)
			printf(" RESULT expected %li actual %li\n", expected_result, actual_result);
		else
			printf(" PRODUCT expected (%i) %lu %I64u / 10^%i\n but got (%i) %lu %I64u / 10^%i\n",
				expected.sign, expected.Hi32, expected.Lo64, expected.scale,
				actual.sign, actual.Hi32, actual.Lo64, actual.scale
			);
	}
}

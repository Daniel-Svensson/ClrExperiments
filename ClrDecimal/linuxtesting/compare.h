#pragma once

#include <cstdint>
#include "src/decimal_calc.h"
// oleauto on windows defines 
struct DECIMAL
{
	uint16_t wReserved;
	union {
		struct
		{
			uint8_t scale;
			uint8_t sign;
		};
		uint16_t signscale;
	};
	uint32_t Hi32;
	union
	{
		struct
		{
			uint32_t Lo32;
			uint32_t Mid32;
		};
		uint64_t Lo64;
	};
};
const uint8_t DECIMAL_NEG = 0x80;
typedef HRESULT(*decimal_func)(const DECIMAL*, const DECIMAL*, DECIMAL*);
typedef uint8_t BYTE;

void InitializeDecimal(DECIMAL &dec, const uint64_t &low64, const uint32_t &hi32, const uint8_t &scale, uint8_t sign);

void ValidateResult(
	decimal_func func,
	uint64_t lhs_low64, uint32_t lhs_hi32, uint8_t lhs_scale, BYTE lhs_sign,
	uint64_t rhs_low64, uint32_t rhs_hi32, uint8_t rhs_scale, BYTE rhs_sign,
	HRESULT expected_result,
	uint64_t expected_low64, uint32_t expected_hi32, uint8_t expected_scale, BYTE expected_sign);



void compare_mul();
void compare_add();
void compare_div();
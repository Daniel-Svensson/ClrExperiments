#pragma once

#include <cstdint>
#include "src/common.h"
#include "src/decimal_calc.h"

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
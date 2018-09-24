#include <cstdio>
#include "compare.h"
#include "src/stdafx.h"
#include "src/decimal_calc.inl"

void compare_mul();

int main()
{
#if defined(__aarch64__)
	printf("ARM64\n");
#endif 

#if defined(_M_ARM)
	printf("_M_ARM = %u\n", _M_ARM);
#endif

	uint32_t lo = UINT32_MAX;
	uint32_t hi = 2;
	uint32_t den = 4;
	uint32_t rem, q;

	

	printf("hello from linuxtesting!\n");

	q = DivMod64By32(lo, hi, den, &rem);
	printf("div => q = %u & rem = %u\n", q, rem);
	compare_mul();
	//compare_add();
	//compare_div();
	printf("done \n");
	return 0;
}

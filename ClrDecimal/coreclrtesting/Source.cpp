#include "stdafx.h"
#include <iostream>

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
static constexpr DWORD32 rgulPower10_32[] = {
	1,
	10,
	100,
	1000,
	10000,
	100000,
	1000000,
	10000000,
	100000000
};

__declspec(noinline)
inline void Print(DWORD64 quo, DWORD64 rem)
{
	std::cout << quo << " with rem " << rem << std::endl;
}
__declspec(noinline)
inline void Print(DWORD32 quo, DWORD32 rem)
{
	std::cout << quo << " with rem " << rem << std::endl;
}

inline void DivideAndPrint(DWORD64 value, DWORD64 denominator)
{
	auto quo = (value / denominator);
	auto rem = (value % denominator);

	Print(quo, rem);
}

inline void DivideAndPrint(DWORD32 value, DWORD32 denominator)
{
	auto quo = (value / denominator);
	auto rem = (value % denominator);

	Print(quo, rem);
}
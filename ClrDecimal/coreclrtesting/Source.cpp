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

__declspec(noinline)
void TestDivideByTen32(const DWORD32 value)
{
	DivideAndPrint(value, rgulPower10_32[1]);
	DivideAndPrint(value, rgulPower10_32[2]);
	DivideAndPrint(value, rgulPower10_32[3]);
	DivideAndPrint(value, rgulPower10_32[4]);
	DivideAndPrint(value, rgulPower10_32[5]);
	DivideAndPrint(value, rgulPower10_32[6]);
	DivideAndPrint(value, rgulPower10_32[7]);
	DivideAndPrint(value, rgulPower10_32[8]);
}


__declspec(noinline)
void TestDivideByTen64(const DWORD64 value)
{
	DivideAndPrint(value, rgulPower10_64[1]);
	DivideAndPrint(value, rgulPower10_64[2]);
	DivideAndPrint(value, rgulPower10_64[3]);
	DivideAndPrint(value, rgulPower10_64[4]);
	DivideAndPrint(value, rgulPower10_64[5]);
	DivideAndPrint(value, rgulPower10_64[6]);
	DivideAndPrint(value, rgulPower10_64[7]);
	DivideAndPrint(value, rgulPower10_64[8]);
	DivideAndPrint(value, rgulPower10_64[9]);
	DivideAndPrint(value, rgulPower10_64[10]);
	DivideAndPrint(value, rgulPower10_64[11]);
	DivideAndPrint(value, rgulPower10_64[12]);
	DivideAndPrint(value, rgulPower10_64[13]);
	DivideAndPrint(value, rgulPower10_64[14]);
	DivideAndPrint(value, rgulPower10_64[15]);
	DivideAndPrint(value, rgulPower10_64[16]);
	DivideAndPrint(value, rgulPower10_64[17]);
	DivideAndPrint(value, rgulPower10_64[18]);
}
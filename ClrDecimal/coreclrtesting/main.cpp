// coreclrtesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <vector>
//#include <functional>

#define _CRT_RAND_S  
#include <stdlib.h>  

//#define TEST_32bit_with_0_scale
//#define TEST_32bit_with_scale
//#define TEST_64bit_with_scale_64bit_result
#define TEST_96bit_with_scale_96bit_result

extern "C" DWORD64 _udiv128(DWORD64 low, DWORD64 hi, DWORD64 divisor, DWORD64 *remainder);
// In _x64 file
DWORD64 FullDiv64By64(DWORD64* pdlNum, DWORD64 ulDen);


// The following functions are defined in the classlibnative\bcltype\decimal.cpp
ULONG Div96By32(ULONG *rgulNum, ULONG ulDen);
ULONG Div96By64(ULONG *rgulNum, SPLIT64 sdlDen);
ULONG Div128By96(ULONG *rgulNum, ULONG *rgulDen);
int ScaleResult(ULONG *rgulRes, int iHiRes, int iScale);
ULONG IncreaseScale(ULONG *rgulNum, ULONG ulPwr);

DWORD32 FullDiv64By32_x64(DWORD64* pdlNum, DWORD32 ulDen)
{
	auto mod = DWORD32(*pdlNum % ulDen);
	auto res = *pdlNum / ulDen;

	*pdlNum = res;
	return mod;
}

// Divides a 96bit ulong by 32bit, returns 32bit remainder
DWORD32 Div96By32_x64(DWORD32 *pdlNum, DWORD32 ulDen)
{
	// Upper 64bit
	DWORD64* hiPtr = (DWORD64*)(pdlNum + 1);
	DWORD64 lopart = (FullDiv64By64(hiPtr, ulDen) << 32) + *pdlNum;
	DWORD32 remainder = FullDiv64By32_x64(&lopart, ulDen);
	*pdlNum = (DWORD32)lopart;

	return remainder;
}

DWORD32 Div96By32_x64_v2(DWORD32 *pdlNum, DWORD32 ulDen)
{
	// Upper 64bit
	DWORD64 hi = pdlNum[2];
	DWORD64 lo = *(DWORD64*)pdlNum;
	*(DWORD64*)pdlNum = _udiv128(lo, hi, ulDen, &hi);
	pdlNum[2] = 0;
	return (DWORD32)hi;
}

static ULONG FullDiv64By32_ORI(DWORDLONG *pdlNum, ULONG ulDen)
{
	SPLIT64  sdlTmp;
	SPLIT64  sdlRes;

	sdlTmp.int64 = *pdlNum;
	sdlRes.u.Hi = 0;

	if (sdlTmp.u.Hi >= ulDen) {
		// DivMod64by32 returns quotient in Lo, remainder in Hi.
		//
		sdlRes.u.Lo = sdlTmp.u.Hi;
		sdlRes.int64 = DivMod64by32(sdlRes.int64, ulDen);
		sdlTmp.u.Hi = sdlRes.u.Hi;
		sdlRes.u.Hi = sdlRes.u.Lo;
	}

	sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulDen);
	sdlRes.u.Lo = sdlTmp.u.Lo;
	*pdlNum = sdlRes.int64;
	return sdlTmp.u.Hi;
}


void TestMultiply(DECIMAL a, DECIMAL b)
{
	DECIMAL prod, prod2;
	HRESULT res1 = VarDecMul(&a, &b, &prod);
	HRESULT res2 = VarDecMul_x64(&a, &b, &prod2);

	printf("Original DecMul %u %u %u sign %i scale %i\n (RETURN %i)", prod.Hi32, prod.Mid32, prod.Lo32, (int)prod.sign, (int)prod.scale, res1);
	printf("Ny DecMul %u %u %u sign %i scale %i (RETURN %i)\n", prod2.Hi32, prod2.Mid32, prod2.Lo32, (int)prod2.sign, (int)prod2.scale, res2);

	DWORD64 dividend = prod.Lo64;
	DWORD64 divisor64 = b.Lo64 + 2;
	DWORD32 divisor32 = static_cast<DWORD32>(b.Lo64 + 2);

	auto minRes = dividend;
	auto min = FullDiv64By32_x64(&minRes, divisor32);
	auto derasRes = dividend;
	auto deras = FullDiv64By32_ORI(&derasRes, divisor32);
	auto min64Res = dividend;
	auto min64 = FullDiv64By64(&min64Res, divisor64);

	auto dummy = FullDiv64By64(&min64Res, divisor64);

	printf("Original Div6432  %I64u %i\n", derasRes, deras);
	printf("Ny Div6432        %I64u %i \n", minRes, min);
	printf("Ny FullDiv64By64  %I64u %I64u \n", min64Res, min64);
}

long long run_benchmark(const char *const name, 
	DECIMAL* lhs, int lhs_count, 
	DECIMAL* rhs, int rhs_count, 
	DECIMAL* target, HRESULT* hresult, int target_count,
	HRESULT(*func)(DECIMAL*, DECIMAL*, DECIMAL*));
void CompareResult(const char * A, const char * B,
	const DECIMAL* lhs, int lhs_count,
	const DECIMAL* rhs, int rhs_count,
	DECIMAL* expected, HRESULT* expected_res, DECIMAL* actual, HRESULT* actual_res, int result_count);
void run_benchmarks(int count, int bytes, int numtests);
void InitializeTestData(DECIMAL * numbers, int count, DECIMAL * targetA, DECIMAL * targetC, int bytes);
void test_round_to_nearest();

int main()
{
	DECIMAL a, b;
	VarDecFromI4(32, &a);
	VarDecFromI4(3, &b);

	DECIMAL sum;
	VarDecAdd(&a, &b, &sum);

	//TestMultiply(a, b);

	a.Mid32 = 4294967295;
	a.Lo32 = 4294967295;
	a.scale = 23;
	b.Lo64 = 0;
	b.scale = 12;

	VarDecMul_x64(&a, &b, &sum);

	//test_round_to_nearest();

	//run_benchmarks(30000, 4, 5);
	run_benchmarks(15000, 4, 5);
	//run_benchmarks(10000, 8);
						
	//auto str = COMDecimal::ToString(sum);
    return 0;
}

long long run_benchmark(const char *const name,
	DECIMAL* lhs, int lhs_count,
	DWORD32 denominator,
	DWORD32(*func)(DWORD32*, DWORD32))
{
	LARGE_INTEGER start, end, frequency;
	QueryPerformanceCounter(&start);
	DWORD32 totalSum = 0;
	for (int i = 0; i < lhs_count; ++i)
		totalSum += func((DWORD32*)&lhs->Lo32, 14);
	QueryPerformanceCounter(&end);

	auto elapsed = end.QuadPart - start.QuadPart;
	QueryPerformanceFrequency(&frequency);

	printf("%s;%I64u;%f\n", name, elapsed, (double)elapsed / (double)frequency.QuadPart);

	return elapsed;
}


void run_benchmarks(int count, int bytes, int numtests)
{
	DECIMAL* numbers = (DECIMAL*)calloc(sizeof(DECIMAL), count);
	DECIMAL* targetA = new DECIMAL[count*count];
	DECIMAL* targetC = new DECIMAL[count*count];
	HRESULT* hresultA = new HRESULT[count*count];
	HRESULT* hresultC = new HRESULT[count*count];
	InitializeTestData(numbers, count, targetA, targetC, bytes);

#ifdef TEST_32bit_with_0_scale
	printf("32bit multiply with scale = 0\n");
	for (int i = 0; i < numtests; ++i)
	{
		run_benchmark("oleauto", numbers, count, numbers, count, targetA, hresultA, count*count, VarDecMul);
		run_benchmark("x64", numbers, count, numbers, count, targetC, hresultC, count*count, VarDecMul_x64);

//		run_benchmark("Div96By32", targetA, count*count, SHORT_MAX, (DWORD32(*)(DWORD32*, DWORD32)) Div96By32);
//		run_benchmark("Div96By32_x64", targetC, count*count, SHORT_MAX, Div96By32_x64);
	}
	CompareResult("oleauto", "x64", numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, count*count);
#endif


#ifdef TEST_32bit_with_scale
	// Change scale
	const int minScale = 10;
	const int maxScale = DEC_SCALE_MAX - 2;
	printf("Scale [%i, %i]\n", minScale, maxScale);
	for (int i = 0; i < count;++i)
		numbers[i].scale = (rand() % (maxScale - minScale)) + minScale;

	for (int i = 0; i < numtests; ++i)
	{
		run_benchmark("oleauto", numbers, count, numbers, count, targetA, hresultA, count*count, VarDecMul);
		run_benchmark("x64", numbers, count, numbers, count, targetC, hresultC, count*count, VarDecMul_x64);

//		run_benchmark("Div96By32_x64_v2", targetA, count*count, SHORT_MAX, Div96By32_x64_v2);
//		run_benchmark("Div96By32", targetA, count*count, 14, (DWORD32(*)(DWORD32*, DWORD32)) Div96By32);
	}
	CompareResult("oleauto", "x64", numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, count*count);
#endif

//#ifdef TEST_64bit_with_scale_64bit_result
#ifdef TEST_64bit_with_scale_64bit_result
	printf("64bit values -> 64bit results with varying scale\n");
	for (int i = 0; i < count;++i)
		numbers[i].Mid32 = numbers[i].Lo32 >> 4;

	// use 4 bits with scales [0,28]
	const BYTE smallNumScaleLimit = DEC_SCALE_MAX;
	const size_t smallNumBitLimit = (1 << 4) - 1;
	const size_t numSmallNumbers = smallNumBitLimit * (smallNumScaleLimit + 1);
	DECIMAL smallNumbers[numSmallNumbers] = { 0 };
	DECIMAL* smallNumbersInit = smallNumbers;
	for (size_t bits = 1; bits <= smallNumBitLimit; bits++)
	{
		for (BYTE scale = 0; scale <= smallNumScaleLimit; scale++)
		{
			VarDecFromUint(bits, smallNumbersInit);
			smallNumbersInit->scale = scale;
			++smallNumbersInit;
		}
	}
	assert((smallNumbersInit - smallNumbers) == numSmallNumbers);

	for (int i = 0; i < numtests; ++i)
	{
		run_benchmark("oleauto", numbers, count, smallNumbers, numSmallNumbers, targetA, hresultA, count*numSmallNumbers, VarDecMul);
		run_benchmark("x64", numbers, count, smallNumbers, numSmallNumbers, targetC, hresultC, count*numSmallNumbers, VarDecMul_x64);
	}
	CompareResult("oleauto", "x64", numbers, count, smallNumbers, numSmallNumbers, targetA, hresultA, targetC, hresultC, count*numSmallNumbers);
#endif

#ifdef TEST_96bit_with_scale_96bit_result
	printf("96bit values with varying scale\n");
	for (int i = 0; i < count;++i)
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	for (int i = 0; i < numtests; ++i)
	{
		//run_benchmark("oleauto", numbers, count, numbers, count, targetA, hresultA, count*count, VarDecMul);
		run_benchmark("x64", numbers, count, numbers, count, targetC, hresultC, count*count, VarDecMul_x64);

//		run_benchmark("Div96By32_x64", targetA, count*count, 14, Div96By32_x64);
//		run_benchmark("Div96By32_x64_v2", targetC, count*count, 14, Div96By32_x64_v2);
	}
//CompareResult("oleauto", "x64", numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, count*count);
#endif
}

void InitializeTestData(DECIMAL * numbers, int count, DECIMAL * targetA, DECIMAL * targetC, int bytes)
{
	// Start with some specific patters
	const int nibbles = bytes * 2;
	const int bits = bytes * 8;

	const DWORD64 allBits = (0xffffffffffffffffui64) >> (64 - bits);
	const DWORD64 noBits = (0x0000000000000000ui64);

	assert(count >= 2 + 3 * bits + 2 * bytes);

	// Genereate 0x0000000000000000, 0x000000000000000f, 0x00000000000000ff, to 0xffffffffffffffffu
	int dest = 0;
	VarDecFromUI8(allBits, numbers + (dest++));
	VarDecFromUI8(noBits, numbers + (dest++));

	// Some bit patterns
	for (int i = 1; i < bits; ++i)
	{
		// i bits to left set
		VarDecFromUI8((allBits >> i) & allBits, numbers + (dest++));
		// i bits to right set
		VarDecFromUI8((allBits << i) & allBits, numbers + (dest++));
		// only bit i set
		VarDecFromUI8((1ui64 << i) & allBits, numbers + (dest++));
	}

	// Genereate 0x000000000000000f, 0x0000000000000f0f, ...	
	for (int i = 1; i < bytes; ++i)
	{
		VarDecFromUI8((allBits >> i * 8) & allBits, numbers + (dest++));
		VarDecFromUI8((allBits << i * 8) & allBits, numbers + (dest++));
	}

	// Generate deterministic random sequence to fill the rest of the numbers
	//CLRRandom random;
	//random.Init(42);
	srand(42);
	for (;dest < count; ++dest)
	{
		DWORD64 number = 0;
		for (int i = 0; i < bytes; ++i)
			number = (number << 8) | (DWORD64)(rand() & 0x00ff);

		assert(number == (number & allBits));
		//random.NextBytes((byte*)&number, bytes);

		VarDecFromUI8(number & allBits, numbers + dest);
	}
}

//void CompareResult(const char * A, const char * B, int count, DECIMAL * numbers,  DECIMAL * targetA, DECIMAL * targetB)
void CompareResult(const char * A, const char * B, 
	const DECIMAL* lhs, int lhs_count,
	const DECIMAL* rhs, int rhs_count,
	DECIMAL* expected, HRESULT* expected_result, DECIMAL* actual, HRESULT* actual_result, int result_count)
{
	assert(result_count == lhs_count * rhs_count);

	size_t scale_diff = 0;

	for (int i = 0; i < lhs_count; ++i)
	for (int j = 0; j < rhs_count; ++j)
	{
		auto result_idx = (i * lhs_count) + j;

		if (expected_result[result_idx] != actual_result[result_idx]
			|| (expected[result_idx].Lo64 != actual[result_idx].Lo64)
			|| (expected[result_idx].Hi32 != actual[result_idx].Hi32)
			|| (expected[result_idx].signscale != actual[result_idx].signscale))
		{
			auto cmp = VarDecCmp(expected + result_idx, actual + result_idx);
			if (cmp == VARCMP_EQ)
			{
				if (expected_result[result_idx] != actual_result[result_idx])
					printf("[%i] ONLY RESULT DIFFERENT?\n", result_idx);
				else
				{
					++scale_diff;
					//printf("[%i] FALSE POSITIVE\n", result_idx);
				}
			}
			else
			{
				printf("[%i]x[%i] expected results '%i' had %i\n", i, j, expected_result[result_idx], actual_result[result_idx]);
				printf("(%i) %u %u %u /10^%i  * (%i) %u %u %u / 10^%i: expected (%i) %u %u %u / 10^%i but got (%i) %u %u %u / 10^%i\n",
					lhs[i].sign, lhs[i].Hi32, lhs[i].Mid32, lhs[i].Lo32, lhs[i].scale,
					rhs[j].sign, rhs[j].Hi32, rhs[j].Mid32, rhs[j].Lo32, rhs[j].scale,
					expected[result_idx].sign, expected[result_idx].Hi32, expected[result_idx].Mid32, expected[result_idx].Lo32, expected[result_idx].scale,
					actual[result_idx].sign, actual[result_idx].Hi32, actual[result_idx].Mid32, actual[result_idx].Lo32, actual[result_idx].scale
				);
			}
		}
	}

	
	printf("%zi equal results with different scale (FALSE POSITIVE)\n", scale_diff);
}

long long run_benchmark(const char *const name,
	DECIMAL* lhs, int lhs_count,
	DECIMAL* rhs, int rhs_count,
	DECIMAL* target, HRESULT *result, int target_count,
	HRESULT(*func)(DECIMAL*, DECIMAL*, DECIMAL*))
{
	assert(target_count == lhs_count * rhs_count);

	LARGE_INTEGER start, end, frequency;
	QueryPerformanceCounter(&start);

	DECIMAL* destination = target;
	for (int i = 0; i < lhs_count; ++i)
		for(int j = 0; j < rhs_count; ++j)
			*result++ = func(lhs + i, rhs + j, destination++);
	QueryPerformanceCounter(&end);

	auto elapsed = end.QuadPart - start.QuadPart;
	QueryPerformanceFrequency(&frequency);

	printf("%s;%I64u;%f\n", name, elapsed, (double)elapsed / (double)frequency.QuadPart);

	return elapsed;
}


void test_round_to_nearest()
{
	DECIMAL a, b;
	VarDecFromI4(32, &a);
	VarDecFromI4(3, &b);

	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 96

	VarDecFromI4(95, &a);
	VarDecFromI4(1, &b);
	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 95 => 100 (even)

	VarDecFromI4(94, &a);
	VarDecFromI4(1, &b);
	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 96

	VarDecFromI4(86, &a);
	VarDecFromI4(1, &b);
	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 85

	VarDecFromI4(85, &a);
	VarDecFromI4(1, &b);
	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 85 => 80 (even)

	VarDecFromI4(84, &a);
	VarDecFromI4(1, &b);
	a.scale = 10;
	b.scale = 19;
	TestMultiply(a, b); // 85

}
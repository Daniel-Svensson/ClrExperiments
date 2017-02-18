// coreclrtesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <vector>
//#include <functional>

#define _CRT_RAND_S  
#include <stdlib.h>  

using namespace std;
//#define TEST_MULTIPLY
#define TEST_ADDSUB
#define TEST_32bit_with_0_scale
#define TEST_32bit_with_scale
#define TEST_64bit_with_scale_64bit_result
#define TEST_96bit_with_scale_96bit_result_and_overflow
#define TEST_64bit_with_0_scale_128bit_result
#define TEST_64bit_with_scale_128bit_result
#define TEST_96bit_with_scale_96bit_result_no_overflow

void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(*baseline)(const DECIMAL *, const DECIMAL *, DECIMAL *), HRESULT(*func)(const DECIMAL *, const DECIMAL *, DECIMAL *));
void InitializeTestData(std::vector<DECIMAL>& numbers, int bytes);
void test_round_to_nearest();

extern "C" DWORD64 _udiv128(DWORD64 low, DWORD64 hi, DWORD64 divisor, DWORD64 *remainder);
// In _x64 file
DWORD64 FullDiv64By64(DWORD64* pdlNum, DWORD64 ulDen);

// The following functions are defined in the classlibnative\bcltype\decimal.cpp

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

ULONG FullDiv64By32_ORI(DWORDLONG *pdlNum, ULONG ulDen)
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

	printf("Original DecMul %lu %lu %lu sign %i scale %i\n (RETURN %li)", prod.Hi32, prod.Mid32, prod.Lo32, (int)prod.sign, (int)prod.scale, res1);
	printf("Ny DecMul %lu %lu %lu sign %i scale %i (RETURN %li)\n", prod2.Hi32, prod2.Mid32, prod2.Lo32, (int)prod2.sign, (int)prod2.scale, res2);

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

	printf("Original Div6432  %I64u %lu\n", derasRes, deras);
	printf("Ny Div6432        %I64u %i \n", minRes, min);
	printf("Ny FullDiv64By64  %I64u %I64u \n", min64Res, min64);
}


BYTE random_scale(int min, int max)
{
	return (BYTE)(rand() % (max - min)) + min;
}

// Runs a single benchmark pass and records the timing when calling func
// with for all combination ov values from lhs and rhs.
long long run_benchmark(
	const std::vector<DECIMAL>& lhs,
	const std::vector<DECIMAL>& rhs,
	std::vector<DECIMAL>& target, std::vector<HRESULT>& result,
	HRESULT(*func)(const DECIMAL*, const DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);

	auto destination = target.begin();
	auto res = result.begin();
	for (int i = 0; i < lhs.size(); ++i)
		for (int j = 0; j < rhs.size(); ++j)
			*(res++) = func(&lhs[i], &rhs[j], &*(destination++));
	QueryPerformanceCounter(&end);

	return end.QuadPart - start.QuadPart;
}

void CompareResult(
	const std::vector<DECIMAL>& lhs,
	const std::vector<DECIMAL>& rhs,
	const std::vector<DECIMAL>& expected, const std::vector<HRESULT>& expected_result,
	const std::vector<DECIMAL>& actual, const std::vector<HRESULT>& actual_result)
{
	assert(expected.size() >= lhs.size()* rhs.size());
	assert(expected.size() == actual.size());
	assert(expected.size() == expected_result.size());
	assert(expected.size() == actual_result.size());

	size_t scale_diff = 0;
	size_t errors = 0;


	for (int i = 0; i < lhs.size(); ++i)
		for (int j = 0; j < rhs.size(); ++j)
		{
			auto result_idx = (i * lhs.size()) + j;

			if (expected_result[result_idx] != actual_result[result_idx]
				|| (expected[result_idx].Lo64 != actual[result_idx].Lo64)
				|| (expected[result_idx].Hi32 != actual[result_idx].Hi32)
				|| (expected[result_idx].signscale != actual[result_idx].signscale))
			{
				auto cmp = VarDecCmp(const_cast<DECIMAL*>(&expected[result_idx]), const_cast<DECIMAL*>(&actual[result_idx]));
				if (cmp == VARCMP_EQ)
				{
					if (expected_result[result_idx] != actual_result[result_idx])
						printf("[%zi] ONLY RESULT DIFFERENT?\n", result_idx);
					else
					{
						++scale_diff;
						//printf("[%i] FALSE POSITIVE\n", result_idx);
					}
				}
				else
				{
					if (++errors < 10)
					{
						printf("[%i]x[%i]  -- (%i) %lu %I64u /10^%i  * (%i) %lu %I64u / 10^%i: \n",
							i, j,
							lhs[i].sign, lhs[i].Hi32, lhs[i].Lo64, lhs[i].scale,
							rhs[j].sign, rhs[j].Hi32, rhs[j].Lo64, rhs[j].scale);

						if (expected_result[result_idx] != actual_result[result_idx])
							printf(" RESULT expected %li actual %li\n", expected_result[result_idx], actual_result[result_idx]);
						else
							printf(" PRODUCT expected (%i) %lu %I64u / 10^%i but got (%i) %lu %I64u / 10^%i\n",
								expected[result_idx].sign, expected[result_idx].Hi32, expected[result_idx].Lo64, expected[result_idx].scale,
								actual[result_idx].sign, actual[result_idx].Hi32, actual[result_idx].Lo64, actual[result_idx].scale
							);
					}
				}
			}
		}


	if (errors > 0)
		printf("ERRORS FOUND: %zi with different results (%f%%)\n", errors, 100.0 * (double)errors / (double)(expected.size()));
	printf("%zi equal results with different scale (FALSE POSITIVE)\n", scale_diff);
}

void compare_benchmark(
	const char *const scenario,
	const char *const first,
	const char *const second,
	const int iterations,
	const std::vector<DECIMAL>& lhs,
	const std::vector<DECIMAL>& rhs,
	std::vector<DECIMAL>& first_target, std::vector<HRESULT>& first_result,
	std::vector<DECIMAL>& second_target, std::vector<HRESULT>& second_result,
	HRESULT(*first_func)(const DECIMAL*, const DECIMAL*, DECIMAL*),
	HRESULT(*second_func)(const DECIMAL*, const DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	printf("\n\nStarting Scenario %20s\n", scenario);

	printf("%s;;%s;;;%% time;speedup\n", first, second);
	printf("ticks;sec;ticks;sec;;;\n");

	for (int i = 0; i < iterations; ++i)
	{
		auto time1 = run_benchmark(lhs, rhs, first_target, first_result, first_func);
		auto time2 = run_benchmark(lhs, rhs, second_target, second_result, second_func);

		double seconds1 = (double)time1 / (double)frequency.QuadPart;
		double seconds2 = (double)time2 / (double)frequency.QuadPart;

		double percent_of_original = (seconds2 / seconds1);
		double speedup = (seconds1 - seconds2) / seconds2;

		printf("%I64u;%g;%I64u;%g;;%f%%;%f%%\n", time1, seconds1, time2, seconds2, percent_of_original * 100.0, speedup* 100.0);
	}
	printf("\n");

	int first_hres0 = (int)std::count(first_result.begin(), first_result.end(), 0);
	int second_hres0 = (int)std::count(second_result.begin(), second_result.end(), 0);
	//cout << second_hres0 << " out of " << first_target.size() << " resuts was successfull, expected " << first_hres0 << " => ratio " << 100.0 * (double)second_hres0 / (double)(first_target.size()) << "%" << endl;
	printf("%d out of %zi results was successfull, expected %d, ratio is %f%%\n", second_hres0, first_target.size(), first_hres0, 100.0 * (double)second_hres0 / (double)(first_target.size()));

	CompareResult(lhs, rhs, first_target, first_result, second_target, second_result);
}

long long run_benchmark(const char *const name,
	DECIMAL* lhs, int lhs_count,
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

void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(*baseline)(DECIMAL *, DECIMAL *, DECIMAL *), HRESULT(*func)(DECIMAL *, DECIMAL *, DECIMAL *))
{
	run_benchmarks(iterations, elements, bytes,
		baseline_name, func_name,
		(HRESULT(*)(const DECIMAL *, const DECIMAL *, DECIMAL *))baseline,
		(HRESULT(*)(const DECIMAL *, const DECIMAL *, DECIMAL *))func);
}

void run_benchmarks(int iterations, int elements, int bytes,
	 const char *const baseline_name, const char *const func_name,
	HRESULT(*baseline)(const DECIMAL *, const DECIMAL *, DECIMAL *), HRESULT(*func)(const DECIMAL *, const DECIMAL *, DECIMAL *))
{
	std::vector<DECIMAL> numbers(elements);
	std::vector<DECIMAL> targetA(elements*elements);
	std::vector<DECIMAL> targetC(elements*elements);
	std::vector<HRESULT> hresultA(elements*elements);
	std::vector<HRESULT> hresultC(elements*elements);
	InitializeTestData(numbers, bytes);

	// Change scale
	const int minScale = 10;
	const int maxScale = DEC_SCALE_MAX - 2;

#ifdef TEST_32bit_with_0_scale
	compare_benchmark("32bit x 32bit with scale 0", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif


#ifdef TEST_32bit_with_scale
	for (size_t i = 0; i < numbers.size();++i)
		numbers[i].scale = random_scale(minScale, maxScale);

	compare_benchmark("32bit x 32bit with scale in range [10,26]", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

	//#ifdef TEST_64bit_with_scale_64bit_result
#ifdef TEST_64bit_with_scale_64bit_result
	for (size_t i = 0; i < numbers.size();++i)
	{
		numbers[i].scale = random_scale(minScale, maxScale);
		numbers[i].Mid32 = numbers[i].Lo32 >> 4;
	}

	// use 4 bits with scales [0,28]
	const BYTE smallNumScaleLimit = DEC_SCALE_MAX;
	const size_t smallNumBitLimit = (1 << 4) - 1;
	const size_t numSmallNumbers = smallNumBitLimit * (smallNumScaleLimit + 1);
	std::vector<DECIMAL> smallNumbers(numSmallNumbers);
	auto smallNumbersInit = smallNumbers.data();
	for (int bits = 1; bits <= smallNumBitLimit; bits++)
	{
		for (BYTE scale = 0; scale <= smallNumScaleLimit; scale++)
		{
			VarDecFromUint(bits, smallNumbersInit);
			smallNumbersInit->scale = scale;
			++smallNumbersInit;
		}
	}
	compare_benchmark("64bit values -> 64bit results with varying scale", baseline_name, func_name, iterations, numbers, smallNumbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_64bit_with_0_scale_128bit_result
	for (size_t i = 0; i < numbers.size();++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = 0;
	}

	compare_benchmark("64bit values -> 65-128 bit results and no scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_64bit_with_scale_128bit_result
	for (size_t i = 0; i < numbers.size();++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = random_scale(2, 9);
	}

	compare_benchmark("64bit values -> 65-128 bit results and scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_and_overflow
	for (size_t i = 0; i < numbers.size();++i)
	{
		numbers[i].scale = rand() % 5;
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	}

	compare_benchmark("96bit values with high overflow probablility", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_no_overflow
	for (size_t i = 0; i < numbers.size();++i)
	{
		numbers[i].scale = (rand() % (maxScale - minScale)) + minScale;
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	}

	compare_benchmark("96bit values with no overflow", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif
}

void InitializeTestData(std::vector<DECIMAL>& numbers, int bytes)
{
	// Start with some specific patters
	const int nibbles = bytes * 2;
	const int bits = bytes * 8;

	const DWORD64 allBits = (0xffffffffffffffffui64) >> (64 - bits);
	const DWORD64 noBits = (0x0000000000000000ui64);

	assert(numbers.size() >= 2 + 3 * bits + 2 * bytes);

	// Genereate 0x0000000000000000, 0x000000000000000f, 0x00000000000000ff, to 0xffffffffffffffffu
	int dest = 0;
	VarDecFromUI8(allBits, &numbers[dest++]);
	VarDecFromUI8(noBits, &numbers[dest++]);

	// Some bit patterns
	for (int i = 1; i < bits; ++i)
	{
		// i bits to left set
		VarDecFromUI8((allBits >> i) & allBits, &numbers[dest++]);
		// i bits to right set
		VarDecFromUI8((allBits << i) & allBits, &numbers[dest++]);
		// only bit i set
		VarDecFromUI8((1ui64 << i) & allBits, &numbers[dest++]);
	}

	// Genereate 0x000000000000000f, 0x0000000000000f0f, ...	
	for (int i = 1; i < bytes; ++i)
	{
		VarDecFromUI8((allBits >> i * 8) & allBits, &numbers[dest++]);
		VarDecFromUI8((allBits << i * 8) & allBits, &numbers[dest++]);
	}

	// Generate deterministic random sequence to fill the rest of the numbers
	srand(42);
	for (;dest < numbers.size(); ++dest)
	{
		DWORD64 number = 0;
		for (int i = 0; i < bytes; ++i)
			number = (number << 8) | (DWORD64)(rand() & 0x00ff);

		assert(number == (number & allBits));
		//random.NextBytes((byte*)&number, bytes);

		VarDecFromUI8(number & allBits, &numbers[dest]);
	}
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

int main()
{
	// Use system formatting
	setlocale(LC_ALL, "");

	DECIMAL a, b;
	VarDecFromI4(32, &a);
	VarDecFromI4(3, &b);

	DECIMAL sum;
	VarDecAdd(&a, &b, &sum);

	//TestMultiply(a, b);
	//[0]x[0]  --(0) 4294967295 18446744073709551615 / 10 ^ 16 * (0) 4294967295 18446744073709551615 / 10 ^ 16:
	// PRODUCT expected(0) 858993459 3689348814741910323 / 10 ^ 15 but got(0) 0 9223372036854775808 / 10 ^ 15
// 		[0]x[10]  --(0) 4294967295 18446744073709551615 / 10 ^ 16 * (0) 8 34359738376 / 10 ^ 10 :
		//PRODUCT expected(0) 430296729 11071482418063330970 / 10 ^ 15 but got(0) 8000000 7382133603321420646 / 10 ^ 15

	// 2]x[11]  --(0) 2147483647 9223372034707292159 / 10 ^ 4 * (0) 268435455 1152921500580315135 / 10 ^ 3:
	// PRODUCT expected(0) 483183819 14987979555647730482 / 10 ^ 3 but got(0) 536870903 7609281926163909836 / 10 ^ 3

	a.Hi32 = 2147483647;
	a.Lo64 = 9223372034707292159;
	a.scale = 4;
	b.Hi32 = 268435455;
	b.Lo64 = 1152921500580315135;
	b.scale = 3;

	VarDecAdd_x64(&a, &b, &sum);
	//VarDecMul_x64(&a, &b, &sum);

	assert(sum.Hi32 == 483183819);
	assert(sum.Lo64 == 14987979555647730482);
	assert(sum.scale == 3);
	assert(sum.sign == 0);

	//test_round_to_nearest();

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS /* or ABOVE_NORMAL_PRIORITY_CLASS */);

	//run_benchmarks(30000, 4, 5);
#ifdef DEBUG
	const int iterations = 2;
	const int elements = 1000;
#else
	const int iterations = 5;
	const int elements = 6000;
#endif
	const int bytes = 4;

#ifdef TEST_MULTIPLY
	run_benchmarks(iterations, elements, bytes, "oleauto", "x64", VarDecMul, VarDecMul_x64);
#endif

#ifdef TEST_ADDSUB
	run_benchmarks(iterations, elements, bytes, "oleauto", "x64", VarDecAdd, VarDecAdd_x64);
	run_benchmarks(iterations, elements, bytes, "oleauto", "x64", VarDecSub, VarDecSub_x64);
#endif

	return 0;
}

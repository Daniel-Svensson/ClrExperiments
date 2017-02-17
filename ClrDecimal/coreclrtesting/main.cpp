// coreclrtesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <vector>
//#include <functional>

#define _CRT_RAND_S  
#include <stdlib.h>  

#define TEST_32bit_with_0_scale
#define TEST_32bit_with_scale
#define TEST_64bit_with_scale_64bit_result
#define TEST_96bit_with_scale_96bit_result_and_overflow
#define TEST_64bit_with_0_scale_128bit_result
#define TEST_64bit_with_scale_128bit_result
#define TEST_96bit_with_scale_96bit_result_no_overflow

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

void compare_benchmark(const char *const scenario,
	const char *const first, 
	const char *const second,
	int iterations,
	DECIMAL* lhs, int lhs_count,
	DECIMAL* rhs, int rhs_count,
	DECIMAL* first_target, HRESULT* first_hresult,
	DECIMAL* second_target, HRESULT* second_hresul,
	HRESULT(*first_func)(DECIMAL*, DECIMAL*, DECIMAL*),
	HRESULT(*second_func)(DECIMAL*, DECIMAL*, DECIMAL*));

long long run_benchmark(
	DECIMAL* lhs, int lhs_count, 
	DECIMAL* rhs, int rhs_count, 
	DECIMAL* target, HRESULT* hresult,
	HRESULT(*func)(DECIMAL*, DECIMAL*, DECIMAL*));
void CompareResult(const char * A, const char * B,
	const DECIMAL* lhs, int lhs_count,
	const DECIMAL* rhs, int rhs_count,
	DECIMAL* expected, HRESULT* expected_res, DECIMAL* actual, HRESULT* actual_res, int result_count);
void run_benchmarks(int count, int bytes, int numtests);
void InitializeTestData(DECIMAL * numbers, int count, DECIMAL * targetA, DECIMAL * targetC, int bytes);
void test_round_to_nearest();


BYTE random_scale(int min, int max)
{
	return (rand() % (max - min)) + min;
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

	a.Hi32 = 4294967295;
	a.Lo64 = 18446744073709551615;
	a.scale = 16;
	b.Hi32 = 4;
	b.Lo64 = 17179869188;
	b.scale = 25;

	VarDecMul_x64(&a, &b, &sum);

	//test_round_to_nearest();

	//run_benchmarks(30000, 4, 5);
#if DEBUG
	run_benchmarks(1000, 4, 2);
#else
	run_benchmarks(6000, 4, 5);
#endif
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

	// Change scale
	const int minScale = 10;
	const int maxScale = DEC_SCALE_MAX - 2;

#ifdef TEST_32bit_with_0_scale
	compare_benchmark("32bit x 32bit with scale 0", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif


#ifdef TEST_32bit_with_scale
	for (int i = 0; i < count;++i)
		numbers[i].scale = random_scale(minScale, maxScale);

	compare_benchmark("32bit x 32bit with scale in range [10,26]", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif

//#ifdef TEST_64bit_with_scale_64bit_result
#ifdef TEST_64bit_with_scale_64bit_result
	for (int i = 0; i < count;++i)
	{
		numbers[i].scale = random_scale(minScale, maxScale);
		numbers[i].Mid32 = numbers[i].Lo32 >> 4;
	}

	// use 4 bits with scales [0,28]
	const BYTE smallNumScaleLimit = DEC_SCALE_MAX;
	const size_t smallNumBitLimit = (1 << 4) - 1;
	const size_t numSmallNumbers = smallNumBitLimit * (smallNumScaleLimit + 1);
	DECIMAL smallNumbers[numSmallNumbers] = { 0 };
	DECIMAL* smallNumbersInit = smallNumbers;
	for (int bits = 1; bits <= smallNumBitLimit; bits++)
	{
		for (BYTE scale = 0; scale <= smallNumScaleLimit; scale++)
		{
			VarDecFromUint(bits, smallNumbersInit);
			smallNumbersInit->scale = scale;
			++smallNumbersInit;
		}
	}
	assert((smallNumbersInit - smallNumbers) == numSmallNumbers);

	compare_benchmark("64bit values -> 64bit results with varying scale", "oleauto", "x64", numtests, numbers, count, smallNumbers, numSmallNumbers, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif

#ifdef TEST_64bit_with_0_scale_128bit_result
	for (int i = 0; i < count;++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = 0;
	}

	compare_benchmark("64bit values -> 65-128 bit results and no scale", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif

#ifdef TEST_64bit_with_scale_128bit_result
	for (int i = 0; i < count;++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = random_scale(2, 9);
	}

	compare_benchmark("64bit values -> 65-128 bit results and scale", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_and_overflow
	for (int i = 0; i < count;++i)
	{
		numbers[i].scale = rand() % 5;
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	}

	compare_benchmark("96bit values with high overflow probablility", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_no_overflow
	for (int i = 0; i < count;++i)
	{
		numbers[i].scale = (rand() % (maxScale - minScale)) + minScale;
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	}

	compare_benchmark("96bit values with no overflow", "oleauto", "x64", numtests, numbers, count, numbers, count, targetA, hresultA, targetC, hresultC, VarDecMul, VarDecMul_x64);
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
	size_t errors = 0;

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
				if (++errors < 10)
				{
					printf("[%i]x[%i]  -- (%i) %u %I64u /10^%i  * (%i) %u %I64u / 10^%i: \n", 
						i, j,
						lhs[i].sign, lhs[i].Hi32, lhs[i].Lo64, lhs[i].scale,
						rhs[j].sign, rhs[j].Hi32, rhs[j].Lo64, rhs[j].scale);

					if (expected_result[result_idx] != actual_result[result_idx])
						printf(" RESULT expected %i actual %i\n", expected_result[result_idx], actual_result[result_idx]);
					else
						printf(" PRODUCT expected (%i) %u %I64u / 10^%i but got (%i) %u %I64u / 10^%i\n",
							expected[result_idx].sign, expected[result_idx].Hi32, expected[result_idx].Lo64, expected[result_idx].scale,
							actual[result_idx].sign, actual[result_idx].Hi32, actual[result_idx].Lo64, actual[result_idx].scale
						);
				}
			}
		}
	}

	
	if (errors > 0)
		printf("ERRORS FOUND: %zi with different results (%f%%)\n", errors, 100.0 * (double)errors/(double)(lhs_count*rhs_count));
	printf("%zi equal results with different scale (FALSE POSITIVE)\n", scale_diff);
}

void compare_benchmark(
	const char *const scenario,
	const char *const first, 
	const char *const second,
	const int iterations,
	DECIMAL* lhs, int lhs_count,
	DECIMAL* rhs, int rhs_count,
	DECIMAL* first_target, HRESULT* first_hresult,
	DECIMAL* second_target, HRESULT* second_hresul,
	HRESULT(*first_func)(DECIMAL*, DECIMAL*, DECIMAL*),
	HRESULT(*second_func)(DECIMAL*, DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	printf("\n\nStarting Scenario %20s\n", scenario);

	printf("%s;;%s;;;%% time;speedup\n", first, second);
	printf("ticks;sec;ticks;sec;;;\n");

	for (int i = 0; i < iterations; ++i)
	{
		auto time1 = run_benchmark(lhs, lhs_count, rhs, rhs_count, first_target, first_hresult, VarDecMul);
		auto time2 = run_benchmark(lhs, lhs_count, rhs, rhs_count, second_target, second_hresul, VarDecMul_x64);

		double seconds1 = (double)time1 / (double)frequency.QuadPart;
		double seconds2 = (double)time2 / (double)frequency.QuadPart;

		double percent_of_original = (seconds2 / seconds1);
		double speedup  = (seconds1 - seconds2) / seconds2;

		printf("%I64u;%g;%I64u;%g;;%f%%;%f%%\n", time1, seconds1, time2, seconds2, percent_of_original * 100.0, speedup* 100.0);
	}
	printf("\n");

	int first_hres0 = 0, second_hres0 = 0;
	for (int i = 0; i < lhs_count*rhs_count; ++i)
	{
		first_hres0 += (first_hresult[i] == 0);
		second_hres0 += (second_hresul[i] == 0);
	}
		
	printf("%d out of %d results was successfull, expected %d, ratio is %f%%\n", second_hres0, lhs_count*rhs_count, first_hres0, 100.0 * (double)second_hres0 / (double)(lhs_count*rhs_count));

	CompareResult(first, second, lhs, lhs_count, rhs, rhs_count, first_target, first_hresult, second_target, second_hresul, lhs_count*rhs_count);
}

long long run_benchmark(DECIMAL* lhs, int lhs_count,
	DECIMAL* rhs, int rhs_count,
	DECIMAL* target, HRESULT *result, 
	HRESULT(*func)(DECIMAL*, DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);

	DECIMAL* destination = target;
	for (int i = 0; i < lhs_count; ++i)
		for(int j = 0; j < rhs_count; ++j)
			*result++ = func(lhs + i, rhs + j, destination++);
	QueryPerformanceCounter(&end);

	return end.QuadPart - start.QuadPart;
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
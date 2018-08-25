// coreclrtesting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#if 0
#include <ittnotify.h>
#pragma comment(lib, "libittnotify.lib")
#else
#define __itt_resume()
#define __itt_pause()
#endif

#include "decimal_calc.h"

//#include <vector>
#include <functional>
#include <oleauto.h>

#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_)
#include <emmintrin.h>
#endif

#define _CRT_RAND_S
#include <stdlib.h>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

// PAL RT Implementationsions
STDAPI VarDecMul_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecAdd_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecSub_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);
STDAPI VarDecDiv_PALRT(LPDECIMAL pdecL, LPDECIMAL pdecR, LPDECIMAL pdecRes);

using namespace std;


//#define SAVE_BASELINE
//#define LOAD_BASELINE

//#define NO_COMPARE
//#define NO_COMPARE_ONLY_BASELINE


//#define NO_VALIDATE
//#define COMPARE_OLEAUT
//#define COMPARE_CORECLR
#define COMPARE_DEFAULT 

#if defined(COMPARE_DEFAULT)
#ifdef _AMD64_
#define COMPARE_OLEAUT
#else
#define COMPARE_CORECLR
#endif
#endif 

#define TEST_MULTIPLY
//#define TEST_ADD
//#define TEST_SUB
//#define TEST_DIV
//
//#define TEST_32bit_with_0_scale
//#define TEST_32bit_with_scale
//#define TEST_64bit_with_scale_64bit_result
//#define TEST_64bit_with_0_scale_128bit_result
//#define TEST_64bit_with_scale_128bit_result
//#define TEST_96bit_with_scale_96bit_result_and_overflow
//#define TEST_96bit_with_scale_96bit_result_no_overflow
#define TEST_Bitpatterns_with_all_scales

#define VERBOSE_OUTPUT 1

#ifdef _TARGET_X86_
const char * platform = "new-x86";
#elif defined(_TARGET_AMD64_)
const char * platform = "new-x64";
#elif defined(_TARGET_ARM_)
const char * platform = "new-ARM";
#else
#error "MISSING _TARGET_XXX_ define" 
#endif


void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(STDAPICALLTYPE *baseline)(const DECIMAL *, const DECIMAL *, DECIMAL *),
	HRESULT(STDAPICALLTYPE *func)(const DECIMAL *, const DECIMAL *, DECIMAL *));
void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(STDAPICALLTYPE *baseline)(DECIMAL *, DECIMAL *, DECIMAL *),
	HRESULT(STDAPICALLTYPE *func)(DECIMAL *, DECIMAL *, DECIMAL *));

void save_or_load_baseline(const char *const &scenario, 
	const char *const &baseline, 
	const std::vector<DECIMAL> & lhs, const std::vector<DECIMAL> & rhs, std::vector<DECIMAL> & first_target, std::vector<HRESULT> & first_result);

void InitializeTestData(std::vector<DECIMAL>& numbers, int bytes);
void test_round_to_nearest();

void AdditionalTests(const int &iterations);

void TestMultiply(DECIMAL a, DECIMAL b)
{
	DECIMAL prod, prod2;
	HRESULT res1 = VarDecMul(&a, &b, &prod);
	HRESULT res2 = DecimalMul(&a, &b, &prod2);

	printf("Original DecMul %lu %lu %lu sign %i scale %i\n (RETURN %li)", prod.Hi32, prod.Mid32, prod.Lo32, (int)prod.sign, (int)prod.scale, res1);
	printf("Ny DecMul %lu %lu %lu sign %i scale %i (RETURN %li)\n", prod2.Hi32, prod2.Mid32, prod2.Lo32, (int)prod2.sign, (int)prod2.scale, res2);
}


BYTE random_scale(int min, int max)
{
	return (BYTE)((rand() % (max - min)) + min);
}


// Runs a single benchmark pass and records the timing when calling func
// with for all combination ov values from lhs and rhs.
long long run_benchmark(
	const std::vector<DECIMAL>& lhs,
	const std::vector<DECIMAL>& rhs,
	std::vector<DECIMAL>& target, std::vector<HRESULT>& result,
	HRESULT(STDAPICALLTYPE *func)(const DECIMAL*, const DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);

	auto destination = target.begin();
	auto res = result.begin();
	for (size_t i = 0; i < lhs.size(); ++i)
		for (size_t j = 0; j < rhs.size(); ++j)
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


	size_t result_idx = (size_t)0 - 1;
	for (size_t i = 0; i < lhs.size(); ++i)
		for (size_t j = 0; j < rhs.size(); ++j)
		{
			++result_idx;

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
						printf("[%i]x[%i]  -- (%i) %lu %I64u / 10^%i  * (%i) %lu %I64u / 10^%i: \n",
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

#ifdef VERBOSE_OUTPUT
	printf("%zi equal results with different scale (FALSE POSITIVE)\n", scale_diff);
#endif
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
	HRESULT(STDAPICALLTYPE *first_func)(const DECIMAL*, const DECIMAL*, DECIMAL*),
	HRESULT(STDAPICALLTYPE *second_func)(const DECIMAL*, const DECIMAL*, DECIMAL*))
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	std::vector<long long> timings1(iterations);
	std::vector<long long> timings2(iterations);

	__itt_resume();
	for (int i = 0; i < iterations; ++i)
	{
#if defined(NO_COMPARE) || defined(LOAD_BASELINE)
		auto time2 = run_benchmark(lhs, rhs, second_target, second_result, second_func);
		auto time1 = time2;
#elif defined(NO_COMPARE_ONLY_BASELINE)
		auto time2 = run_benchmark(lhs, rhs, first_target, first_result, first_func);;
		auto time1 = time2;
#else
		auto time1 = run_benchmark(lhs, rhs, first_target, first_result, first_func);
		auto time2 = run_benchmark(lhs, rhs, second_target, second_result, second_func);
#endif
		timings1[i] = time1;
		timings2[i] = time2;
	}
	__itt_pause();

	for (int i = 0; i < iterations; ++i)
	{
		auto time1 = timings1[i];
		auto time2 = timings2[i];
		double seconds1 = (double)time1 / (double)frequency.QuadPart;
		double seconds2 = (double)time2 / (double)frequency.QuadPart;

		double percent_of_original = (seconds2 / seconds1);
		double speedup = (seconds1 - seconds2) / seconds2;

		printf("%s;%s vs %s; %I64u;%g;%I64u;%g;;%f%%;%f%%\n", scenario, second, first, time1, seconds1, time2, seconds2, percent_of_original * 100.0, speedup* 100.0);
	}

#if defined(SAVE_BASELINE) || defined(LOAD_BASELINE)
	save_or_load_baseline(scenario, first, lhs, rhs, first_target, first_result);
#endif

#ifdef VERBOSE_OUTPUT
	printf("\n");
#endif

#if !(defined(NO_VALIDATE) || defined(NO_COMPARE) || defined(NO_COMPARE_ONLY_BASELINE))
	int first_hres0 = (int)std::count(first_result.begin(), first_result.end(), 0);
	int second_hres0 = (int)std::count(second_result.begin(), second_result.end(), 0);
	//cout << second_hres0 << " out of " << first_target.size() << " resuts was successfull, expected " << first_hres0 << " => ratio " << 100.0 * (double)second_hres0 / (double)(first_target.size()) << "%" << endl;


#ifndef VERBOSE_OUTPUT
	if (first_hres0 != second_hres0)
#endif
		printf("%d out of %zi results was successfull, expected %d, ratio is %f%%\n", second_hres0, first_target.size(), first_hres0, 100.0 * (double)second_hres0 / (double)(first_target.size()));

	CompareResult(lhs, rhs, first_target, first_result, second_target, second_result);
#endif // !NO_COMPARE
}

#include "miniz.h"

template <typename T>
void WriteCompressed(const std::vector<T> & source, const std::string &filename)
{
	ofstream file(filename, ios::binary);

#if COMPRESSION
#if VERBOSE_OUTPUT
	printf("compressing output\n");
#endif

	const mz_ulong bytes = (mz_ulong)source.size() * sizeof(T);
	const unsigned char * data = (const unsigned char *)source.data();

	vector<byte> buffer;
	buffer.resize(bytes);

	mz_ulong bytes_written = (mz_ulong)buffer.size();
	int status = mz_compress2(buffer.data(), &bytes_written, data, bytes, 8);
	assert(status == MZ_OK);

#if VERBOSE_OUTPUT
	printf("Writing to '%s' (compression status %d)\n", filename.c_str(), status);
#endif
	file.write((const char*)buffer.data(), bytes_written);
#else
	auto & buffer = source;
	

	if constexpr (is_same<T,DECIMAL>())
	{
		//std::for_each((const DECIMAL*)source.data(), ((const DECIMAL*)source.data())+source.size(),
		/*std::for_each(source.data(), source.data() + source.size(),
			[&file](const DECIMAL &d)*/
		for(const DECIMAL & d : source)
		{
			file.write((const char*)&d.Lo32, sizeof(d.Lo32));
			file.write((const char*)&d.Mid32, sizeof(d.Mid32));
			file.write((const char*)&d.Hi32, sizeof(d.Hi32));
			char scaleSign = (d.scale | (DECIMAL_NEG & d.sign));
			file.write((const char*)&scaleSign, 1);
		}
	}
	else
	{
		const auto bytes_written = source.size() * sizeof(T);
		file.write((const char*)buffer.data(), bytes_written);
	}
#endif

	file.close();

#if VERBOSE_OUTPUT
	printf("Write completed\n");
#endif
}

template <typename T>
void ReadCompressed(std::vector<T> & destination, const std::string &filename)
{
	ifstream fin(filename, ios::binary);
	auto start = fin.tellg();
	fin.seekg(0, ios::end);
	auto bytes = fin.tellg() - start;

	fin.seekg(0, ios::beg);

#if COMPRESSION
	vector<byte> buffer;
	buffer.resize(bytes);

	// Read file
	fin.read((char*)buffer.data(), bytes);
	mz_ulong bytes_written = (mz_ulong)(destination.size() * sizeof(T));
	int status = mz_uncompress((unsigned char*)destination.data(), &bytes_written, buffer.data(), bytes);

	assert(status == MZ_OK);
	assert(bytes_written == destination.size() * sizeof(T));

#else
	auto & buffer = source;
	fin.read((char*)buffer.data(), bytes);
#endif
}

/*
void ReadCompressed(const size_t bytes, const unsigned char * data, const std::string &filename)
{
	ifstream fin(filename);
	
	z_stream strm;
	memset(&strm, 0, sizeof(strm));

	strm.avail_in = bytes;
	strm.next_in = data;

	vector<byte> buffer;
	buffer.resize(strm.avail_in);
	strm.next_out = buffer.data();
	strm.avail_out = buffer.size();

	deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	deflate(&strm, Z_FINISH);
	deflateEnd(&strm);

	ofstream file(filename, ios::binary);
	file.write((const char*)buffer.data(), strm.total_out);
}*/


// Handles persisting or restoring expected result
void save_or_load_baseline(const char *const &scenario, const char *const &baseline, const std::vector<DECIMAL> & lhs, const std::vector<DECIMAL> & rhs, std::vector<DECIMAL> & first_target, std::vector<HRESULT> & first_result)
{
	std::ostringstream filename;
	filename << "baselines/" << baseline << "_" << scenario << "_" << lhs.size() << "x" << rhs.size();
	std::string values_file = filename.str() + "_values.dat";
	std::string result_file = filename.str() + "_hresult.dat";

#if COMPRESSION
	values_file = values_file + "gzip";
	result_file = result_file + "gzip";
#endif

#if defined(SAVE_BASELINE)
	WriteCompressed(first_target, values_file);
	WriteCompressed(first_result, result_file);
#endif

#if defined (LOAD_BASELINE)
	ReadCompressed(first_target, values_file);
	ReadCompressed(first_result, result_file);
#endif
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

	uint64_t elapsed = end.QuadPart - start.QuadPart;
	QueryPerformanceFrequency(&frequency);

	printf("%s;%I64u;%f\n", name, elapsed, (double)elapsed / (double)frequency.QuadPart);

	return elapsed;
}


void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(STDAPICALLTYPE *baseline)(DECIMAL *, DECIMAL *, DECIMAL *),
	HRESULT(STDAPICALLTYPE *func)(DECIMAL *, DECIMAL *, DECIMAL *))
{
	run_benchmarks(iterations, elements, bytes,
		baseline_name, func_name,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))baseline,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))func);
}

void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(STDAPICALLTYPE *baseline)(DECIMAL *, DECIMAL *, DECIMAL *),
	HRESULT(STDAPICALLTYPE *func)(const DECIMAL *, const DECIMAL *, DECIMAL *))
{
	run_benchmarks(iterations, elements, bytes,
		baseline_name, func_name,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))baseline,
		func);
}

void run_benchmarks(int iterations, int elements, int bytes,
	const char *const baseline_name, const char *const func_name,
	HRESULT(STDAPICALLTYPE *baseline)(const DECIMAL *, const DECIMAL *, DECIMAL *),
	HRESULT(STDAPICALLTYPE *func)(const DECIMAL *, const DECIMAL *, DECIMAL *)
)
{
	std::vector<DECIMAL> numbers(elements);
	std::vector<DECIMAL> targetA(elements*max(elements, 435));
	std::vector<DECIMAL> targetC(elements*max(elements, 435));
	std::vector<HRESULT> hresultA(elements*max(elements, 435));
	std::vector<HRESULT> hresultC(elements*max(elements, 435));
	InitializeTestData(numbers, bytes);

	// Change scale
	const int minScale = 10;
	const int maxScale = DEC_SCALE_MAX - 2;

	printf("%s;;%s;;;%% time;speedup;elements=%d\n", baseline_name, func_name, elements);
	printf("scenario;ticks;sec;ticks;sec;;;\n");

#ifdef TEST_32bit_with_0_scale
	compare_benchmark("32bit x 32bit with scale 0", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif


#ifdef TEST_32bit_with_scale
	for (size_t i = 0; i < numbers.size(); ++i)
		numbers[i].scale = random_scale(minScale, maxScale);

	compare_benchmark("32bit x 32bit with scale in range [10,26]", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

	//#ifdef TEST_64bit_with_scale_64bit_result
#ifdef TEST_64bit_with_scale_64bit_result
	for (size_t i = 0; i < numbers.size(); ++i)
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
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = 0;
	}

	compare_benchmark("64bit values -> 65-128 bit results and no scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_64bit_with_scale_128bit_result
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		numbers[i].Mid32 = RotateLeft32(numbers[i].Lo32, 14);
		numbers[i].scale = random_scale(2, 9);
	}

	compare_benchmark("64bit values -> 65-128 bit results and scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_and_overflow
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		numbers[i].scale = rand() % 5;
		numbers[i].Hi32 = numbers[i].Mid32 = numbers[i].Lo32;
	}

	compare_benchmark("96bit values with high overflow probablility", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#ifdef TEST_96bit_with_scale_96bit_result_no_overflow
	for (size_t i = 0; i < numbers.size(); ++i)
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
	const int bits = bytes * 8;

	const DWORD64 allBits = (0xffffffffffffffffui64) >> (64 - bits);
	const DWORD64 noBits = (0x0000000000000000ui64);

	assert(numbers.size() >= 2 + 3 * bits + 2 * bytes);

	// Genereate 0x0000000000000000, 0x000000000000000f, 0x00000000000000ff, to 0xffffffffffffffffu
	size_t dest = 0;
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
	for (; dest < numbers.size(); ++dest)
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

void WriteDecimal(FILE* file, const DECIMAL& dec)
{
	fprintf(file, "%I64uULL,%u,%u,%u", dec.Lo64, dec.Hi32, dec.scale, dec.sign ? 1 : 0);
}

typedef HRESULT (STDAPICALLTYPE *DecimalOp)(const DECIMAL* pdecL, const DECIMAL *pdecR, DECIMAL * __restrict res);

void WriteOp(FILE* file, const DECIMAL& lhs, const DECIMAL& rhs, const char* name, DecimalOp func)
{
	DECIMAL result;
	HRESULT return_code = func(&lhs, &rhs, &result);

	fprintf(file, "ValidateResult(%s,", name);
	WriteDecimal(file, lhs);
	fprintf(file, ",\n");
	WriteDecimal(file, rhs);
	fprintf(file, ",\n%ul,\n", return_code);
	if (return_code == 0)
		WriteDecimal(file, result);
	else
		fprintf(file, "0,0,0,0");
	fprintf(file, ");\n");
}

void WriteMul(FILE* file, const DECIMAL& lhs, const DECIMAL& rhs)
{
	WriteOp(file, lhs, rhs, "DecimalMul", DecimalMul);
}


void AdditionalTests(const int &iterations)
{
	// Additional tests
	vector<DECIMAL> numbers;
	numbers.reserve(2 * (DEC_SCALE_MAX + 1) * 96);
	BYTE signs[] = { 0, DECIMAL_NEG };
	const int bitsteps = 3;
	const uint32_t bitmask = (1 << (bitsteps + 1)) - 1;

	for (BYTE sign : signs)
	{
		DECIMAL current;
		current.sign = sign;
		for (size_t scale = 0; scale <= DEC_SCALE_MAX; scale+=3)
		{
			current.scale = (BYTE)scale;
			current.Lo64 = MAXDWORD64;
			current.Hi32 = 0;

			//for (int bit = 0; bit < 96; ++bit)
			for (int bit = 0; bit < 32; bit += bitsteps)
			{
				if (current.Lo64 == MAXDWORD64)
					current.Hi32 = (current.Hi32 << bitsteps) | bitmask;
				current.Lo64 = (current.Lo64 << bitsteps) | bitmask;
				numbers.push_back(current);
			}
		}
	}
	vector<DECIMAL> expected(numbers.size()*numbers.size()), actual(numbers.size()*numbers.size());
	vector<HRESULT> expected_res(numbers.size()*numbers.size()), actual_res(numbers.size()*numbers.size());


	FILE* mul = fopen("compare_mul.cpp", "w");
	FILE* add = fopen("compare_add.cpp", "w");
	FILE* div = fopen("compare_div.cpp", "w");
	for (size_t i = 0; i < numbers.size(); i++)
	{
		for (size_t j =i; j < numbers.size(); j += 3)
		{
			WriteOp(mul, numbers[i], numbers[j], "DecimalMul", DecimalMul);
			WriteOp(add, numbers[i], numbers[j], "DecimalAdd", DecimalAdd);
			WriteOp(div, numbers[i], numbers[j], "DecimalDiv", DecimalDiv);
		}
	}
	fclose(mul);
	fclose(add);
	fclose(div);

#ifdef TEST_MULTIPLY
#ifdef COMPARE_OLEAUT
	compare_benchmark("VarDecMul all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecMul,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalMul
	);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	compare_benchmark("VarDecMul all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecMul_PALRT,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalMul
	);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_ADD
#ifdef COMPARE_OLEAUT
	compare_benchmark("VarDecAdd all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecAdd,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalAdd
	);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	compare_benchmark("VarDecAdd all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecAdd_PALRT,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalAdd
	);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_SUB
#ifdef COMPARE_OLEAUT
	compare_benchmark("VarDecSub all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecSub,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalSub
	);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	compare_benchmark("VarDecSub all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecSub_PALRT,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalSub
	);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_DIV
#ifdef COMPARE_OLEAUT
	compare_benchmark("VarDecDiv all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecDiv,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalDiv
	);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	compare_benchmark("VarDecDiv all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
		numbers, expected, expected_res, actual, actual_res,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecDiv_PALRT,
		(HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))DecimalDiv
	);
#endif // COMPARE_CORECLR
#endif

}

struct DECOVFL2
{
	DWORD64 Hi;
	DWORD32 Lo;
};

static const DECOVFL2 PowerOvfl[] = {
	{ ULLONG_MAX, ULONG_MAX }, // 
	{ 1844674407370955161uI64, 2576980377u }, // 10^1 0,6
	{ 184467440737095516uI64, 687194767u }, // 10^2 0,16
	{ 18446744073709551uI64, 2645699854u }, // 10^3 0,616
	{ 1844674407370955uI64, 694066715u }, // 10^4 0,1616
	{ 184467440737095uI64, 2216890319u }, // 10^5 0,51616
	{ 18446744073709uI64, 2369172679u }, // 10^6 0,551616
	{ 1844674407370uI64, 4102387834u }, // 10^7 0,9551616
	{ 184467440737uI64, 410238783u }, // 10^8 0,09551616
	{ 18446744073uI64, 3047500985u }, // 10^9 0,709551616 
	{ 1844674407uI64, 1593240287u }, // 10^10 0,3709551616
	{ 184467440uI64, 3165801135u }, // 10^11 0,73709551616
	{ 18446744uI64, 316580113u }, // 10^12 0,073709551616
	{ 1844674uI64, 1749644929u }, // 10^13 0,4073709551616
	{ 184467uI64, 1892951411u }, // 10^14 0,44073709551616
	{ 18446uI64, 3195772248u }, // 10^15 0,744073709551616
	{ 1844uI64, 2896557602u }, // 10^16 0,674407370955162
	{ 184uI64, 2007642678u }, // 10^17 0,467440737095516
	{ 18uI64, 1918751186u }, // 10^18 0,446744073709552
	{ 1uI64, 3627848955u }, // 10^19 0,844674407370955
};


int SearchScale(ULONG ulResHi, ULONG ulResMid, ULONG ulResLo, int iScale); // coreclr_impl
int SearchScale32(const ULONG* rgulQuo, int iScale)
{
	return SearchScale(rgulQuo[2], rgulQuo[1], rgulQuo[0], iScale);
}

int SearchScale32(const ULONG* rgulQuo, int iScale);
void InitializeDecimal(DECIMAL &expected, const uint64_t &expected_low64, const uint32_t &expected_hi32, const uint8_t &expected_scale, bool expected_sign);
int SearchScale64(const uint32_t(&rgulQuo)[4], int iScale);
ULONG IncreaseScale(ULONG *rgulNum, ULONG ulPwr);

void CompareScaleResult()
{
	ULONG input[4];
	ULONG copy[4];
	DWORD64 *pHi64 = (DWORD64*)&input[1];
	DWORD32 *pLo32 = (DWORD32*)&input[0];
	std::vector<int> result;


	for (int i = 0; i <= 19; ++i)
	{
		for (int hi = -1000; hi <= 1000; ++hi)
		{
			for (int lo = -100; lo <= 100; ++lo)
				//for (int lo = 0; lo <= 0; ++lo)
			{
				*pHi64 = PowerOvfl[i].Hi + hi;
				*pLo32 = PowerOvfl[i].Lo + lo;

				//for (int scale = -10; scale <= 30; ++scale)
				for (int scale = -DEC_SCALE_MAX; scale <= DEC_SCALE_MAX; ++scale)
				{
					//for (int func = 0; func < all_functions.size(); ++func)
					const int func = 1;
					{
						auto res = SearchScale64((const uint32_t(&)[4])input, scale);
						auto control = SearchScale32(input, scale);


						if (res != control)
						{
							// New function can detect overflow faster
							if (control == 9)
							{
								memcpy(copy, input, sizeof(copy));
								copy[3] = 0;
								IncreaseScale(copy, 1000000000U); // 10^9
								int extraScale = 9;
								control = SearchScale32(copy, scale + extraScale);
								if (control == 9)
								{
									IncreaseScale(copy, 10); // 10^9
									extraScale += 1;
									control = SearchScale32(copy, scale + extraScale);
								}

								// res != -1
								if ((res == -1 && control == -1) || (res == control + extraScale)) //  || (control + extraScale == res)
									continue;
								else
								{
									cout << "FAILED extended validation" << endl;
								}
							}

							cout << "FAILED control" << endl;
							cin.get();
							exit(-1);
						}

						//cout << "Function " << func << " item " << i << " => " << res;
						/*
						if (func != 0)
						{
							bool ok = res == all_results[0].at(result.size() - 1);
							//cout << (ok ? " OK" : " FAIL");
							assert(ok);
						}
						*/
						//cout << endl;
					}
				}
			}
		}
	}
}

#ifdef SIGN_TEST
int singTest(int i1, int i2)
{
	__asm
	{
		mov eax, i1
		or ebx, 0xffffffff
		mov ecx, i2
		cmp eax, ecx
		setg al; //(d = (i1 > i2), dvs 1 om(i1 > i2) annars 0)
		cmovl eax, ebx
	}
}

__declspec(noinline)
void signTest(int &a, int &b, int &c)
{
	int i1, i2;
	cout << "Enter 2 int" << endl;
	cin >> i1 >> i2;

	a = (i1 < i2) ? -1 : (i1 > i2) ? 1 : 0;

	printf("a is %d", a);
	c = singTest(i1, i2);
	printf("c is %d", c);
	unsigned int u1, u2;
	cout << "Enter 2 uint" << endl;
	cin >> u1 >> u2;

	b = (u1 < u2) ? -1 : (u1 > u2) ? 1 : 0;
	printf("b is %d", b);
}
#endif

void InitializeDecimal(DECIMAL &dec, const uint64_t &low64, const uint32_t &hi32, const uint8_t &scale, BYTE sign)
{
	dec.Lo64 = low64;
	dec.Hi32 = hi32;
	dec.scale = scale;
	dec.sign = (sign != 0) ? DECIMAL_NEG : 0;
}

void ValidateResult( HRESULT (*func)(const DECIMAL*, const DECIMAL*, DECIMAL*),
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

	if (actual.Lo64 != expected.Lo64
		|| actual.Hi32 != expected.Hi32
		|| actual.scale != expected.scale
		|| actual.sign != expected.sign
		|| actual_result != expected_result)
	{
		printf("(%i) %lu %I64u / 10^%i  * (%i) %lu %I64u / 10^%i: \n",
			lhs.sign, lhs.Hi32, lhs.Lo64, lhs.scale,
			rhs.sign, rhs.Hi32, rhs.Lo64, rhs.scale);

		if (expected_result != actual_result)
			printf(" RESULT expected %li actual %li\n", expected_result, actual_result);
		else
			printf(" PRODUCT expected (%i) %lu %I64u / 10^%i but got (%i) %lu %I64u / 10^%i\n",
				expected.sign, expected.Hi32, expected.Lo64, expected.scale,
				actual.sign, actual.Hi32, actual.Lo64, actual.scale
			);
	}
}

int __cdecl main()
{
	__itt_pause();

	// Use system formatting
	setlocale(LC_ALL, "");

#ifdef SIGN_TEST
	int a1, b1, c1;
	signTest(a1, b1, c1);
	printf("a is %d", a1);
	printf("b is %d", b1);
#endif

#if 0
	DECIMAL a, b;
	DECIMAL expected, actual;

	printf("reserved %ld, scale %ld, sign %ld, scalesign %ld, hi32 %ld, mid 32 %ld, lo 32 %ld, lo64 %ld\n",
		FIELD_OFFSET(DECIMAL, wReserved),
		FIELD_OFFSET(DECIMAL, scale),
		FIELD_OFFSET(DECIMAL, sign),
		FIELD_OFFSET(DECIMAL, signscale),
		FIELD_OFFSET(DECIMAL, Hi32),
		FIELD_OFFSET(DECIMAL, Mid32),
		FIELD_OFFSET(DECIMAL, Lo32),
		FIELD_OFFSET(DECIMAL, Lo64)
	);

	/*
	[0]x[9]  -- (0) 0 18446744073709551615 / 10^3  * (0) 0 18446251492500307960 / 10^6:
 PRODUCT expected (0) 542115562 5349999624952008190 / 10^25 but got (0) 0 0 / 10^0
	*/
	//a.Hi32 = 1;
	//a.Lo64 = 18446744073709551615;
	//a.scale = 0;
	//b.Hi32 = 1;
	//b.Lo64 = 18446744073709551615;
	//b.scale = 19;
	a.Hi32 = 2147483647;
	a.Lo64 = 9223372034707292159;
	a.scale = 14;
	a.sign = 0;
	b.Hi32 = 2851480405;
	//b.Mid32 = 23;
	b.Lo64 = 12247015087511315285;
	b.scale = 25;
	b.sign = 0; // DECIMAL_NEG;


	VarDecMul_PALRT(&a, &b, &expected);
	DecimalMul(&a, &b, &actual);

	assert(VarDecCmp(&actual, &expected) == VARCMP_EQ);
	assert(actual.Hi32 == expected.Hi32);
	assert(actual.Lo64 == expected.Lo64);
	assert(actual.signscale == expected.signscale);
	//test_round_to_nearest();

#endif // ! NO_COMPARE

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS /* or ABOVE_NORMAL_PRIORITY_CLASS */);

	//run_benchmarks(30000, 4, 5);
#ifdef DEBUG
	const int iterations = 2;
	const int elements = 106;
#else
	const int iterations = 2;
	const int elements = 4000;
	//const int elements = 4000;
#endif
	const int bytes = 4;

	static_assert(elements >= 106, "Must fit special elements");

#ifdef TEST_MULTIPLY
#ifdef COMPARE_OLEAUT
	run_benchmarks(iterations, elements, bytes, "oleauto-VarDecMul", platform, VarDecMul, DecimalMul);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	run_benchmarks(iterations, elements, bytes, "palrt-VarDecMul", platform, VarDecMul_PALRT, DecimalMul);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_DIV
#ifdef COMPARE_OLEAUT
	run_benchmarks(iterations, elements, bytes, "oleauto-VarDecDiv", platform, VarDecDiv, DecimalDiv);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	run_benchmarks(iterations, elements, bytes, "palrt-VarDecDiv", platform, VarDecDiv_PALRT, DecimalDiv);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_ADD
#ifdef COMPARE_OLEAUT
	run_benchmarks(iterations, elements, bytes, "oleauto-VarDecAdd", platform, VarDecAdd, DecimalAdd);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	run_benchmarks(iterations, elements, bytes, "palrt-VarDecAdd", platform, VarDecAdd_PALRT, DecimalAdd);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_SUB
#ifdef COMPARE_OLEAUT
	run_benchmarks(iterations, elements, bytes, "oleauto-VarDecSub", platform, VarDecSub, DecimalSub);
#endif // COMPARE_OLEAUT
#ifdef COMPARE_CORECLR
	run_benchmarks(iterations, elements, bytes, "palrt-VarDecSub", platform, VarDecSub_PALRT, DecimalSub);
#endif // COMPARE_CORECLR
#endif

#ifdef TEST_Bitpatterns_with_all_scales
	AdditionalTests(iterations);
#endif

	return 0;
}
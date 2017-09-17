#pragma once

// Helper calculation functions
//
// * low32(uint64_t &value) - access lower 32 bits
// * hi32(uint64_t &value) - access upper 32 bits
// 
// * AddCarry32 - 
// * AddCarry64
// * SubBorrow32
// * SubBorrow64

// * Mul32By32
// * Mul64By32 - 
// * Mul64By64 - Same as uint64_t _umul128(uint64_t lhs, uint64_t rhs, uint64_t * _HighProduct)


// * DivMod64By32
// * DivMod64By32InPlace
// * DivMod64By64
// * DivMod128By64 - only AMD64, HAS_DIVMOD128BY64 gets defined when availible
// * DivMod128By64InPlace  - only AMD64, HAS_DIVMOD128BY64 gets defined when availible

// 
// * BitScanMsb32
// * BitScanMsb64

// 
// * ShiftLeft128
//

#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#else // CLANG? 
#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_)
#include <x86intrin.h>
#endif
#endif


inline uint32_t & low32(uint64_t &value) { return *(uint32_t*)&((SPLIT64*)&value)->u.Lo; }
inline const uint32_t & low32(const uint64_t &value) {
	return *(const uint32_t*)&((SPLIT64*)&value)->u.Lo;
}

inline uint32_t & hi32(uint64_t &value) { return *(uint32_t*)&((SPLIT64*)&value)->u.Hi; }
inline const uint32_t & hi32(const uint64_t &value) {
	return *(const uint32_t*)&((SPLIT64*)&value)->u.Hi;
}

// TODO: Define addcaryy etc for ARM 
// CLANG builtins availible 
/*
unsigned char      __builtin_addcb (unsigned char x, unsigned char y, unsigned char carryin, unsigned char *carryout);
unsigned short     __builtin_addcs (unsigned short x, unsigned short y, unsigned short carryin, unsigned short *carryout);
unsigned           __builtin_addc  (unsigned x, unsigned y, unsigned carryin, unsigned *carryout);
unsigned long      __builtin_addcl (unsigned long x, unsigned long y, unsigned long carryin, unsigned long *carryout);
unsigned long long __builtin_addcll(unsigned long long x, unsigned long long y, unsigned long long carryin, unsigned long long *carryout);
unsigned char      __builtin_subcb (unsigned char x, unsigned char y, unsigned char carryin, unsigned char *carryout);
unsigned short     __builtin_subcs (unsigned short x, unsigned short y, unsigned short carryin, unsigned short *carryout);
unsigned           __builtin_subc  (unsigned x, unsigned y, unsigned carryin, unsigned *carryout);
unsigned long      __builtin_subcl (unsigned long x, unsigned long y, unsigned long carryin, unsigned long *carryout);
unsigned long long __builtin_subcll(unsigned long long x, unsigned long long y, unsigned long long carryin, unsigned long long *carryout);
*/

#if defined(_TARGET_X86_)  || defined(_TARGET_AMD64_)
#define AddCarry32 _addcarry_u32
#define SubBorrow32 _subborrow_u32
#else
inline unsigned char AddCarry32(unsigned char carry, uint32_t lhs, uint32_t rhs, uint32_t *pRes)
{
	*pRes = lhs + rhs;

	// check for overflow
	unsigned char carry_out = (*pRes < lhs) ? 1 : 0;
	if (carry)
	{
		*pRes += 1;

		// Can use or + xor instead
		if (*pRes == 0)
			carry_out = 1;
	}
	return carry_out;
}

inline unsigned char SubBorrow32(unsigned char carry, uint32_t lhs, uint32_t rhs, uint32_t *pRes)
{
	*pRes = lhs - rhs;

	// check for overflow
	unsigned char carry_out = (*pRes > lhs) ? 1 : 0;
	if (carry)
	{
		if (*pRes == 0)
			carry_out = 1;

		*pRes -= 1;
	}
	return carry_out;
}
#endif

#if defined(_TARGET_AMD64_)

#if defined(__GNUC__) || defined(__GNUC__)
#define AddCarry64(carry, lhs,rhs, pRes) _addcarry_u64(carry, lhs,rhs, (unsigned long long int *)pRes)
#define SubBorrow64(carry, lhs,rhs, pRes) _subborrow_u64(carry, lhs,rhs,  (unsigned long long int *)pRes)
#else
#define AddCarry64 _addcarry_u64
#define SubBorrow64 _subborrow_u64
#endif
#else
inline unsigned char AddCarry64(unsigned char carry, uint64_t lhs, uint64_t rhs, uint64_t *pRes)
{
	carry = AddCarry32(carry, low32(lhs), low32(rhs), &low32(*pRes));
	return AddCarry32(carry, hi32(lhs), hi32(rhs), &hi32(*pRes));
}

inline unsigned char SubBorrow64(unsigned char carry, uint64_t lhs, uint64_t rhs, uint64_t *pRes)
{
	carry = SubBorrow32(carry, low32(lhs), low32(rhs), &low32(*pRes));
	return SubBorrow32(carry, hi32(lhs), hi32(rhs), &hi32(*pRes));
}
#endif

// -------------------------- MULTIPLY ----------------------

inline uint64_t Mul32By32(uint32_t lhs, uint32_t rhs) { return UInt32x32To64(lhs, rhs); }


// Performs multiplications of 2 64bit values, returns lower 64bit and store upper 64bit in _HighProduct
inline uint64_t __fastcall Mul64By32(uint64_t lhs, uint32_t rhs, uint32_t * _HighProduct)
{
#ifdef _TARGET_AMD64_
	uint64_t temp;
	auto res = _umul128(lhs, rhs, &temp);
	*_HighProduct = (uint32_t)temp;
	return res;
#else
	uint64_t lowRes = UInt32x32To64(low32(lhs), rhs); // quo * lo divisor
	uint64_t hiRes = UInt32x32To64(hi32(lhs), rhs); // quo * mid divisor

	hiRes += hi32(lowRes);

	hi32(lowRes) = low32(hiRes);
	*_HighProduct = hi32(hiRes);

	return lowRes;
#endif
}

#if defined(_TARGET_AMD64_) && defined(_MSC_VER)
#define Mul64By64 _umul128
#elif defined(_TARGET_AMD64_)
// CLANG/GCC Does not contain umul instrinct, but has __uint128 datatype
inline uint64_t Mul64By64(uint64_t lhs, uint64_t rhs, uint64_t * _HighProduct)
{
	__uint128_t res = ((__uint128_t)lhs) * ((__uint128_t)rhs);
	*_HighProduct = ((uint64_t*)res)[1];
	return (uint64_t)res;
}
#else
// Performs multiplications of 2 64bit values, returns lower 64bit and store upper 64bit in _HighProduct
inline uint64_t __fastcall Mul64By64(uint64_t lhs, uint64_t rhs, uint64_t * _HighProduct)
{
	uint64_t lowRes, sdltmp1, sdltmp2;
	uint64_t &hiRes = *_HighProduct;

	unsigned char carry = 0;

	lowRes = UInt32x32To64(low32(lhs), low32(rhs));
	if ((hi32(lhs) | hi32(rhs)) == 0)
	{
		*_HighProduct = 0;
		return lowRes;
	}

	sdltmp1 = UInt32x32To64(hi32(lhs), low32(rhs));
	carry = AddCarry32(0, low32(sdltmp1), hi32(lowRes), &hi32(lowRes));

	hiRes = UInt32x32To64(hi32(lhs), hi32(rhs));
	carry = AddCarry32(carry, hi32(sdltmp1), low32(hiRes), &low32(hiRes));
	AddCarry32(carry, hi32(hiRes), 0, &hi32(hiRes));

	sdltmp2 = UInt32x32To64(low32(lhs), hi32(rhs));
	carry = AddCarry32(0, low32(sdltmp2), hi32(lowRes), &hi32(lowRes));
	carry = AddCarry32(carry, hi32(sdltmp2), low32(hiRes), &low32(hiRes));
	AddCarry32(carry, hi32(hiRes), 0, &hi32(hiRes));

	return lowRes;
}
#endif

// --------------------------- DIVISION ---------------------
#if defined(__GNUC__) && (defined(_TARGET_X86_) || defined(_TARGET_AMD64_))
inline uint32_t DivMod64By32(uint32_t lo, uint32_t hi, uint32_t ulDen, uint32_t *remainder)
{
	uint32_t eax, edx;

	asm("divl %[den]"
		: "=d" (edx), "=a"(eax)  // results in edx, eax reg
		: [den] "rm" (ulDen), "d"(hi), "a"(lo) /* input - hi in edx, lo in eax , ulDen as either reg or mem */
		: ); // no clobbers

	*remainder = edx;
	return eax;
}

inline uint32_t DivMod64By32InPlace(uint32_t* pLow, uint32_t hi, uint32_t ulDen)
{
	uint32_t rem;
	*pLow = DivMod64By32(*pLow, hi, ulDen, &rem);
	return rem;
}
#elif defined(_MSC_VER) && defined(_TARGET_AMD64_) // _MSVC_ x64 does not support inline asm
extern "C" uint32_t DivMod64By32(uint32_t lo, uint32_t hi, uint32_t ulDen, _Out_ uint32_t *remainder);
extern "C" uint32_t DivMod64By32InPlace(uint32_t* pLow, uint32_t hi, uint32_t ulDen);
#else // not X86 or not _MSC_VER

#if defined(_MSC_VER) && defined(_TARGET_X86_)
_declspec(naked)
inline uint64_t __fastcall DivMod64By32_impl(uint32_t lo, uint32_t hi, uint32_t ulDen)
{
	UNREFERENCED_PARAMETER(lo);
	UNREFERENCED_PARAMETER(hi);
	UNREFERENCED_PARAMETER(ulDen);

	// DX:AX = DX:AX / r/m; resulting 
	_asm
	{
		mov eax, ecx; // lo is in ecx, edx is already hi
		div dword ptr[esp + 4]; // result in eax:edx following 64bit value return on x86
		ret 4;// ulDen (4 uint8_t on stack)
	}
}
#else
// no X86 asm support 
inline uint64_t DivMod64By32_impl(uint32_t lo, uint32_t hi, uint32_t ulDen)
{
	assert(hi < ulDen);
	if (hi == 0)
	{
		uint64_t res;
		hi32(res) = lo % ulDen;
		low32(res) = lo / ulDen;
		return res;
	}
	else
	{
		uint64_t divisor;
		hi32(divisor) = hi;
		hi32(divisor) = lo;

		uint64_t res;
		hi32(res) = static_cast<uint32_t>(divisor % ulDen);
		low32(res) = static_cast<uint32_t>(divisor / ulDen);
		return res;
	}
}
#endif
inline uint32_t DivMod64By32(uint32_t lo, uint32_t hi, uint32_t ulDen, _Out_ uint32_t *remainder)
{
	uint64_t temp = DivMod64By32_impl(lo, hi, ulDen);
	*remainder = hi32(temp);
	return low32(temp);
}

inline uint32_t DivMod64By32InPlace(uint32_t* pLow, uint32_t hi, uint32_t ulDen)
{
	auto temp = DivMod64By32_impl(*pLow, hi, ulDen);
	*pLow = low32(temp);
	return hi32(temp);
}
#endif

// Divides a 32bit number (ulNum) by 32bit value (ulDen)
// returns 32bit quotient with remainder in *pRemainder
// This results in a single div instruction on x86 platforms
inline uint32_t DivMod32By32(uint32_t ulNum, uint32_t ulDen, _Out_ uint32_t* pRemainder)
{
	auto mod = ulNum % ulDen;
	auto res = ulNum / ulDen;

	*pRemainder = mod;
	return res;
}

// Divides a 64bit number (ullNum) by 64bit value (ullDen)
// returns 64bit quotient with remainder in *pRemainder
// This results in a single div instruction on x64 platforms
inline uint64_t DivMod64By64(uint64_t ullNum, uint64_t ullDen, _Out_ uint64_t* pRemainder)
{
	auto mod = ullNum % ullDen;
	auto res = ullNum / ullDen;

	*pRemainder = mod;
	return res;
}

#ifdef _TARGET_AMD64_
#define HAS_DIVMOD128BY64

#if defined(__GNUC__)
inline uint64_t DivMod128By64(uint64_t lo, uint64_t hi, uint64_t ulDen, uint64_t *remainder)
{
	uint64_t rax, rdx;

	asm("divq %[den]"
		: "=d" (rdx), "=a"(rax)  // results in edx, eax reg
		: [den] "rm" (ulDen), "d"(hi), "a"(lo) /* input - hi in edx, lo in eax , ulDen as either reg or mem */
		: ); // no clobbers

	*remainder = rdx;
	return rax;
}

inline uint64_t DivMod128By64InPlace(uint64_t* pLow, uint64_t hi, uint64_t ulDen)
{
	uint64_t rem;
	*pLow = DivMod128By64(*pLow, hi, ulDen, &rem);
	return rem;
}
#else
// Performs division of a 128bit number with 
// requires that hi < ulDen in order to not loose precision
// so it is best to set it to 0 unless when chaining calls in which case 
// the remainder from a previus devide should be used
extern "C" uint64_t DivMod128By64(uint64_t low, uint64_t hi, uint64_t divisor, _Out_ uint64_t *remainder);
// Performs inplace division of a 128bit number with 
// requires that hi < ulDen in order to not loose precision
// so it is best to set it to 0 unless when chaining calls in which case 
// the remainder from a previus devide should be used
extern "C" uint64_t DivMod128By64InPlace(uint64_t* pLow, uint64_t hi, uint64_t ulDen);
#endif
#endif

// Returns index of most significant bit in mask if any bit is set, returns false if Mask is 0
inline bool BitScanMsb32(uint32_t *Index, uint32_t Mask) {
#ifdef _WIN32
	return BitScanReverse((DWORD*)Index, Mask);
#else
	// G++/Clang __builtin_clz count LSB as 1, and MSB as 0 while BitScanReverse works in other way around
	*Index = 31 - __builtin_clz(Mask);
	return (Mask != 0);
#endif
}

// Returns index of most significant bit in mask if any bit is set, returns false if Mask is 0
inline unsigned char BitScanMsb64(uint32_t * Index, uint64_t Mask)
{
#if defined(_WIN32) && defined(_TARGET_AMD64_)
	return BitScanReverse64((DWORD*)Index, Mask);
#elif defined(__GNUC__) || defined(__CLANG__)
	static_assert(sizeof(long) == 8, "assuming long is 64 bits");
	*Index = 63 - __builtin_clzl(Mask);
	return (Mask != 0);
#else
	unsigned char res = BitScanMsb32(Index, hi32(Mask));
	if (res)
	{
		*Index += 32;
	}
	else
	{
		res = BitScanMsb32(Index, low32(Mask));
	}
	return res;
#endif
}

#if defined (WIN32) && !defined (_TARGET_AMD64_)
inline uint64_t ShiftLeft128(uint64_t _LowPart, uint64_t _HighPart, unsigned char _Shift)
{
	return (_HighPart << _Shift) + (_LowPart >> (64 - _Shift));
}
#endif

// ClrClassLibrary.h

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <oleauto.h>

// Prototype X64 Implementations
STDAPI VarDecMul_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecAdd_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecSub_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);
STDAPI VarDecDiv_x64(DECIMAL* l, DECIMAL *r, DECIMAL *res);


using namespace System;

namespace ClrClassLibrary {

	public ref class Methods
	{
	public:
		static Decimal AddNative(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecAdd_x64((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal AddOle32(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecAdd((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal AddManaged(Decimal lhs, Decimal rhs)
		{
			return Decimal::Add(lhs, rhs);
		}

		static Decimal MulNative(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecMul_x64((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal MulOle32(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecMul((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal MulManaged(Decimal lhs, Decimal rhs)
		{
			return Decimal::Multiply(lhs, rhs);
		}

		static Decimal DivNative(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecDiv_x64((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal DivOle32(Decimal lhs, Decimal rhs)
		{
			Decimal res;
			VarDecDiv((DECIMAL*)&lhs, (DECIMAL*)&rhs, (DECIMAL*)&res);
			return res;
		}

		static Decimal DivManaged(Decimal lhs, Decimal rhs)
		{
			return Decimal::Divide(lhs, rhs);
		}

		int AbsManaged(int i)
		{
			return Math::Abs(i);
		}

		int AbsNative(int i)
		{
			return abs(i);
		}
	};
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class FastMod
    {

        [Benchmark]
        public void MultiplyNoFlags()
        {
            for (uint i = 0; i < 1000; i++)
                FastMod_bmi(i, 375, 49191317529892138ul);
        }

        [Benchmark]
        public void BigMul()
        {
            for (uint i = 0; i < 1000; i++)
                FastMod_BigMul(i, 375, 49191317529892138ul);
        }


        [Benchmark]
        public void BigMul_OLD()
        {
            for (uint i = 0; i < 1000; i++)
                FastMod_BigMulOld(i, 375, 49191317529892138ul);
        }

        [Benchmark]
        public void BigMul2_OldPR()
        {
            for (uint i = 0; i < 1000; i++)
                FastMod_BigMul2(i, 375, 49191317529892138ul);
        }

        [Benchmark(Baseline = true)]
        public void Old()
        {
            for (uint i = 0; i < 1000; i++)
                FastMod_old(i, 375, 49191317529892138ul);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static uint FastMod_old(uint value, uint divisor, ulong multiplier) =>
            (uint)(((((multiplier * value) >> 32) + 1) * divisor) >> 32);

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static uint FastMod_bmi(uint value, uint divisor, ulong multiplier) =>
            (uint)Bmi2.X64.MultiplyNoFlags(multiplier * value, divisor);

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static uint FastMod_BigMul(uint value, uint divisor, ulong multiplier) =>
            (uint)Math.BigMul(multiplier * value, divisor, out _);

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static unsafe uint FastMod_BigMulOld(uint value, uint divisor, ulong multiplier)
        {
            return (uint)BigMulx(multiplier * value, divisor, out _);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static unsafe uint FastMod_BigMul2(uint value, uint divisor, ulong multiplier)
        {
            return (uint)BigMul2(multiplier * value, divisor, out _);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe static ulong BigMulx(ulong a, ulong b, out ulong low)
        {
            ulong tmp;
            ulong res = Bmi2.X64.MultiplyNoFlags(a, b, &tmp);
            low = tmp;
            return res;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe static ulong BigMul2(ulong a, ulong b, out ulong low)
        {
            low = a * b;
            return Bmi2.X64.MultiplyNoFlags(a, b);
        }
    }
}

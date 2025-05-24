using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks.Scenarios
{
    public class BigMulTests
    {
        [Params(0x0123456789abcdef)]
        public ulong TestA { get; set; }

        [Params(0xdeadbeefdeadbeef)]
        public ulong TestB { get; set; }

        [Benchmark]
        public ulong BenchBigMulUnsigned()
        {
            ulong accLo = TestA;
            ulong accHi = TestB;
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            return accLo + accHi;
        }

        [Benchmark]
        public long BenchBigMulSigned()
        {
            long accLo = unchecked((long)TestA);
            long accHi = ((long)TestB);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            MathBigMulAcc(accLo, accHi, ref accHi, ref accLo);
            return accLo + accHi;
        }

        [Benchmark]
        public ulong BenchMultiplyNoFlags3Ards()
        {
            ulong accLo = TestA;
            ulong accHi = TestB;
            BmiMultiplyNoFlagsAcc(accLo, accHi, ref accHi, ref accLo);
            BmiMultiplyNoFlagsAcc(accLo, accHi, ref accHi, ref accLo);
            BmiMultiplyNoFlagsAcc(accLo, accHi, ref accHi, ref accLo);
            BmiMultiplyNoFlagsAcc(accLo, accHi, ref accHi, ref accLo);
            return accLo + accHi;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private unsafe void MathBigMulAcc(ulong a, ulong b, ref ulong accHi, ref ulong accLo)
        {
            ulong lo;
            ulong hi = Math.BigMul(a, b, out lo);
            accHi += hi;
            accLo += lo;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private unsafe void MathBigMulAcc(long a, long b, ref long accHi, ref long accLo)
        {
            long lo;
            long hi = Math.BigMul(a, b, out lo);
            accHi += hi;
            accLo += lo;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static unsafe void BmiMultiplyNoFlagsAcc(ulong a, ulong b, ref ulong accHi, ref ulong accLo)
        {
            ulong lo;
            ulong hi = Bmi2.X64.MultiplyNoFlags(a, b, &lo);
            accHi += hi;
            accLo += lo;
        }
    }
}

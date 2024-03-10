using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;

namespace Managed
{
    internal static class Math
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ulong BigMul(ulong a, ulong b, out ulong low)
            => System.Math.BigMul(a, b, out low);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ulong BigMul(ulong a, uint b, out ulong low)
        {
#if TARGET_64BIT
            return System.Math.BigMul((ulong)a, (ulong)b, out low);
#elif TARGET_32BIT
#if BIGENDIAN
            const int lowOffset = 1, highOffset = 0;
#else
            const int lowOffset = 0, highOffset = 1;
#endif
            Unsafe.SkipInit(out low);

            ulong tmp = BigMulx((uint)a, b);
            Unsafe.Add(ref Unsafe.As<ulong, uint>(ref low), lowOffset) = (uint)tmp;
            tmp >>= 32;
            tmp += BigMulx((uint)(a >> 32), b);
            Unsafe.Add(ref Unsafe.As<ulong, uint>(ref low), highOffset) = (uint)tmp;

            return (tmp >> 32);
#else
            
            uint al = (uint)a;
            uint ah = (uint)(a >> 32);

            ulong prodH = (((ulong)ah) * (ulong)b);
            ulong prodL = ((ulong)al) * (ulong)b;
            prodH += (prodL >> 32);

            low = ((prodH << 32) | (uint)prodL);
            return (prodH >> 32);
#endif
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ulong BigMul(uint a, ulong b, out ulong low)
            => BigMul(b, a, out low);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static ulong BigMul(uint a, uint b)
        {
            return (ulong)a * b;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe static ulong BigMulx(uint a, uint b)
        {
#if TARGET_32BIT
            if (Bmi2.IsSupported)
            {
                uint low;
                uint hi = Bmi2.MultiplyNoFlags(a, b, &low);
                return ((ulong)hi << 32) | low;
            }
#endif
            return (ulong)a * b;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static (ulong Quotient, ulong Remainder) DivRem(ulong left, ulong right)
            => System.Math.DivRem(left, right);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static (uint Quotient, uint Remainder) DivRem(uint left, uint right)
            => System.Math.DivRem(left, right);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static int Min(int left, int right)
            => System.Math.Min(left, right);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static double Round(double a)
            => System.Math.Round(a);
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
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
            return Math.BigMul((ulong)a, (ulong)b, out low);
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

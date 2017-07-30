using System;
using System.Runtime.CompilerServices;

namespace Benchmarks.ClrClassLibrary
{
    internal static class Methods
    {
        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static decimal AddManaged(decimal a, decimal b)
        {
            return Decimal.Add(a, b);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static decimal AddNative(decimal a, decimal b)
        {
            return DecimalDLL.Add(a, b);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static decimal AddOle32(decimal a, decimal b)
        {
            return Oleaut32.Add(a, b);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static CoreRT.Decimal AddCoreRTManaged(CoreRT.Decimal a2, CoreRT.Decimal b2)
        {
            return CoreRT.Decimal.Add(a2, b2);
        }

        internal static decimal MulManaged(decimal a, decimal b)
        {
            return System.Decimal.Multiply(a, b);
        }

        internal static decimal MulNative(decimal a, decimal b)
        {
            return DecimalDLL.Mul(a, b);
        }

        internal static decimal MulOle32(decimal a, decimal b)
        {
            return Oleaut32.Mul(a, b);
        }

        internal static decimal DivManaged(decimal a, decimal b)
        {
            return Decimal.Divide(a, b);
        }
        internal static CoreRT.Decimal MulCoreRTManaged(CoreRT.Decimal a2, CoreRT.Decimal b2)
        {
            return CoreRT.Decimal.Multiply(a2, b2);
        }

        internal static decimal DivNative(decimal a, decimal b)
        {
            return DecimalDLL.Div(a, b);
        }

        internal static decimal DivOle32(decimal a, decimal b)
        {
            return Oleaut32.Div(a, b);
        }

        internal static CoreRT.Decimal DivCoreRTManaged(CoreRT.Decimal a2, CoreRT.Decimal b2)
        {
            return CoreRT.Decimal.Divide(a2, b2);
        }
    }
}
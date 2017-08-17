using System;
using System.Runtime.CompilerServices;

namespace Benchmarks.ClrClassLibrary
{
    internal static class Methods
    {
        internal static decimal AddManaged(decimal a, decimal b)
        {
            return Decimal.Add(a, b);
        }


        internal static decimal AddNative(decimal a, decimal b)
        {
            return DecimalDLL.Add(a, b);
        }


        internal static decimal AddPalRT(decimal a, decimal b)
        {
            return PalRT.Add(a, b);
        }


        internal static decimal AddOle32(decimal a, decimal b)
        {
            return Oleaut32.Add(a, b);
        }


        internal static CoreRT.Decimal AddCoreRTManaged(CoreRT.Decimal a2, CoreRT.Decimal b2)
        {
            return CoreRT.Decimal.Add(a2, b2);
        }


        internal static CoreRT.Decimal2 AddCoreRTManaged(CoreRT.Decimal2 a2, CoreRT.Decimal2 b2)
        {
            return CoreRT.Decimal2.Add(a2, b2);
        }

        internal static decimal MulManaged(decimal a, decimal b)
        {
            return System.Decimal.Multiply(a, b);
        }

        internal static decimal MulNative(decimal a, decimal b)
        {
            return DecimalDLL.Mul(a, b);
        }

        internal static decimal MulPalRT(decimal a, decimal b)
        {
            return PalRT.Mul(a, b);
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

        internal static CoreRT.Decimal2 MulCoreRTManaged(CoreRT.Decimal2 a2, CoreRT.Decimal2 b2)
        {
            return CoreRT.Decimal2.Multiply(a2, b2);
        }

        internal static decimal DivNative(decimal a, decimal b)
        {
            return DecimalDLL.Div(a, b);
        }

        internal static decimal DivPalRT(decimal a, decimal b)
        {
            return PalRT.Div(a, b);
        }

        internal static decimal DivOle32(decimal a, decimal b)
        {
            return Oleaut32.Div(a, b);
        }

        internal static CoreRT.Decimal DivCoreRTManaged(CoreRT.Decimal a2, CoreRT.Decimal b2)
        {
            return CoreRT.Decimal.Divide(a2, b2);
        }

        internal static CoreRT.Decimal2 DivCoreRTManaged(CoreRT.Decimal2 a2, CoreRT.Decimal2 b2)
        {
            return CoreRT.Decimal2.Divide(a2, b2);
        }

        internal static decimal AddNoop(decimal a, decimal b)
        {
            return Noop.Add(a, b);
        }

        internal static decimal MulNoop(decimal a, decimal b)
        {
            return Noop.Mul(a, b);
        }

        internal static decimal DivNoop(decimal a, decimal b)
        {
            return Noop.Div(a, b);
        }
    }
}
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


        internal static Managed.New.Decimal AddCoreRTManaged(Managed.New.Decimal a2, Managed.New.Decimal b2)
        {
            return Managed.New.Decimal.Add(a2, b2);
        }


        //internal static Managed.New.Decimal2 AddCoreRTManaged(Managed.New.Decimal2 a2, Managed.New.Decimal2 b2)
        //{
        //    return Managed.New.Decimal2.Add(a2, b2);
        //}

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

        internal static Managed.New.Decimal MulCoreRTManaged(Managed.New.Decimal a2, Managed.New.Decimal b2)
        {
            return Managed.New.Decimal.Multiply(a2, b2);
        }

        //internal static Managed.New.Decimal2 MulCoreRTManaged(Managed.New.Decimal2 a2, Managed.New.Decimal2 b2)
        //{
        //    return Managed.New.Decimal2.Multiply(a2, b2);
        //}

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

        internal static Managed.New.Decimal DivCoreRTManaged(Managed.New.Decimal a2, Managed.New.Decimal b2)
        {
            return Managed.New.Decimal.Divide(a2, b2);
        }

        //internal static Managed.New.Decimal2 DivCoreRTManaged(Managed.New.Decimal2 a2, Managed.New.Decimal2 b2)
        //{
        //    return Managed.New.Decimal2.Divide(a2, b2);
        //}

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
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Benchmarks
{
    public static class DecimalDLL
    {
        const string DLL = "decimalDLL.dll";

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int DecimalDiv(ref decimal lhs, ref decimal rhs, out decimal res);

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int DecimalMul(ref decimal lhs, ref decimal rhs, out decimal res);

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int DecimalAdd(ref decimal lhs, ref decimal rhs, out decimal res);

        public static Decimal Div(Decimal lhs, Decimal rhs)
        {
            Decimal res;
            int hres = DecimalDiv(ref lhs, ref rhs, out res);
            if (hres == 0)
                return res;

            if (hres == DISP_E_DIVBYZERO)
            {
                throw new DivideByZeroException();
            }
            else
                throw new OverflowException("DUMMY");
        }

        public static Decimal Mul(Decimal lhs, Decimal rhs)
        {
            Decimal res;
            int hres = DecimalMul(ref lhs, ref rhs, out res);
            if (hres == 0)
                return res;

            throw new OverflowException("DUMMY");
        }

        public static Decimal Add(Decimal lhs, Decimal rhs)
        {
            Decimal res;
            int hres = DecimalAdd(ref lhs, ref rhs, out res);
            if (hres == 0)
                return res;

            throw new OverflowException("DUMMY");
        }

        public const int DISP_E_DIVBYZERO = unchecked ((int)0x80020012L);
        public const int DISP_E_OVERFLOW = unchecked((int)0x8002000AL);
    };
}


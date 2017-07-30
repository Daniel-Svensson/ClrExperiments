using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Benchmarks
{
    public static class Oleaut32
    {
        const string DLL = "oleaut32.dll";

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int VarDecDiv(ref decimal lhs, ref decimal rhs, out decimal res);

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int VarDecMul(ref decimal lhs, ref decimal rhs, out decimal res);

        [SuppressUnmanagedCodeSecurity]
        [DllImport(DLL, CallingConvention = CallingConvention.Winapi, PreserveSig = true)]
        private static extern int VarDecAdd(ref decimal lhs, ref decimal rhs, out decimal res);

        public static Decimal Div(Decimal lhs, Decimal rhs)
        {
            Decimal res;
            int hres = VarDecDiv(ref lhs, ref rhs, out res);
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
            int hres = VarDecMul(ref lhs, ref rhs, out res);
            if (hres == 0)
                return res;

            throw new OverflowException("DUMMY");
        }

        public static Decimal Add(Decimal lhs, Decimal rhs)
        {
            Decimal res;
            int hres = VarDecAdd(ref lhs, ref rhs, out res);
            if (hres == 0)
                return res;

            throw new OverflowException("DUMMY");
        }

        public const int DISP_E_DIVBYZERO = unchecked ((int)0x80020012L);
        public const int DISP_E_OVERFLOW = unchecked((int)0x8002000AL);
    };
}


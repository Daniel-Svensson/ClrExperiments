using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace Benchmarks
{
    public class ArrayMul : ArrayBase
    {
        public ArrayMul(decimal[] lhs, decimal[] rhs)
            : base(lhs, rhs)
        {
        }

        [Benchmark]
        public decimal[] NetFramework()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.MulManaged(lhs, rhs);

            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokeNew()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.MulNative(lhs, rhs);
            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokePalRT()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.MulPalRT(lhs, rhs);
            return res_builtin;
        }

        [Benchmark(Baseline = true)]
        public decimal[] PInvokeOle32()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.MulOle32(lhs, rhs);
            return res_builtin;
        }

        [Benchmark]
        public CoreRT.Decimal[] CoreCRTManaged()
        {
            int dest = 0;
            foreach (var lhs in lhs_corert)
                foreach (var rhs in rhs_corert)
                    res_corert[dest++] = ClrClassLibrary.Methods.MulCoreRTManaged(lhs, rhs);
            return res_corert;
        }

        [Benchmark]
        public CoreRT.Decimal2[] CoreCRTManaged2()
        {
            int dest = 0;
            foreach (var lhs in lhs_corert2)
                foreach (var rhs in rhs_corert2)
                    res_corert2[dest++] = ClrClassLibrary.Methods.MulCoreRTManaged(lhs, rhs);
            return res_corert2;
        }
    }
}

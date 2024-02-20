using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class ArrayAdd : ArrayBase
    {
        public ArrayAdd(decimal[] lhs, decimal[] rhs)
            : base(lhs, rhs)
        {
        }

        [Benchmark]
        public decimal[] NetFramework()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.AddManaged(lhs, rhs);

            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokeNew()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.AddNative(lhs, rhs);
            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokePalRT()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.AddPalRT(lhs, rhs);
            return res_builtin;
        }

        [Benchmark(Baseline = true)]
        public decimal[] PInvokeOle32()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.AddOle32(lhs, rhs);
            return res_builtin;
        }

        [Benchmark]
        public Managed.New.Decimal[] CoreCRTManaged()
        {
            int dest = 0;
            foreach (var lhs in lhs_corert)
                foreach (var rhs in rhs_corert)
                    res_corert[dest++] = ClrClassLibrary.Methods.AddCoreRTManaged(lhs, rhs);
            return res_corert;
        }

        //[Benchmark]
        //public Managed.New.Decimal2[] CoreCRTManaged2()
        //{
        //    int dest = 0;
        //    foreach (var lhs in lhs_corert2)
        //        foreach (var rhs in rhs_corert2)
        //            res_corert2[dest++] = ClrClassLibrary.Methods.AddCoreRTManaged(lhs, rhs);
        //    return res_corert2;
        //}

        [Benchmark]
        public decimal[] PInvokeDummy()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                    res_builtin[dest++] = ClrClassLibrary.Methods.AddNoop(lhs, rhs);

            return res_builtin;
        }
    }
}

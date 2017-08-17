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
                {
                    try
                    {
                        res_builtin[dest++] = ClrClassLibrary.Methods.MulManaged(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }


            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokeNew()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = ClrClassLibrary.Methods.MulNative(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }
            return res_builtin;
        }

        [Benchmark]
        public decimal[] PInvokePalRT()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = ClrClassLibrary.Methods.MulPalRT(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }
            return res_builtin;
        }

        [Benchmark(Baseline = true)]
        public decimal[] PInvokeOle32()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = ClrClassLibrary.Methods.MulOle32(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }

            return res_builtin;
        }


        [Benchmark]
        public CoreRT.Decimal2[] CoreCRTManaged2()
        {
            int dest = 0;
            foreach (var lhs in lhs_corert2)
                foreach (var rhs in rhs_corert2)
                {
                    try
                    {
                        res_corert2[dest++] = ClrClassLibrary.Methods.MulCoreRTManaged(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }
            return res_corert2;
        }

        [Benchmark]
        public decimal[] PInvokeDummy()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = ClrClassLibrary.Methods.MulNoop(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }


            return res_builtin;
        }
    }
}

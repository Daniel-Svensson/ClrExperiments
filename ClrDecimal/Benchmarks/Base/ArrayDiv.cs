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
    public class ArrayDiv : ArrayBase
    {
        public ArrayDiv(decimal[] lhs, decimal[] rhs)
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
                        res_builtin[dest++] = ClrClassLibrary.Methods.DivManaged(lhs, rhs);
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
                        res_builtin[dest++] = ClrClassLibrary.Methods.DivNative(lhs, rhs);
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
                        res_builtin[dest++] = ClrClassLibrary.Methods.DivPalRT(lhs, rhs);
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
                        res_builtin[dest++] = ClrClassLibrary.Methods.DivOle32(lhs, rhs);
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
                        res_corert2[dest++] = ClrClassLibrary.Methods.DivCoreRTManaged(lhs, rhs);
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
                        res_builtin[dest++] = ClrClassLibrary.Methods.DivNoop(lhs, rhs);
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

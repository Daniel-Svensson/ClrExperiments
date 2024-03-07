using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class ArrayMul : ArrayBase
    {
        public ArrayMul(decimal[] lhs, decimal[] rhs)
            : base(lhs, rhs)
        {
        }

        //[Benchmark]
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

        //[Benchmark]
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

        //[Benchmark]
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

        //[Benchmark(Baseline = true)]
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
        public decimal[] New()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = Managed.New.Decimal.Multiply(lhs, rhs);
                    }
                    catch (Exception)
                    {
                        dest++;
                    }
                }

            return res_builtin;
        }

        [Benchmark(Baseline = true)]
        public decimal[] Main()
        {
            int dest = 0;
            foreach (var lhs in lhs_builtin)
                foreach (var rhs in rhs_builtin)
                {
                    try
                    {
                        res_builtin[dest++] = Managed.Main.Decimal.Multiply(lhs, rhs);
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

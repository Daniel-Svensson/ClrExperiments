using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace Benchmarks
{
    public class Mul96by96
    {
        readonly decimal a;
        readonly decimal b;

        public Mul96by96()
        {
            a = new decimal(1023, 1, 1, false, 0);
            b = new decimal(3, 0, 0, false, 0);
        }

        [Benchmark]
        public decimal NetFramework()
        {
            return ClrClassLibrary.Methods.MulManaged(a, b);
        }

        [Benchmark]
        public decimal Native()
        {
            return ClrClassLibrary.Methods.MulNative(a, b);
        }

        [Benchmark(Baseline = true)]
        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.MulOle32(a, b);
        }
    }
}

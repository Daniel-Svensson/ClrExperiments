using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace Benchmarks
{
    public class Div96by96
    {
        readonly decimal a;
        readonly decimal b;
        readonly CoreRT.Decimal a2;
        readonly CoreRT.Decimal b2;

        public Div96by96()
        {
            a = new decimal(1023, 412, 213, false, 0);
            b = new decimal(32, 32, 1, false, 0);
            a2 = a;
            b2 = b;
        }

        [Benchmark]
        public decimal NetFramework()
        {
            return ClrClassLibrary.Methods.DivManaged(a, b);
        }

        [Benchmark]
        public decimal Native()
        {
            return ClrClassLibrary.Methods.DivNative(a, b);
        }

        [Benchmark(Baseline = true)]
        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.DivOle32(a, b);
        }

        [Benchmark]
        public CoreRT.Decimal CoreCRTManaged()
        {
            return ClrClassLibrary.Methods.DivCoreRTManaged(a2, b2);
        }
    }
}

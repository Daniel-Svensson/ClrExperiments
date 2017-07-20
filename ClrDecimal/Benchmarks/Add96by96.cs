using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace Benchmarks
{
    public class Add96by96
    {
        readonly decimal a;
        readonly decimal b;

        public Add96by96()
        {
            a = new decimal(1023, 345, 321, false, 0);
            b = new decimal(32, 23, 2, false, 0);
        }

        [Benchmark]
        public decimal NetFramework()
        {
            return ClrClassLibrary.Methods.AddManaged(a, b);
        }

        [Benchmark]
        public decimal Native()
        {
            return ClrClassLibrary.Methods.AddNative(a, b);
        }

        [Benchmark(Baseline = true)]
        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.AddOle32(a, b);
        }
    }
}

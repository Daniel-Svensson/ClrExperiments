using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    //[InProcess]
    [InProcess]
    public class Addition
    {
        public IEnumerable<object[]> TestCases()
        {
            return [
                [new decimal(0, 1, 0, false, 3), new decimal(0, 1, 0, false, 3), "2^32 + 2^32 same scale"],
                [(1m/3), (decimal)ulong.MaxValue,"1/3 + (2^64-1)"],
                [new decimal(-1, -1, -1, false, 1), new decimal(-1, -1, -1, false, 1), "with carry"],
                //[new decimal(-1, -1, -2, false, 1), new decimal(-1, -1, -1, false, 1), "subtract"],
            ];
        }

        [Benchmark]
        [ArgumentsSource(nameof(TestCases))]
        public decimal System_Decimal(decimal a, decimal b, string descr)
        {
            return System.Decimal.Add(a, b);
        }

        [Benchmark]
        [ArgumentsSource(nameof(TestCases))]
        public decimal New(decimal a, decimal b, string descr)
        {
            return Managed.New.Decimal.Add(a, b);
        }

        [Benchmark(Baseline = true)]
        [ArgumentsSource(nameof(TestCases))]
        public decimal Main(decimal a, decimal b, string descr)
        {
            return Managed.Main.Decimal.Add(a, b);
        }

    }
}

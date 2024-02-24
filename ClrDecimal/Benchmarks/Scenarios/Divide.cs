using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    //[ReturnValueValidator()]
    //[RyuJitX64Job, RyuJitX86Job]
#if TARGET_32BIT || TARGET_64BIT
    [InProcess] // To allow "Native"
#endif
    public class Divide
    {
        public IEnumerable<object[]> TestCases()
        {
            return [
                [new decimal(1023, 412, 213, false, 0), new decimal(32, 32, 1, false, 0), "96bit / 96bit"],
                [new decimal(1023, 412, 213, false, 2), new decimal(32, 3, 0, false, 3), "96bit / 64bit"],
                [new decimal(int.MaxValue, int.MaxValue, 5, false, 2), new decimal(3, 0, 0, false, 1), "96bit / 32bit"],
                [new decimal(int.MaxValue, 2, 5, false, 2), new decimal(33, 0, 0, false, 1), "34bit / 32bit"],
            ];
        }

        //[Benchmark]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal NetFramework(decimal a, decimal b, string descr)
        {
            return decimal.Divide(a, b);
        }

        //[Benchmark]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal Native(decimal a, decimal b, string descr)
        {
            return ClrClassLibrary.Methods.DivNative(a, b);
        }

        //[Benchmark(Baseline = true)]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal Ole32(decimal a, decimal b, string descr)
        {
            return ClrClassLibrary.Methods.DivOle32(a, b);
        }

        [Benchmark]
        [ArgumentsSource(nameof(TestCases))]
        public decimal New(decimal a, decimal b, string descr)
        {
            return Managed.New.Decimal.Divide(a, b);
        }

        [Benchmark(Baseline = true)]
        [ArgumentsSource(nameof(TestCases))]
        public decimal Main(decimal a, decimal b, string descr)
        {
            return Managed.Main.Decimal.Divide(a, b);
        }
    }
}

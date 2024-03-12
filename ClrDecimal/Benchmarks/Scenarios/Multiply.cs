using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    //[ReturnValueValidator()]
    //[RyuJitX64Job, /*RyuJitX86Job*/]
#if TARGET_32BIT
    [InProcess] // To allow "Native"
#endif
    public class Multiply
    {
        public IEnumerable<object[]> TestCases()
        {
            return [
                    [new decimal(1023, 1, 1, false, 10), new decimal(3, 1, 21, false, 8), "96bit * 96bit"],
                [new decimal((int)0x5cc5c5c5, 0, 0, false, 5), new decimal(3, 0, 0, false, 8), "32bit * 32bit"],
                [new decimal((int)0x5cc5c5c5, (int)0x1e1e1e1e, 3, false, 5), new decimal(0x00123456, 0, 0, false, 8), "96bit * 32bit"],
                [new decimal((int)0x5cc5c5c5, (int)0x1e1e1e1e, 0, false, 5), new decimal(0x00123456, 0x00123456, 0, false, 8), "64it * 64bit"],
            ];
        }

        //[Benchmark(Baseline = true)]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal System_Decimal(decimal a, decimal b, string descr)
        {
            return ClrClassLibrary.Methods.MulManaged(a, b);
        }

        //[Benchmark]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal Native(decimal a, decimal b, string descr)
        {
            return ClrClassLibrary.Methods.MulNative(a, b);
        }
       
        //[Benchmark()]
        //[ArgumentsSource(nameof(TestCases))]
        public decimal Ole32(decimal a, decimal b, string descr)
        {
            return ClrClassLibrary.Methods.MulOle32(a, b);
        }

        [Benchmark]
        [ArgumentsSource(nameof(TestCases))]
        public decimal New(decimal a, decimal b, string descr)
        {
            return Managed.New.Decimal.Multiply(a, b);
        }

        [Benchmark(Baseline = true)]
        [ArgumentsSource(nameof(TestCases))]
        public decimal Main(decimal a, decimal b, string descr)
        {
            return Managed.Main.Decimal.Multiply(a, b);
        }

        //[Benchmark]
        //public Managed.New.Decimal2 CoreCRTManaged2()
        //{
        //    return ClrClassLibrary.Methods.MulCoreRTManaged(a3, b3);
        //}
    }
}

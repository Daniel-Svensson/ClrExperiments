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
        readonly CoreRT.Decimal a2;
        readonly CoreRT.Decimal b2;
        readonly CoreRT.Decimal2 a3;
        readonly CoreRT.Decimal2 b3;

        public Mul96by96()
        {
            a = new decimal(1023, 1, 1, false, 10);
            b = new decimal(3, 1, 21, false, 8);
            a2 = a;
            b2 = b;
            a3 = a;
            b3 = b;
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
        
        [Benchmark]
        public decimal PalRT()
        {
            return ClrClassLibrary.Methods.MulPalRT(a, b);
        }

        [Benchmark(Baseline = true)]
        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.MulOle32(a, b);
        }

        [Benchmark]
        public CoreRT.Decimal CoreCRTManaged()
        {
            return ClrClassLibrary.Methods.MulCoreRTManaged(a2, b2);
        }

        [Benchmark]
        public CoreRT.Decimal2 CoreCRTManaged2()
        {
            return ClrClassLibrary.Methods.MulCoreRTManaged(a3, b3);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class Mul96by96
    {
        readonly decimal a;
        readonly decimal b;
        readonly Managed.New.Decimal a2;
        readonly Managed.New.Decimal b2;
        //readonly Managed.New.Decimal2 a3;
        //readonly Managed.New.Decimal2 b3;

        public Mul96by96()
        {
            a = new decimal(1023, 1, 1, false, 10);
            b = new decimal(3, 1, 21, false, 8);
            a2 = a;
            b2 = b;
            //a3 = a;
            //b3 = b;
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
        public Managed.New.Decimal CoreCRTManaged()
        {
            return ClrClassLibrary.Methods.MulCoreRTManaged(a2, b2);
        }

        //[Benchmark]
        //public Managed.New.Decimal2 CoreCRTManaged2()
        //{
        //    return ClrClassLibrary.Methods.MulCoreRTManaged(a3, b3);
        //}
    }
}

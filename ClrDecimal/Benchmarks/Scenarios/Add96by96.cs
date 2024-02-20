using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class Add96by96
    {
        readonly decimal a;
        readonly decimal b;
        readonly Managed.New.Decimal a2;
        readonly Managed.New.Decimal b2;
        //readonly Managed.New.Decimal2 a3;
        //readonly Managed.New.Decimal2 b3;

        public Add96by96()
        {
            // TODO: test bort 3 and 10 
            a = new decimal(1023, 345, 321, false, 3);
            b = new decimal(32, 23, 2, false, 0);
            a2 = a;
            b2 = b;
            //a3 = a;
            //b3 = b;
        }

        //[Benchmark]
        public decimal NetFramework()
        {
            return ClrClassLibrary.Methods.AddManaged(a, b);
        }

        [Benchmark]
        public decimal Native()
        {
            return ClrClassLibrary.Methods.AddNative(a, b);
        }
        [Benchmark]
        public decimal PalRT()
        {
            return ClrClassLibrary.Methods.AddPalRT(a, b);
        }

        [Benchmark(Baseline = true)]
        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.AddOle32(a, b);
        }

        [Benchmark]
        public Managed.New.Decimal CoreCRTManaged()
        {
            return ClrClassLibrary.Methods.AddCoreRTManaged(a2, b2);
        }
    }
}

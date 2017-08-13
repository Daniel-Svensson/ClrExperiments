using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Diagnosers;
using BenchmarkDotNet.Horology;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Toolchains;
using BenchmarkDotNet.Toolchains.InProcess;

namespace Benchmarks
{
    class Program
    {
        static readonly bool Is64Bit = IntPtr.Size == 8;

        static void Main(string[] args)
        {
            var a = new Div96by96();
            a.CoreCRTManaged2();
            return;

            var dummy = new Add96by96();
            var expected = dummy.NetFramework();
            var pinvoke = dummy.Ole32();

            //var coreAssemblyInfo = FileVersionInfo.GetVersionInfo(typeof(object).Assembly.Location);
            //Console.WriteLine($"Hello World from Core {coreAssemblyInfo.ProductVersion}");

            //Console.WriteLine(typeof(Program).GetTypeInfo().Assembly.Location);
            //Console.WriteLine($"Is 64bit: {IntPtr.Size == 8}");

            ////Debugger.Break();
            //Debug.Assert(expected == pinvoke);
            //Console.WriteLine(dummy.NetFramework());
            //Console.WriteLine(dummy.CoreCRTManaged());
            //Console.WriteLine(dummy.CoreCRTManaged2());
            //Console.WriteLine(dummy.Ole32());
            //Console.WriteLine(dummy.Native());

            //Debugger.Break();
            //return;,


            //dummy.CoreCRTManaged2();
            //dummy.CoreCRTManaged2();

            var b = new InterestBenchmark();
            b.Setup();

            for (int i = 0; i < 1000; ++i)
                b.CoreRT2();
           //return;


            var config = DefaultConfig.Instance
                .With(Job.ShortRun
                        // Job.Default
#if !NET47

                        .With(InProcessToolchain.Instance)
#endif // !NET47
                        //.WithMinIterationTime(TimeInterval.Millisecond * 100)
                        )
                .With(BenchmarkDotNet.Validators.ExecutionValidator.FailOnError)
#if NET47_DUMMY
                 .With(MemoryDiagnoser.Default)
                .With(new[]
                {
                HardwareCounter.BranchInstructionRetired,
                HardwareCounter.BranchInstructions,
                HardwareCounter.BranchMispredictions,
                HardwareCounter.CacheMisses,
                HardwareCounter.TotalIssues,
                HardwareCounter.LlcReference,
                HardwareCounter.LlcMisses,

                }
                )
#endif
                //.With(Job.Core)
                //.With(Job.RyuJitX64, Job.LegacyJitX86)
                ;

            //#if NET47
            //            if (Is64Bit)
            //            {
            //                config = config.With(Job.RyuJitX64, Job.LegacyJitX64);
            //            }
            //#endif

            //BenchmarkRunner.Run<Add96by96_Carry>(config);
            //BenchmarkRunner.Run<Add96by96>(config);
            //BenchmarkRunner.Run<Mul96by96>(config);
            //

            BenchmarkRunner.Run<InterestBenchmark>(config);

            //BenchmarkRunner.Run<AddDummy>(config);


            if (Debugger.IsAttached)
            {
                Console.WriteLine("Press ENTER to exit");
                Console.ReadLine();
            }
        }

    }

    public class AddDummy
    {
        readonly decimal a;
        readonly decimal b;
        readonly CoreRT.Decimal a2;
        readonly CoreRT.Decimal b2;
        readonly CoreRT.Decimal2 a3;
        readonly CoreRT.Decimal2 b3;

        public AddDummy()
        {
            // TODO: test bort 3 and 10 
            a = new decimal(1023, 345, 321, false, 3);
            b = new decimal(32, 23, 2, false, 0);
            a2 = a;
            b2 = b;
            a3 = a;
            b3 = b;
        }

        //[Benchmark]
        public decimal NetFramework()
        {
            return ClrClassLibrary.Methods.AddManaged(a, b);
        }


        public decimal Native()
        {
            return ClrClassLibrary.Methods.AddNative(a, b);
        }

        public decimal PalRT()
        {
            return ClrClassLibrary.Methods.AddPalRT(a, b);
        }


        public decimal Ole32()
        {
            return ClrClassLibrary.Methods.AddOle32(a, b);
        }



        public CoreRT.Decimal CoreCRTManaged()
        {
            return ClrClassLibrary.Methods.AddCoreRTManaged(a2, b2);
        }

        [Benchmark]
        public CoreRT.Decimal2 CoreCRTManaged2()
        {
            return ClrClassLibrary.Methods.AddCoreRTManaged(a3, b3);
        }
    }

}

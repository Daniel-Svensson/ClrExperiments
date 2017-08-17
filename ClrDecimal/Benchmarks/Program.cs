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


        static void PrintStats(decimal[] numbers)
        {
            double count = numbers.Length;

            var bits = numbers.Select(dec => decimal.GetBits(dec)).ToList();
            var scales = bits.Select(b => (b[3] & 0x00ff0000) >> 16).ToList();

            double bits96 = bits.Count(b => b[2] != 0);
            double bits64 = bits.Count(b => b[2] == 0 && b[1] != 0);
            double bits32 = bits.Count(b => b[2] == 0 && b[1] == 0);

            double scaled = scales.Count(s => s != 0);

            Console.WriteLine($"Total of {count} numbers");
            Console.WriteLine($"{bits96} ({bits96/count:p}) use 3 int (96 bits)");
            Console.WriteLine($"{bits64} ({bits64/count:p}) use 2 int (64 bits)");
            Console.WriteLine($"{bits32} ({bits32/count:p}) use 1 int (32 bits)");

            Console.WriteLine($"Total of {scaled} numbers with average scale {scales.Average()}");
            Console.WriteLine($"Total of {count} numbers");
        }

        static void Main(string[] args)
        {
            PrintStats(Distributions.AddPentP.LoadFile());

            //(new Distributions.MulPentP()).NetFramework();
            (new Distributions.DivPentP()).NetFramework();

            var config = DefaultConfig.Instance
                .With(Job.ShortRun
                        // Job.Default
#if !NET47

                        .With(InProcessToolchain.Instance)
#endif // !NET47
                        //.WithMinIterationTime(TimeInterval.Millisecond * 100)
                        )
                .With(BenchmarkDotNet.Validators.ExecutionValidator.FailOnError)
#if NET47
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

            //BenchmarkRunner.Run<InterestBenchmark>(config);

            BenchmarkRunner.Run<Distributions.AddPentP>(config);
            //BenchmarkRunner.Run<Distributions.MulPentP>(config);
            //BenchmarkRunner.Run<Distributions.DivPentP>(config);

            //BenchmarkRunner.Run<AddDummy>(config);


            if (Debugger.IsAttached)
            {
                Console.WriteLine("Press ENTER to exit");
                Console.ReadLine();
            }
        }

    }
}

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Configs;
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
            var dummy = new Add96by96();
            var expected = dummy.NetFramework();
            var pinvoke = dummy.Ole32();

            var coreAssemblyInfo = System.Diagnostics.FileVersionInfo.GetVersionInfo(typeof(object).Assembly.Location);
            Console.WriteLine($"Hello World from Core {coreAssemblyInfo.ProductVersion}");

            Console.WriteLine(typeof(Program).GetTypeInfo().Assembly.Location);
            Console.WriteLine($"Is 64bit: {IntPtr.Size == 8}");

            //Debugger.Break();
            Debug.Assert(expected == pinvoke);
            Console.WriteLine(dummy.NetFramework());
            Console.WriteLine(dummy.CoreCRTManaged());
            Console.WriteLine(dummy.Ole32());
            Console.WriteLine(dummy.Native());

            //Debugger.Break();
            //return;


            var config = DefaultConfig.Instance
                .With(Job.Default
                        .With(InProcessToolchain.Instance)
                        )
                .With(BenchmarkDotNet.Validators.ExecutionValidator.FailOnError)
                //.With(Job.Core)
                //.With(Job.RyuJitX64, Job.LegacyJitX86)
                ;

#if NET47
            if (Is64Bit)
            {
                config = config.With(Job.RyuJitX64, Job.LegacyJitX64);
            }
#endif

            BenchmarkRunner.Run<Add96by96>(config);
            BenchmarkRunner.Run<Mul96by96>(config);
            BenchmarkRunner.Run<Div96by96>(config);

            Console.WriteLine("Press ENTER to exit");
            Console.ReadLine();
        }
    }
}

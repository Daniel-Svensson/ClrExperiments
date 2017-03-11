using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Numerics;
using System.Threading.Tasks;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Configs;

namespace Benchmarks
{
    class Program
    {
        static void Main(string[] args)
        {
            var benchmarks = new[] 
            {
                    //typeof(SumArrayInt),
                    typeof(SumArrayFloat),
                    typeof(SumArrayDouble),
              
                    typeof(SimpleMathTests_Float),
                    typeof(SimpleMathTests_Decimal),

                    typeof(SumIntegers),
            };

            // Invoke methods in order to have a change of seeing dissasembly
            if (Debugger.IsAttached)
            {
                var runner = new SimpleMathTests_Float();
                runner.GenericSumImpl();
                runner.MethodSumImpl();
                runner.ForLoopSumImpl();

                PrintOutputs(new SumArrayFloat());
                PrintOutputs(new SumArrayDouble());
                PrintOutputs(new SumArrayInt());

                PrintOutputs(new SumArrayInt());

                PrintOutputs(new GenericArrayProcessingBenchmarks<float>());


                Console.WriteLine("Press any key to continue");
                Console.Read();
            }

            /*var s = new BenchmarkSwitcher(benchmarks);
            s.RunAll();
            //s.Run(args);
            */
            
            BenchmarkRunner.Run<SimpleMathTests_Decimal>(DefaultConfig.Instance.KeepBenchmarkFiles());
            /*BenchmarkRunner.Run<SimpleMathTests_Float>();

            BenchmarkRunner.Run<SumArrayFloat>();
            BenchmarkRunner.Run<SumArrayDouble>();
            BenchmarkRunner.Run<SumArrayInt>();

            BenchmarkRunner.Run<SumIntegers>();
            */
            Console.WriteLine("Done");
            Console.ReadLine();
        }

        private static void PrintOutputs(object instance)
        {
            var type = instance.GetType();
            Console.WriteLine("Running tests for {0}", type);
            //var instance = Activator.CreateInstance(type);

            foreach (var method in type
                        .GetMethods(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.DeclaredOnly)
                        .Where(m => m.GetCustomAttributes(false).Any()))
            {
                Console.WriteLine($"Method {method.Name} returns {method.Invoke(instance, null)}");
            }
            Console.WriteLine();
        }
    }

 
}

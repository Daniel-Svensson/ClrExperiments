using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Running;

namespace Benchmarks
{
    class Program
    {
        static void Main(string[] args)
        {
            BenchmarkRunner.Run<Add96by96>();
            BenchmarkRunner.Run<Mul96by96>();
            BenchmarkRunner.Run<Div96by96>();

            Console.WriteLine("Press ENTER to exit");
            Console.ReadLine();
        }
    }
}

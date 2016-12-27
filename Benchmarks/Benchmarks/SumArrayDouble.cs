using System;
using System.Linq;
using System.Numerics;
using BenchmarkDotNet.Attributes;
using NBench;

namespace Benchmarks
{
    [Config(typeof(BenchmarkConfig))]
    public class SumArrayDouble : GenericSumArrayBenchmarks<double>
    {
        public SumArrayDouble()
        {
            var rnd = new Random();
            for (int i = 0; i < ArraySize; ++i)
                _values[i] = (rnd.NextDouble() - 0.5) * (100);
        }

        [Benchmark(Baseline = true)]
        public double ForLoopSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum += _values[i];
            return sum;
        }

        [Benchmark()]
        public double LinqSum()
        {
            return _values.Sum();
        }
    }
}

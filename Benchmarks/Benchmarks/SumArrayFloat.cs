using System;
using System.Linq;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

namespace Benchmarks
{
    [Config(typeof(BenchmarkConfig))]
    public class SumArrayFloat : GenericSumArrayBenchmarks<float>
    {
        public SumArrayFloat()
        {
            var rnd = new Random();
            for (int i = 0; i < ArraySize; ++i)
                _values[i] = (float)(rnd.NextDouble() - 0.5) * (100);
        }

        [Benchmark(Baseline = true)]
        public float ForLoopSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = sum + _values[i];
            return sum;
        }

        [Benchmark()]
        public double LinqSum()
        {
            return _values.Sum();
        }
    }
}

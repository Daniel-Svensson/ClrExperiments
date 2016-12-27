using System;
using System.Linq;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

namespace Benchmarks
{
    [Config(typeof(BenchmarkConfig))]
    public class SimpleMathTests_Float : GenericSumArrayBenchmarks<float>
    {
        public SimpleMathTests_Float()
        {
            var rnd = new Random();
            for (int i = 0; i < ArraySize; ++i)
                _values[i] = (float)(rnd.NextDouble() - 0.5) * (100);
        }

        [Benchmark(Baseline = true)]
        public void NormalSum()
        {
            ForLoopSumImpl();
        }

        [Benchmark()]
        public void MethodSum()
        {
            MethodSumImpl();
        }

        [Benchmark()]
        public void MethodRefSum()
        {
            MethodRefSumImpl();
        }

        [Benchmark()]
        public void GenericSum()
        {
            GenericSumImpl();
        }

        [Benchmark()]
        public void MethodIncSum()
        {
            MethodSumIncImpl();
        }

        public float ForLoopSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = sum + _values[i];
            return sum;
        }

        public float GenericSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.Add(sum, _values[i]);
            return sum;
        }

        public float MethodSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.AddFloat(sum, _values[i]);
            return sum;
        }

        public float MethodRefSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.AddFloatRef(ref sum, ref _values[i]);
            return sum;
        }

        public float MethodSumIncImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                MathHelper.IncFloat(ref sum, _values[i]);
            return sum;
        }
    }

    
    [Config(typeof(BenchmarkConfig))]
    public class SimpleMathTests_Decimal
    {
        const int ArraySize = 100000;
        const int PartSums = 10;

        decimal[] _values;
        const decimal Zero = decimal.Zero;

        public SimpleMathTests_Decimal()
        {
            _values = new decimal[ArraySize];
            var rnd = new Random();
            for (int i = 0; i < ArraySize; ++i)
                _values[i] = (decimal)(rnd.NextDouble() - 0.5) * (100);
        }

        [Benchmark(Baseline = true)]
        public void NormalSum()
        {
            ForLoopSumImpl();
        }

        [Benchmark()]
        public void MethodSum()
        {
            MethodSumImpl();
        }

        [Benchmark()]
        public void MethodRefSum()
        {
            MethodSumRefImpl();
        }

        [Benchmark()]
        public void MethodIncSum()
        {
            MethodSumIncImpl();
        }

        [Benchmark()]
        public void GenericSum()
        {
            GenericSumImpl();
        }

        public decimal ForLoopSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = sum + _values[i];
            return sum;
        }

        public decimal GenericSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.Add(sum, _values[i]);
            return sum;
        }

        public decimal MethodSumImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.AddDecimal(sum, _values[i]);
            return sum;
        }

        public decimal MethodSumRefImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MathHelper.AddDecimalRef(ref sum, ref _values[i]);
            return sum;
        }

        public decimal MethodSumIncImpl()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                MathHelper.IncDec(ref sum, _values[i]);
            return sum;
        }
    }
 
}

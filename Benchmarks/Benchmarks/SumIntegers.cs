using System;
using System.Linq;
using System.Numerics;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

namespace Benchmarks
{
    [Config(typeof(BenchmarkConfig))]
    public class SumIntegers
    {
        [Benchmark(Baseline = true)]
        public float ForLoopSumImpl()
        {
            var sum = 0;
            for (int i = 0; i < 10000; ++i)
                sum = sum + i;
            return sum;
        }

        [Benchmark()]
        public float GenericILMethod()
        {
            var sum = 0;
            for (int i = 0; i < 10000; ++i)
                sum = MyClass.AddGeneric(sum, i);
            return sum;
        }

        [Benchmark()]
        public float GenericHelperClass()
        {
            var sum = 0;
            for (int i = 0; i < 10000; ++i)
                sum = MathHelper.Add(sum, i);
            return sum;
        }

        [Benchmark()]
        public float DynamicHelperC()
        {
            var sum = 0;
            for (int i = 0; i < 10000; ++i)
                sum = DynamicHelper.Add(sum, i);
            return sum;
        }

        [Benchmark()]
        public float VectorBased()
        {
            var sum = Vector<int>.Zero;
            var numbers = ComputeVectorStart();

            for (int i = 0; i < 10000; i += Vector<int>.Count)
            {
                sum = sum + numbers;
                numbers += VectorOffset;
            }
            return Vector.Dot(sum, Vector<int>.One);
        }

        static readonly Vector<int> VectorStart = ComputeVectorStart();

        private static Vector<int> ComputeVectorStart()
        {
            var offset = new int[Vector<int>.Count];
            for (int i = 0; i < Vector<int>.Count; ++i)
                offset[i] = i;

            return new Vector<int>(offset);
        }

        static Vector<int> VectorOffset = ComputeVectorOffset();

        private static Vector<int> ComputeVectorOffset()
        {
            return new Vector<int>(Vector<int>.Count);
        }
    }
}

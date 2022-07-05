using System;
using System.Linq;
using System.Numerics;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class GenericArrayProcessingBenchmarks<T>
        where T : struct, IConvertible, IComparable<T>, IEquatable<T>
    {
        public const int ArraySize = 100000;

        public T[] _values;
        public int VectorCount => Vector<T>.Count;
        public Vector<T> VectorZero => Vector<T>.Zero;
        public Vector<T> VectorOne => Vector<T>.One;
        public readonly T Zero;

        public GenericArrayProcessingBenchmarks()
        {
            _values = new T[ArraySize];
        }

        [Benchmark]
        public T VectorMagnitude()
        {
            var sum = VectorZero;
            int i = 0;
            int stop = _values.Length - VectorCount;
            for (i = 0; i < _values.Length; i += VectorCount)
            {
                var item = new Vector<T>(_values, i);
                sum += Vector.Abs(item);
            }

            return Vector.Dot(sum, VectorOne);
        }

        [Benchmark]
        public T VectorMin()
        {
            var min = new Vector<T>(_values);
            for (int i = 0; i < _values.Length; i += VectorCount)
            {
                var item = new Vector<T>(_values, i);
                min = Vector.Min(min, item);
            }

            T minimum = min[0];
            for (int i = 1; i < Vector<T>.Count; ++i)
            {
                var item = min[i];
                if (minimum.CompareTo(item) > 0)
                    minimum = item;
            }
            return minimum;
        }
    }
}

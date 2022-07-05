using System;
using System.Linq;
using System.Numerics;
using BenchmarkDotNet.Attributes;

namespace Benchmarks
{
    public class GenericSumArrayBenchmarks<T>
        where T : struct, IConvertible, IComparable<T>, IEquatable<T>
    {
        public const int ArraySize = 100000;

        public T[] _values;
        public int VectorCount => Vector<T>.Count;
        public Vector<T> VectorZero => Vector<T>.Zero;
        public Vector<T> VectorOne => Vector<T>.One;
        public readonly T Zero;

        public GenericSumArrayBenchmarks()
        {
            _values = new T[ArraySize];
        }

        [Benchmark]
        public T Generic_Sum_Scalar_Proposal()
        {
            var sum = System.Numerics.Scalar.Zero<T>();
            for (int i = 0; i < _values.Length; ++i)
                sum = System.Numerics.Scalar.Add(sum, _values[i]);
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_ScalarOfT_NoCast()
        {
            System.Numerics.Scalar<T> sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = sum + _values[i];
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_ScalarOfT_ExplicitCast()
        {
            System.Numerics.Scalar<T> sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = sum + (System.Numerics.Scalar<T>)_values[i];
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_Base_Member()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = Add(sum, _values[i]);
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_IL_Method()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = MyClass.AddGeneric(sum, _values[i]);
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_Dynamic()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = DynamicHelper.Add(sum, _values[i]);
            return sum;
        }

/*
        [Benchmark]
        public T Generic_Sum_ScalarClass_Static()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = ScalarClass<T>.AddStatic(sum, _values[i]);
            return sum;
        }

        [Benchmark]
        public T Generic_Sum_ScalarClass_Instance()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = ScalarClass<T>.Instance.Add(sum, _values[i]);
            return sum;
        }
*/
        [Benchmark]
        public T Generic_Sum_ScalarHelper()
        {
            var s = new ScalarHelper<T>();
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = s.Add(sum, _values[i]);
            return sum;
        }

/*
        public T Generic_Sum_Scalar_Helper()
        {
            var s = new ScalarStruct<T>();
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = s.Add(sum, _values[i]);
            return sum;
        }
*/

        [Benchmark]
        public T Generic_Sum_Base_Static()
        {
            var sum = Zero;
            for (int i = 0; i < _values.Length; ++i)
                sum = AddStatic(sum, _values[i]);
            return sum;
        }

        [Benchmark]
        public T VectorSummary()
        {
            var sum = VectorZero;
            int i = 0;
            int stop = _values.Length - VectorCount;
            for (i = 0; i < _values.Length; i += VectorCount)
            {
                var item = new Vector<T>(_values, i);
                sum += item;
            }

            return Vector.Dot(sum, VectorOne);
        }

        protected T Add(T lhs, T rhs)
        {
            if (typeof(T) == typeof(float))
            {
                return (T)(object)(((float)(object)lhs) + ((float)(object)rhs));
            }
            if (typeof(T) == typeof(int))
            {
                return (T)(object)(((int)(object)lhs) + ((int)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) + ((double)(object)rhs));
            }
            if (typeof(T) == typeof(long))
            {
                return (T)(object)(((long)(object)lhs) + ((long)(object)rhs));
            }
            if (typeof(T) == typeof(decimal))
            {
                return (T)(object)(((decimal)(object)lhs) + ((decimal)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) + ((double)(object)rhs));
            }
            if (typeof(T) == typeof(short))
            {
                return (T)(object)(((short)(object)lhs) + ((short)(object)rhs));
            }
            throw new NotSupportedException();
        }

        protected static T AddStatic(T lhs, T rhs)
        {
            if (typeof(T) == typeof(float))
            {
                return (T)(object)(((float)(object)lhs) + ((float)(object)rhs));
            }
            if (typeof(T) == typeof(int))
            {
                return (T)(object)(((int)(object)lhs) + ((int)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) + ((double)(object)rhs));
            }
            if (typeof(T) == typeof(long))
            {
                return (T)(object)(((long)(object)lhs) + ((long)(object)rhs));
            }
            if (typeof(T) == typeof(decimal))
            {
                return (T)(object)(((decimal)(object)lhs) + ((decimal)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) + ((double)(object)rhs));
            }
            if (typeof(T) == typeof(short))
            {
                return (T)(object)(((short)(object)lhs) + ((short)(object)rhs));
            }
            throw new NotSupportedException();
        }
    }
}

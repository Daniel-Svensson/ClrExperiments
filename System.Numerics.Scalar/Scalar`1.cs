using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace System.Numerics
{
    public struct Scalar<T>
           where T : struct /*, IEquatable<T>, IComparable<T> */
    {
        readonly T _value;

        Scalar(T value)
        {
            _value = value;
        }

        public T Value => _value;

        public static T Zero => Scalar.Zero<T>();
        public static T One => Scalar.One<T>();

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Scalar<T> operator +(Scalar<T> a, Scalar<T> b) => Scalar.Add(a.Value, b.Value);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Scalar<T> operator +(Scalar<T> a, T b) => Scalar.Add(a.Value, b);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Scalar<T> operator -(Scalar<T> a, Scalar<T> b) => new Scalar<T>(Scalar.Subtract(a.Value, b.Value));
        public static Scalar<T> operator *(Scalar<T> a, Scalar<T> b) => new Scalar<T>(Scalar.Multiply(a.Value, b.Value));
        public static Scalar<T> operator /(Scalar<T> a, Scalar<T> b) => new Scalar<T>(Scalar.Divide(a.Value, b.Value));

        // Conversion to/from native types
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static implicit operator T(Scalar<T> scalar) => scalar.Value;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static implicit operator Scalar<T>(T value) => new Scalar<T>(value);
    }
}

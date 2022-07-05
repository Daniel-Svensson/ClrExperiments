using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace System.Numerics
{
    public static partial class Scalar
    {
        public static T Zero<T>()
        {
            return default(T);
        }

        public static T One<T>()
        {
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)((System.SByte)1);
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)((System.Int16)1);
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)((System.Int32)1);
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)((System.Int64)1L);
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)((System.Byte)1);
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)((System.UInt16)1u);
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)((System.UInt32)1u);
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)((System.UInt64)1uL);
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)((System.Single)1.0f);
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)((System.Double)1.0d);
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)((System.Decimal)1.0m);
			}
			else
			{
				throw new NotSupportedException();
			}
        }


/*
 *        public static T Max<T>(T left, T right)
            where T : struct, IComparable<T>
        {
            return (left.CompareTo(right) <= 0) ? left : right;
        }

        public static bool LessThan<T>(T left, T right)
            where T : struct, IComparable<T>
        {
            return (left.CompareTo(right) < 0);
        }

        public static bool LessThanOrEqual<T>(T left, T right)
            where T : struct, IComparable<T>
        {
            return (left.CompareTo(right) <= 0);
        }

        public static bool GreaterThan<T>(T left, T right)
            where T : struct, IComparable<T>
        {
            return (left.CompareTo(right) > 0);
        }

        public static bool GreaterThanOrEqual<T>(T left, T right)
            where T : struct, IComparable<T>
        {
            return (left.CompareTo(right) >= 0);
        }
        */
    }
}

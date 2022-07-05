using System;
using System.Runtime.CompilerServices;

namespace System.Numerics
{
    public static partial class Scalar
    {

		///<Summary>
		/// Add performs the operation <c>left + right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Add<T>(T left, T right)
			where T : struct 
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) + ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) + ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) + ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) + ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) + ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) + ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) + ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) + ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) + ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) + ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) + ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// Subtract performs the operation <c>left - right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Subtract<T>(T left, T right)
			where T : struct 
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) - ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) - ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) - ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) - ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) - ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) - ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) - ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) - ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) - ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) - ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) - ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// Multiply performs the operation <c>left * right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Multiply<T>(T left, T right)
			where T : struct 
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) * ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) * ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) * ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) * ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) * ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) * ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) * ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) * ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) * ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) * ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) * ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// Divide performs the operation <c>left / right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Divide<T>(T left, T right)
			where T : struct 
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) / ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) / ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) / ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) / ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) / ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) / ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) / ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) / ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) / ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) / ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) / ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// Min performs the operation <c>Math.Min(left, right)</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Min<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(Math.Min(((System.SByte)(object)left), ((System.SByte)(object)right)));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(Math.Min(((System.Int16)(object)left), ((System.Int16)(object)right)));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(Math.Min(((System.Int32)(object)left), ((System.Int32)(object)right)));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(Math.Min(((System.Int64)(object)left), ((System.Int64)(object)right)));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(Math.Min(((System.Byte)(object)left), ((System.Byte)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(Math.Min(((System.UInt16)(object)left), ((System.UInt16)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(Math.Min(((System.UInt32)(object)left), ((System.UInt32)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(Math.Min(((System.UInt64)(object)left), ((System.UInt64)(object)right)));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(Math.Min(((System.Single)(object)left), ((System.Single)(object)right)));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(Math.Min(((System.Double)(object)left), ((System.Double)(object)right)));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(Math.Min(((System.Decimal)(object)left), ((System.Decimal)(object)right)));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// Max performs the operation <c>Math.Max(left, right)</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T Max<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(Math.Max(((System.SByte)(object)left), ((System.SByte)(object)right)));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(Math.Max(((System.Int16)(object)left), ((System.Int16)(object)right)));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(Math.Max(((System.Int32)(object)left), ((System.Int32)(object)right)));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(Math.Max(((System.Int64)(object)left), ((System.Int64)(object)right)));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(Math.Max(((System.Byte)(object)left), ((System.Byte)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(Math.Max(((System.UInt16)(object)left), ((System.UInt16)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(Math.Max(((System.UInt32)(object)left), ((System.UInt32)(object)right)));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(Math.Max(((System.UInt64)(object)left), ((System.UInt64)(object)right)));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(Math.Max(((System.Single)(object)left), ((System.Single)(object)right)));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(Math.Max(((System.Double)(object)left), ((System.Double)(object)right)));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(Math.Max(((System.Decimal)(object)left), ((System.Decimal)(object)right)));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// GreaterThan performs the operation <c>left > right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T GreaterThan<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) > ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) > ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) > ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) > ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) > ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) > ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) > ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) > ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) > ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) > ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) > ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// GreaterThanOrEqual performs the operation <c>left >= right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T GreaterThanOrEqual<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) >= ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) >= ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) >= ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) >= ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) >= ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) >= ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) >= ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) >= ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) >= ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) >= ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) >= ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// LessThan performs the operation <c>left < right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T LessThan<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) < ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) < ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) < ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) < ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) < ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) < ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) < ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) < ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) < ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) < ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) < ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}

		///<Summary>
		/// LessThanOrEqual performs the operation <c>left <= right</c>
		/// for the builtin types.
		///</Summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T LessThanOrEqual<T>(T left, T right)
			where T : struct , IComparable<T>
		{
			if (typeof(T) == typeof(System.SByte))
			{
				return (T)(object)(((System.SByte)(object)left) <= ((System.SByte)(object)right));
			}
			if (typeof(T) == typeof(System.Int16))
			{
				return (T)(object)(((System.Int16)(object)left) <= ((System.Int16)(object)right));
			}
			if (typeof(T) == typeof(System.Int32))
			{
				return (T)(object)(((System.Int32)(object)left) <= ((System.Int32)(object)right));
			}
			if (typeof(T) == typeof(System.Int64))
			{
				return (T)(object)(((System.Int64)(object)left) <= ((System.Int64)(object)right));
			}
			if (typeof(T) == typeof(System.Byte))
			{
				return (T)(object)(((System.Byte)(object)left) <= ((System.Byte)(object)right));
			}
			if (typeof(T) == typeof(System.UInt16))
			{
				return (T)(object)(((System.UInt16)(object)left) <= ((System.UInt16)(object)right));
			}
			if (typeof(T) == typeof(System.UInt32))
			{
				return (T)(object)(((System.UInt32)(object)left) <= ((System.UInt32)(object)right));
			}
			if (typeof(T) == typeof(System.UInt64))
			{
				return (T)(object)(((System.UInt64)(object)left) <= ((System.UInt64)(object)right));
			}
			if (typeof(T) == typeof(System.Single))
			{
				return (T)(object)(((System.Single)(object)left) <= ((System.Single)(object)right));
			}
			if (typeof(T) == typeof(System.Double))
			{
				return (T)(object)(((System.Double)(object)left) <= ((System.Double)(object)right));
			}
			if (typeof(T) == typeof(System.Decimal))
			{
				return (T)(object)(((System.Decimal)(object)left) <= ((System.Decimal)(object)right));
			}
			// For all unsupported types throw exception
			throw new NotSupportedException();
		}
    }
}


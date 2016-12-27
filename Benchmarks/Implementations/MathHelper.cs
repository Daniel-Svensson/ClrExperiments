using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Benchmarks
{
    public static class MathHelper
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AddFloat(float lhs, float rhs)
        {
            return lhs + rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float AddFloatRef(ref float lhs, ref float rhs)
        {
            return lhs + rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void IncFloat(ref float lhs, float rhs)
        {
            lhs += rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static decimal AddDecimal(decimal lhs, decimal rhs)
        {
            return lhs + rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static decimal AddDecimalRef(ref decimal lhs, ref decimal rhs)
        {
            return lhs + rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void IncDec(ref decimal lhs, decimal rhs)
        {
            lhs += rhs;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static T Add<T>(T lhs, T rhs)
            where T : struct
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

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static T Multiply<T>(T lhs, T rhs)
            where T : struct, IComparable<T>
        {
            if (typeof(T) == typeof(float))
            {
                return (T)(object)(((float)(object)lhs) * ((float)(object)rhs));
            }
            if (typeof(T) == typeof(int))
            {
                return (T)(object)(((int)(object)lhs) * ((int)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) * ((double)(object)rhs));
            }
            if (typeof(T) == typeof(long))
            {
                return (T)(object)(((long)(object)lhs) * ((long)(object)rhs));
            }
            if (typeof(T) == typeof(decimal))
            {
                return (T)(object)(((decimal)(object)lhs) * ((decimal)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) * ((double)(object)rhs));
            }
            if (typeof(T) == typeof(short))
            {
                return (T)(object)(((short)(object)lhs) * ((short)(object)rhs));
            }
            throw new NotSupportedException();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static T Subtract<T>(T lhs, T rhs)
            where T : struct
        {
            if (typeof(T) == typeof(float))
            {
                return (T)(object)(((float)(object)lhs) - ((float)(object)rhs));
            }
            if (typeof(T) == typeof(int))
            {
                return (T)(object)(((int)(object)lhs) - ((int)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) - ((double)(object)rhs));
            }
            if (typeof(T) == typeof(long))
            {
                return (T)(object)(((long)(object)lhs) - ((long)(object)rhs));
            }
            if (typeof(T) == typeof(decimal))
            {
                return (T)(object)(((decimal)(object)lhs) - ((decimal)(object)rhs));
            }
            if (typeof(T) == typeof(double))
            {
                return (T)(object)(((double)(object)lhs) - ((double)(object)rhs));
            }
            if (typeof(T) == typeof(short))
            {
                return (T)(object)(((short)(object)lhs) - ((short)(object)rhs));
            }
            throw new NotSupportedException();
        }
    }
}

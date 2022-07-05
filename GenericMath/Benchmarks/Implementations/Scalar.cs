using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Benchmarks
{    
    /// <summary>
    /// Concept DOES NOT WORK. 
    /// It seems to be at JIT time that EE exceptions are raised. 
    /// so trying to validate type by forcing constructor to runt in some way does only add overhead on each call.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    static class ScalarClass
    {
        public static void ThrowIfUnsupported<T>()
        {
            if (typeof(T) != typeof(int)
                && typeof(T) != typeof(long)
                && typeof(T) != typeof(float)
                && typeof(T) != typeof(double))
                throw new NotSupportedException();
        }

        public static T Add<T>(T a, T b)
            where T : struct
        {
            return MyClass.AddGeneric(a, b);
        }

        /// <summary>
        /// Concept DOES NOT WORK. 
        /// It seems to be at JIT time that EE exceptions are raised. 
        /// so trying to validate type by forcing constructor to runt in some way does only add overhead on each call.
        /// </summary>
        /// <typeparam name="T"></typeparam>    
        public static T SafeAdd<T>(T a, T b)
            where T : struct
        {
            ThrowIfUnsupported<T>();

            return MyClass.AddGeneric(a, b);
        }

        public static bool IsPrimitive<T>() => typeof(T).IsPrimitive;
    }

    struct ScalarStruct<T>
        where T : struct
    {
        private readonly static bool _isPrimitive = typeof(T).IsPrimitive;
        public static bool IsPrimitive => _isPrimitive;

        /// <summary>
        /// Concept DOES NOT WORK. 
        /// It seems to be at JIT time that EE exceptions are raised. 
        /// so trying to validate type by forcing constructor to runt in some way does only add overhead on each call.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        static ScalarStruct()
        {
            if (typeof(T) != typeof(int)
                && typeof(T) != typeof(long)
                && typeof(T) != typeof(float))
                throw new NotSupportedException();
        }

        public T Add(T a, T b) => MyClass.AddGeneric(a, b);

    }

    /// <summary>
    /// Concept DOES NOT WORK. 
    /// It seems to be at JIT time that EE exceptions are raised. 
    /// so trying to validate type by forcing constructor to runt in some way does only add overhead on each call.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    class ScalarClass<T>
        where T : struct
    {
        private readonly bool _isPrimitive = typeof(T).IsPrimitive;
        private readonly static ScalarClass<T> _instance = new ScalarClass<T>();

        public bool IsPrimitive => _isPrimitive;
        public static ScalarClass<T> Instance => _instance;

        private ScalarClass()
        {
            if (typeof(T) != typeof(int)
                && typeof(T) != typeof(long)
                && typeof(T) != typeof(float)
                && typeof(T) != typeof(double))
                throw new NotSupportedException();
        }

        public T Add(T a, T b) => MyClass.AddGeneric(a, b);

        public static T AddStatic(T a, T b) => Instance.Add(a, b);
    }


    /// <summary>
    /// Concept DOES NOT WORK. 
    /// It seems to be at JIT time that EE exceptions are raised. 
    /// so trying to validate type by forcing static constructor to runt in some way 
    /// does only add overhead on each call.
    /// </summary>
    struct ScalarHelper<T>
        where T : struct
    {
        private readonly T _value;

        static void ThrowIfNotSupported()
        {
            if (typeof(T) != typeof(int)
                && typeof(T) != typeof(long)
                && typeof(T) != typeof(float)
                && typeof(T) != typeof(double))
                throw new NotSupportedException();
        }


        public ScalarHelper(T value)
        {
            ThrowIfNotSupported();

            _value = value;
        }

        public T Add(T a, T b) => MyClass.AddGeneric(a, b);

        public T Add(T a) => MyClass.AddGeneric(_value, a);
        public T Add(ScalarHelper<T> a) => MyClass.AddGeneric(_value, a.Value);

        public ScalarHelper<T> Adds(ScalarHelper<T> a) => new ScalarHelper<T>(MyClass.AddGeneric(_value, a.Value));

        public T Value { get { return _value; } }
    }
}

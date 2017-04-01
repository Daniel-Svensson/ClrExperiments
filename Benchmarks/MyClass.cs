using System;
using System.Collections.Generic;
using System.Text;
using System.Numerics;

namespace Benchmarks
{
    class MyClass
    {
        public static T AddGeneric<T>(T l, T r)
            where T : struct
        {
            return Scalar.Add(l, r);
        }
    }

    public static class TypeExtensions
    {

    }
}

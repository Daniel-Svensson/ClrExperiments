using System;

namespace Benchmarks
{
    static class DynamicHelper
    {
        public static T Add<T>(T left, T right) where T : struct
        {
            return  (T)((dynamic)left + right);
        }
    }
}
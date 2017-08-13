using System;

namespace Benchmarks
{
#if !NET47
    internal class SuppressUnmanagedCodeSecurityAttribute : Attribute
    {
    }
#endif
}
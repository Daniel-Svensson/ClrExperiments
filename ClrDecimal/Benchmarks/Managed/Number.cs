// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Runtime.CompilerServices;

[assembly:  CLSCompliant(false)]
[assembly: InternalsVisibleTo("Benchmarks")]


namespace Managed
{
    internal class Number
    {
        internal static void ThrowOverflowException(string overflow_Decimal)
        {
            throw new OverflowException(overflow_Decimal);
        }
    }
}
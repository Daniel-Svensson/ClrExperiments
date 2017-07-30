// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Globalization;

namespace CoreRT
{
    internal class FormatProvider
    {
        internal static Decimal ParseDecimal(string s, NumberStyles number, IFormatProvider provider)
        {
            throw new NotImplementedException();
        }

        internal static bool TryParseDecimal(string s, NumberStyles number, object p, out Decimal result)
        {
            throw new NotImplementedException();
        }

        internal static string FormatDecimal(Decimal @decimal, string format, IFormatProvider provider)
        {
            throw new NotImplementedException();
        }

        internal static string FormatDecimal(Decimal2 decimal2, string format, IFormatProvider provider)
        {
            throw new NotImplementedException();
        }

        internal static Decimal2 ParseDecimal2(string s, NumberStyles number, IFormatProvider provider)
        {
            throw new NotImplementedException();
        }

        internal static bool TryParseDecimal2(string s, NumberStyles number, object p, out Decimal2 result)
        {
            throw new NotImplementedException();
        }
    }
}
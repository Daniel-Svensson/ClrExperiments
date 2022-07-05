// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.


using System.Runtime.Serialization;

namespace System
{
	internal static class SR
	{
		public static string XmlOnlySingleValue { get; internal set; }
		public static string XmlInvalidLowSurrogate { get; internal set; }
		public static string XmlInvalidHighSurrogate { get; internal set; }
		public static string XmlInvalidSurrogate { get; internal set; }

		internal static string Format(object xmlInvalidLowSurrogate, object v)
		{
			throw new NotImplementedException();
		}
	}
}
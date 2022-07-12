using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace ConsoleApp1
{
	public class Utf8BenchmarksLength
	{
		public Encoding? _encoding;
		private static readonly UTF8Encoding s_UTF8Encoding = new UTF8Encoding(false, true);

		private string _input = String.Empty;

		[Params(/*85*/128 / 3, /*170*/ 256 / 3, 256, 512, 2024)]
		//[Params(512, 1024, 2048)]
		public int StringLengthInChars;

		[Params(Utf8Scenario.AsciiOnly /*, Utf8Scenario.Mixed, Utf8Scenario.OnlyNonAscii*/)]
		public Utf8Scenario Scenario;

		[GlobalSetup]
		public void Setup()
		{
			_input = Scenario switch
			{
				Utf8Scenario.AsciiOnly => String.Create(StringLengthInChars, (object?)null, (chars, state) =>
				{
					int i = 0;
					foreach (ref char ch in chars)
						ch = (char)('a' + (i++ % 28));

				}),
				Utf8Scenario.Mixed => String.Create(StringLengthInChars, (object?)null, (chars, state) =>
				{
					string text = "Det här är en text med lite blandat innehåll.";
					int i = 0;
					foreach (ref char ch in chars)
						ch = text[i++ % text.Length];
				}),
				Utf8Scenario.OnlyNonAscii => String.Create(StringLengthInChars, (object?)null, (chars, state) =>
				{
					string text = "åäö";
					int i = 0;
					foreach (ref char ch in chars)
						ch = text[i++ % text.Length];
				}),
			};
		}

		[Benchmark(Baseline = true)]
		public unsafe int Original()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Length(s, _input.Length);
			}
		}

		//[Benchmark()]
		//public unsafe int Better()
		//{
		//	fixed (char* s = _input)
		//	{
		//		return UnsafeGetUTF8LengthNoCopy(s, _input.Length);
		//	}
		//}

		// Better than long after ~50 count, beats original after 16
		[Benchmark]
		public int Encoding()
		{
			return s_UTF8Encoding.GetByteCount(_input);
		}

		//[Benchmark] - slower than encoding
		//public int Systemtext()
		//{
		//	var status = System.Text.Unicode.Utf8.FromUtf16(_input, _buffer, out int _, out int bytesWritten);
		//	if (status == System.Buffers.OperationStatus.Done)
		//		return bytesWritten;
		//	else 
		//		return 0;
		//}

		//[Benchmark]
		public unsafe int Long()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Length_Long(s, _input.Length);
			}
		}

		//[Benchmark]
		public unsafe int Simd()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Length_Simd(s, _input.Length);
			}
		}

		[Benchmark]
		public unsafe int Avx1()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Length_Avx(s, _input.Length);
			}
		}

		[Benchmark]
		public unsafe int VectorLength()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Length_Vector(s, _input.Length);
			}
		}

		protected unsafe int UnsafeGetUTF8Length(char* chars, int charCount)
		{
			char* charsMax = chars + charCount;
			while (chars < charsMax)
			{
				if (*chars >= 0x80)
					break;

				chars++;
			}

			if (chars == charsMax)
				return charCount;

			char[] chArray = new char[charsMax - chars];
			for (int i = 0; i < chArray.Length; i++)
			{
				chArray[i] = chars[i];
			}
			return (int)(chars - (charsMax - charCount)) + GetByteCount(chArray);
		}

		protected unsafe int UnsafeGetUTF8LengthNoCopy(char* chars, int charCount)
		{
			char* charsMax = chars + charCount;
			while (chars < charsMax)
			{
				if (*chars >= 0x80)
					break;

				chars++;
			}

			if (chars == charsMax)
				return charCount;

			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}

		private int GetByteCount(char[] chars)
		{
			if (_encoding == null)
			{
				return s_UTF8Encoding.GetByteCount(chars);
			}
			else
			{
				return _encoding.GetByteCount(chars);
			}
		}

		private int GetByteCount2(ReadOnlySpan<char> chars)
		{
			if (_encoding == null)
			{
				return s_UTF8Encoding.GetByteCount(chars);
			}
			else
			{
				return _encoding.GetByteCount(chars);
			}
		}

		protected unsafe int UnsafeGetUTF8Length_Long(char* chars, int charCount)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			char* charsMax = chars + charCount;
			char* longMax = chars + charCount - (LongsPerChar - 1);

			while (chars < longMax)
			{
				ulong l = *(ulong*)chars;
				if ((l & Pattern) != 0)
					break;

				chars += 4;
			}

			while (chars < charsMax)
			{
				if (*chars >= 0x80)
					break;

				chars++;
			}

			if (chars == charsMax)
				return charCount;

			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}

		protected unsafe int UnsafeGetUTF8Length_Simd(char* chars, int charCount)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			char* charsMax = chars + charCount;
			char* longMax = chars + charCount - (LongsPerChar - 1);
			char* simdMax = chars + charCount - (Vector128<ushort>.Count - 1);


			if (Vector128.IsHardwareAccelerated)
			{
				if (chars < simdMax)
				{
					var mask = Vector128.Create((ushort)0xff80);
					do
					{
						var l = Vector128.Load((ushort*)chars);
						if (Sse41.IsSupported ? !Sse41.TestZ(l, mask) : !Vector128.LessThanAll(l, mask))
							break;
						chars += Vector128<ushort>.Count;
					} while (chars < simdMax);
				}
			}

			while (chars < longMax)
			{
				ulong l = *(ulong*)chars;
				if ((l & Pattern) != 0)
					break;

				chars += 4;
			}

			while (chars < charsMax)
			{
				if (*chars >= 0x80)
					break;

				chars++;
			}

			if (chars == charsMax)
				return charCount;

			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}

		protected unsafe int UnsafeGetUTF8Length_Avx(char* chars, int charCount)
		{
			char* charsMax = chars + charCount;
			char* lastSimd = chars + charCount - Vector256<ushort>.Count;

			if (Avx.IsSupported && chars <= lastSimd)
			{
				var mask = Vector256.Create((ushort)0xff80);
				while (chars < lastSimd)
				{
					var v = Vector256.Load((ushort*)chars);
					if (Avx.IsSupported ? !Avx.TestZ(v, mask) : !(Vector256.BitwiseAnd(v, mask).Equals(Vector256<ushort>.Zero)))
					{
						if (Sse41.TestZ(v.GetLower(), mask.GetLower()))
							chars += Vector128<ushort>.Count;
						goto NonAscii;
					}
					chars += Vector256<ushort>.Count;
				}

				if (Avx.TestZ(Vector256.Load((ushort*)chars), mask))
					return charCount;
				else
					goto NonAscii;
			}

			NonAscii:
			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}

		protected unsafe int UnsafeGetUTF8Length_Vector(char* chars, int charCount)
		{
			if (Vector.IsHardwareAccelerated 
				&& Vector<short>.Count > Vector128<short>.Count
/*				&& charCount <= 512, 2048*/)
			{
				char* lastSimd = chars + charCount - Vector<short>.Count;
				var mask = new Vector<short>(unchecked((short)0xff80));

				while (chars < lastSimd)
				{
					if (((*(Vector<short>*)chars) & mask) != Vector<short>.Zero)
						goto NonAscii;

					chars += Vector<short>.Count;
				}

				if ((*(Vector<short>*)lastSimd & mask) == Vector<short>.Zero)
					return charCount;
			}

		NonAscii:
			char* charsMax = chars + charCount;
			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}

		protected unsafe int UnsafeGetUTF8Length_Avx2(char* chars, int charCount)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			char* charsMax = chars + charCount;
			char* longMax = chars + charCount - (LongsPerChar - 1);
			char* simdMax = chars + charCount - (Vector256<ushort>.Count - 1);


			if (Avx.IsSupported && chars < simdMax)
			{
				var mask = Vector256.Create((ushort)0xff80);
				do
				{
					var l = Vector256.Load((ushort*)chars);
					var l2 = Vector256.Load((ushort*)chars);
					if (Avx.IsSupported ? !Avx.TestZ(l, mask) : !Vector256.LessThanAll(l, mask))
						break;
					chars += 2 * Vector256<ushort>.Count;
				} while (chars < simdMax);
			}

			while (chars < longMax)
			{
				ulong l = *(ulong*)chars;
				if ((l & Pattern) != 0)
					break;

				chars += 4;
			}

			while (chars < charsMax)
			{
				if (*chars >= 0x80)
					break;

				chars++;
			}

			if (chars == charsMax)
				return charCount;

			return (int)(chars - (charsMax - charCount)) + GetByteCount2(new ReadOnlySpan<char>(chars, (int)(charsMax - chars)));
		}
	}
}

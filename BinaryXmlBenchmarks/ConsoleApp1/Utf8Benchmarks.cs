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
	public class Utf8Benchmarks
	{
		public Encoding? _encoding;
		private static readonly UTF8Encoding s_UTF8Encoding = new UTF8Encoding(false, true);

		private string _input;
		private char[] _inputAsChars;
		private byte[] _buffer;

		[Params(5, 8, 16, 30, 34, 50, 100,/*85*/512/3)]
		public int StringLengthInChars;

		[GlobalSetup]
		public void Setup()
		{
			_input = String.Create(StringLengthInChars, (object?)null, (bytes, state) =>
			{
				int i = 0;
				foreach (ref char ch in bytes)
					ch = (char)('a' + (i++ % 28));

			});
			_inputAsChars = _input.ToCharArray();
			_buffer = new byte[StringLengthInChars * 2];
		}

		[Benchmark(Baseline = true)]
		public unsafe int Original()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8Chars(s, _input.Length, _buffer, 0);
			}
		}

		// Better than long after ~50 count, beats original after 16
		//[Benchmark]
		public int Encoding()
		{
			return s_UTF8Encoding.GetBytes(_input, 0, _input.Length, _buffer, 0);
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


		[Benchmark]
		public unsafe int Long2()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8CharsLong2(s, _input.Length, _buffer, 0);
			}
		}

		//[Benchmark]
		//public unsafe int Simd1()
		//{
		//	fixed (char* s = _input)
		//	{
		//		return UnsafeGetUTF8CharsSimd(s, _input.Length, _buffer, 0);
		//	}
		//}

		//[Benchmark]
		//public unsafe int Simd_Span()
		//{
		//	return UnsafeGetUTF8CharsSimd(_input, _buffer, 0);
		//}

		//[Benchmark]
		//public unsafe int Simd2()
		//{
		//	fixed (char* s = _input)
		//	{
		//		return UnsafeGetUTF8CharsSimd2(s, _input.Length, _buffer, 0);
		//	}
		//}


		protected unsafe int UnsafeGetUTF8Chars(char* chars, int charCount, byte[] buffer, int offset)
		{
			if (charCount > 0)
			{
				fixed (byte* _bytes = &buffer[offset])
				{
					byte* bytes = _bytes;
					byte* bytesMax = &bytes[buffer.Length - offset];
					char* charsMax = &chars[charCount];

					while (true)
					{
						while (chars < charsMax)
						{
							char t = *chars;
							if (t >= 0x80)
								break;

							*bytes = (byte)t;
							bytes++;
							chars++;
						}

						if (chars >= charsMax)
							break;

						char* charsStart = chars;
						while (chars < charsMax && *chars >= 0x80)
						{
							chars++;
						}

						bytes += (_encoding ?? s_UTF8Encoding).GetBytes(charsStart, (int)(chars - charsStart), bytes, (int)(bytesMax - bytes));

						if (chars >= charsMax)
							break;
					}

					return (int)(bytes - _bytes);
				}
			}
			return 0;
		}

		protected unsafe int UnsafeGetUTF8CharsLong2(char* chars, int charCount, byte[] buffer, int offset)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			if (charCount > 0)
			{
				fixed (byte* _bytes = &buffer[offset])
				{
					byte* bytes = _bytes;
					byte* bytesMax = &bytes[buffer.Length - offset];
					char* charsMax = &chars[charCount];
					char* longMax = &chars[charCount - (LongsPerChar - 1)];

					while (chars < longMax)
					{
						ulong l = *(ulong*)chars;
						if ((l & Pattern) != 0)
							break;

						// 0x00aa00bb_00cc00dd => 0x00aaaabb_bbccccdd
						l = l | (l >> 8);
						*(ushort*)bytes = (ushort)l;
						*(ushort*)(bytes + 2) = (ushort)(l >> 32);
						bytes += 4;
						chars += 4;
					}

					while (chars < charsMax)
					{
						char t = *chars;
						if (t >= 0x80)
							break;

						*bytes = (byte)t;
						bytes++;
						chars++;
					}

					if (chars < charsMax)
						bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));

					return (int)(bytes - _bytes);
				}
			}
			return 0;
		}

		protected unsafe int UnsafeGetUTF8CharsSimd(char* chars, int charCount, byte[] buffer, int offset)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			if (charCount > 0)
			{
				fixed (byte* _bytes = &buffer[offset])
				{
					byte* bytes = _bytes;
					byte* bytesMax = &bytes[buffer.Length - offset];
					char* charsMax = &chars[charCount];
					char* simdMax = &chars[charCount - (Vector128<ushort>.Count - 1)];
					char* longMax = &chars[charCount - (LongsPerChar - 1)];

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

								*((long*)bytes) = Vector128.Narrow(l, l)
									.AsInt64()
									.GetElement(0);
								bytes += Vector128<ushort>.Count;
								chars += Vector128<ushort>.Count;
							} while (chars < simdMax);
						}
					}

					while (chars < longMax)
					{
						ulong l = *(ulong*)chars;
						if ((l & Pattern) != 0)
							break;

						// 0x00aa00bb_00cc00dd => 0x00aaaabb_bbccccdd
						l = l | (l >> 8);
						*(ushort*)bytes = (ushort)l;
						*(ushort*)(bytes + 2) = (ushort)(l >> 32);
						bytes += 4;
						chars += 4;
					}

					while (chars < charsMax)
					{
						char t = *chars;
						if (t >= 0x80)
							break;

						*bytes = (byte)t;
						bytes++;
						chars++;
					}

					if (chars < charsMax)
						bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));

					return (int)(bytes - _bytes);
				}
			}
			return 0;
		}

		protected unsafe int UnsafeGetUTF8CharsSimd(ReadOnlySpan<char> chars, byte[] buffer, int offset)
		{
			if (chars.Length > 0)
			{
				fixed (byte* _bytes = &buffer[offset])
				{
					byte* bytes = _bytes;
					byte* bytesMax = &bytes[buffer.Length - offset];
					int charIdx = 0;

					if (Vector128.IsHardwareAccelerated && chars.Length >= Vector128<ushort>.Count)
					{
						var vectors = MemoryMarshal.Cast<char, Vector128<ushort>>(chars);
						var mask = Vector128.Create((ushort)0xff80);
						int i = 0;
						for (; i < vectors.Length; ++i)
						{
							var l = vectors[i];
							if (!Sse41.TestZ(l, mask))
								break;

							*((long*)bytes) = Vector128.Narrow(l, l)
								.AsInt64()
								.GetElement(0);
							bytes += Vector128<ushort>.Count;
						}
						charIdx = i * Vector128<ushort>.Count;
					}

					for (; charIdx < chars.Length; ++charIdx)
					{
						char t = chars[charIdx];
						if (t >= 0x80)
							break;

						*bytes = (byte)t;
						bytes++;
					}

					if (charIdx < chars.Length)
						bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars.Slice(charIdx), new Span<byte>(bytes, (int)(bytesMax - bytes)));

					return (int)(bytes - _bytes);
				}
			}
			return 0;
		}

		protected unsafe int UnsafeGetUTF8CharsSimd2(char* chars, int charCount, byte[] buffer, int offset)
		{
			const int LongsPerChar = 4;
			const ulong Pattern = 0xff80ff80ff80ff80;

			if (charCount > 0)
			{
				fixed (byte* _bytes = &buffer[offset])
				{
					byte* bytes = _bytes;
					byte* bytesMax = &bytes[buffer.Length - offset];
					char* charsMax = &chars[charCount];
					char* longMax = &chars[charCount - (LongsPerChar - 1)];
					char* simdMax = &chars[charCount - (2 * Vector128<ushort>.Count - 1)];


					var mask = Vector128.Create((ushort)0xff80);
					while (chars < simdMax)
					{
						var v1 = Vector128.Load((ushort*)chars);
						var v2 = Vector128.Load((ushort*)(chars + Vector128<ushort>.Count));
						if (!Sse41.TestZ(v1, mask))
							break;

						if (!Sse41.TestZ(v2, mask))
						{
							*((long*)bytes) = Vector128.Narrow(v1, v1)
							.AsInt64()
							.GetElement(0);
							bytes += Vector128<ushort>.Count;
							chars += Vector128<ushort>.Count;
							break;
						}
						else
						{
							Vector128.Narrow(v1, v2)
								.Store(bytes);
							bytes += 2 * Vector128<ushort>.Count;
							chars += 2 * Vector128<ushort>.Count;
						}
					}

					while (chars < longMax)
					{
						ulong l = *(ulong*)chars;
						if ((l & Pattern) != 0)
							break;

						bytes[0] = (byte)l;
						bytes[1] = (byte)((uint)l >> 16);
						bytes[2] = (byte)(l >> 32);
						bytes[3] = (byte)(l >> 48);
						bytes += 4;
						chars += 4;
					}

					while (chars < charsMax)
					{
						char t = *chars;
						if (t >= 0x80)
							break;

						*bytes = (byte)t;
						bytes++;
						chars++;
					}

					if (chars < charsMax)
						bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));

					return (int)(bytes - _bytes);
				}
			}
			return 0;
		}

	}
}

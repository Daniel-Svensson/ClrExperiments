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
    public enum Utf8Scenario
    {
        AsciiOnly = 0,
        Mixed = 1,
        OnlyNonAscii = 2,
    }
    public class Utf8Benchmarks
    {
        public Encoding? _encoding;
        private static readonly UTF8Encoding s_UTF8Encoding = new UTF8Encoding(false, true);

        private string _input;
        private char[] _inputAsChars;
        private byte[] _buffer;

        [Params(5, 8, 12, 16, 20, 30, 34, 50, /*85*/256 / 3,/*170*/512 / 3)]
        public int StringLengthInChars;

        [Params(Utf8Scenario.AsciiOnly, Utf8Scenario.Mixed, Utf8Scenario.OnlyNonAscii)]
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
        [Benchmark]
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
        public unsafe int Long()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsLong2(s, _input.Length, _buffer, 0);
            }
        }

		[Benchmark]
		public unsafe int SimdGeneric()
		{
			fixed (char* s = _input)
			{
				return UnsafeGetUTF8CharsSimd(s, _input.Length, _buffer, 0);
			}
		}

        [Benchmark]
        public unsafe int SimdSSE()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdSSE(s, _input.Length, _buffer, 0);
            }
        }

		//[Benchmark]
		//public unsafe int Simd_Span()
		//{
		//	return UnsafeGetUTF8CharsSimd(_input, _buffer, 0);
		//}

        [Benchmark]
        public unsafe int Simd2()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimd2(s, _input.Length, _buffer, 0);
            }
        }

        [Benchmark]
        public unsafe int SimdAVX()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdAVX(s, _input.Length, _buffer, 0);
            }
        }


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

                                if (/*Sse41.IsSupported */ false ? !Sse41.TestZ(l, mask) : !(Vector128.BitwiseAnd(l, mask) == Vector128<ushort>.Zero))
                                    break;

								*((long*)bytes) = Vector128.Narrow(l, l).AsInt64().ToScalar();
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

        protected unsafe int UnsafeGetUTF8CharsSimdSSE(char* chars, int charCount, byte[] buffer, int offset)
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
                            var mask = Vector128.Create(unchecked((short)0xff80));
                            do
                            {
                                var v = Sse41.LoadVector128((short*)chars);
                                if (!Sse41.TestZ(v, mask))
                                {
                                    //									var m = Sse42.MoveMask(Sse42.CompareLessThan(mask, v).AsByte());
                                    break;
                                }

                                Sse41.StoreScalar((long*)bytes, Sse41.PackUnsignedSaturate(v, v).AsInt64());
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


                    var mask = Vector128.Create(unchecked((short)0xff80));
                    while (chars < simdMax)
                    {
                        var v1 = Vector128.Load((short*)chars);
                        var v2 = Vector128.Load((short*)(chars + Vector128<ushort>.Count));
                        if (!Sse41.TestZ(v1, mask))
                            break;

                        if (!Sse41.TestZ(v2, mask))
                        {
                            Sse41.StoreScalar((long*)bytes, Sse41.PackUnsignedSaturate(v1, v1).AsInt64());
                            bytes += Vector128<ushort>.Count;
                            chars += Vector128<ushort>.Count;
                            break;
                        }
                        else
                        {
                            Sse41.PackUnsignedSaturate(v1, v2).Store(bytes);
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

        protected unsafe int UnsafeGetUTF8CharsSimdAVX(char* chars, int charCount, byte[] buffer, int offset)
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
                    char* simdMax = &chars[charCount - (Vector256<ushort>.Count - 1)];

                    if (chars < simdMax)
                    {
                        var mask = Vector256.Create(unchecked((short)0xff80));
                        do
                        {
                            var v = Avx2.LoadVector256((short*)chars);
                            if (!Avx2.TestZ(v, mask))
                            {
                                //									var m = Sse42.MoveMask(Sse42.CompareLessThan(mask, v).AsByte());
                                break;
                            }

                            var packed = Avx2.PackUnsignedSaturate(v, v);
                            packed.GetLower().Store(bytes);
                            bytes += Vector256<ushort>.Count;
                            chars += Vector256<ushort>.Count;
                        } while (chars < simdMax);
                    }

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

    }
}

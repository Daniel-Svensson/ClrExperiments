﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
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
        sealed class SealedUTF : UTF8Encoding
        {
            public SealedUTF() : base(encoderShouldEmitUTF8Identifier: false, throwOnInvalidBytes: true)
            { }


        }

        public Encoding? _encoding;
        private static readonly UTF8Encoding s_UTF8Encoding = new UTF8Encoding(false, true);
        private static readonly SealedUTF s_UTF8EncodingSealed = new SealedUTF();

        private string _input;
        private char[] _inputAsChars;
        private byte[] _buffer;

//        [Params(5, 8, 34, 85)]
        [Params(5, 8, /*16,*/ /*23, 32,*/ 20, 85)]
        //[Params(5, 8, 12, 16, 20, 30, 34, 50, /*85*/256 / 3,/*170*/512 / 3)]
        public int StringLengthInChars;

        [Params(Utf8Scenario.AsciiOnly/*, Utf8Scenario.Mixed*/)]
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

        [Benchmark]
        public unsafe int Original()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8Chars(s, _input.Length, _buffer, 0);
            }
        }

        // Better than long after ~50 count, beats original after 16
        [Benchmark]
        public unsafe int EncodingNullCoal()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return (_encoding ?? s_UTF8EncodingSealed).GetBytes(s, _input.Length, b, _buffer.Length); ;
        }


        [Benchmark]
        public unsafe int Encoding_If_Ptr_UTF8()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
            {
                if (_encoding == null)
                {
                    return Encoding.UTF8.GetBytes(s, _input.Length, b, _buffer.Length);
                }
                else
                {
                    return _encoding.GetBytes(s, _input.Length, b, _buffer.Length);
                }
            }
        }

        //[Benchmark]
        public unsafe int Encoding_Span()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return s_UTF8Encoding.GetBytes(new Span<char>(s, _input.Length), new Span<byte>(b, _buffer.Length));
        }

        [Benchmark]
        public unsafe int SealedEncoding()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return s_UTF8EncodingSealed.GetBytes(s, _input.Length, b, _buffer.Length); ;
        }

        [Benchmark]
        public unsafe int EncodingNullCoal_UTF8()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return (_encoding ?? Encoding.UTF8).GetBytes(s, _input.Length, b, _buffer.Length);
        }

        [Benchmark]
        public unsafe int SealedEncoding_UTF8()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return Encoding.UTF8.GetBytes(s, _input.Length, b, _buffer.Length); ;
        }

        //[Benchmark]
        public unsafe int SealedEncoding_Span()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return s_UTF8EncodingSealed.GetBytes(_input, _buffer);
        }

        //[Benchmark]
        public int SealedEncoding_Span_NoPinning()
        {
            return s_UTF8EncodingSealed.GetBytes(_input.AsSpan(), _buffer.AsSpan());
        }


        //[Benchmark]
        public int UTF8_transform()
        {
            var res = System.Text.Unicode.Utf8.FromUtf16(_input, _buffer, out int charsWritten, out int bytesWritten, replaceInvalidSequences: false);
            if (res == System.Buffers.OperationStatus.Done)
                return bytesWritten;
            else // Throw correct exception
                return s_UTF8EncodingSealed.GetBytes(_input.AsSpan(charsWritten), _buffer.AsSpan(bytesWritten));
        }

        //[Benchmark]
        public int AScII_transform()
        {
            var res = System.Text.Ascii.FromUtf16(_input, _buffer, out int bytesWritten);
            if (res == System.Buffers.OperationStatus.Done)
                return bytesWritten;
            else // Throw correct exception
                return s_UTF8Encoding.GetBytes(_input.AsSpan(bytesWritten), _buffer.AsSpan(bytesWritten));
        }

        public unsafe int EncodingUTF8()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return System.Text.Encoding.UTF8.GetBytes(s, _input.Length, b, _buffer.Length);
        }

        //[Benchmark]
        public unsafe int EncodingUTF8_Span()
        {
            fixed (char* s = _input)
            fixed (byte* b = _buffer)
                return System.Text.Encoding.UTF8.GetBytes(new Span<char>(s, _input.Length), new Span<byte>(b, _buffer.Length));
        }

        //[Benchmark]
        public unsafe int SimdGeneric()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimd(s, _input.Length, _buffer, 0);
            }
        }

        //[Benchmark()]
        public unsafe int SimdSSE_v4()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdSSE_v4(s, _input.Length, _buffer, 0);
            }
        }

        //[Benchmark]
        public unsafe int SimdSSE_2_128()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSSE_2_128(s, _input.Length, _buffer, 0);
            }
        }

        //[Benchmark(Baseline = true)]
        public unsafe int SimdAVX()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdAVX(s, _input.Length, _buffer, 0);
            }
        }


        //[Benchmark]
        public unsafe int SimdAVX_2()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdAVX2(s, _input.Length, _buffer, 0);
            }
        }


        //[Benchmark]
        public unsafe int SimdVector256()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdAVX_3(s, _input.Length, _buffer, 0);
            }
        }


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

        protected unsafe int UnsafeGetUTF8CharsNEW(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                // Fast path for small strings, skip and use Encoding.GetBytes for larger strings since it is faster even for the all-Ascii case
                if (charCount < 16)
                {
                    fixed (byte* _bytes = &buffer[offset])
                    {
                        byte* bytes = _bytes;
                        char* charsMax = &chars[charCount];

                        while (chars < charsMax)
                        {
                            char t = *chars;
                            if (t >= 0x80)
                                goto NonAscii;

                            *bytes = (byte)t;
                            bytes++;
                            chars++;
                        }
                        return charCount;

                    NonAscii:
                        byte* bytesMax = _bytes + buffer.Length - offset;
                        return (int)(bytes - _bytes)
                            + (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    }
                }
                else
                {
                    return (_encoding ?? s_UTF8Encoding).GetBytes(new ReadOnlySpan<char>(chars, charCount), buffer);
                }
            }
            return 0;
        }


        protected unsafe int UnsafeGetUTF8Chars2(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    char* charsMax = &chars[charCount];

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
                    {
                        byte* bytesMax = &bytes[buffer.Length - offset];
                        bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    }

                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }


        protected unsafe int UnsafeGetUTF8Chars_10(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                // Fast path for small strings, skip and use Encoding.GetBytes for larger strings since it is faster even for the all-Ascii case
                if (charCount < 10)
                {
                    fixed (byte* _bytes = &buffer[offset])
                    {
                        byte* bytes = _bytes;
                        char* charsMax = &chars[charCount];

                        while (chars < charsMax)
                        {
                            char t = *chars;
                            if (t >= 0x80)
                                goto NonAscii;

                            *bytes = (byte)t;
                            bytes++;
                            chars++;
                        }
                        return charCount;

                    NonAscii:
                        byte* bytesMax = _bytes + buffer.Length - offset;
                        return (int)(bytes - _bytes)
                            + (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    }
                }
                else
                {
                    return (_encoding ?? s_UTF8Encoding).GetBytes(new ReadOnlySpan<char>(chars, charCount), buffer);
                }
            }
            return 0;
        }


        protected unsafe int UnsafeGetUTF8Chars_30(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                // Fast path for small strings, skip and use Encoding.GetBytes for larger strings since it is faster even for the all-Ascii case
                if (charCount < 60)
                {
                    fixed (byte* _bytes = &buffer[offset])
                    {
                        byte* bytes = _bytes;
                        char* charsMax = &chars[charCount];

                        while (chars < charsMax)
                        {
                            char t = *chars;
                            if (t >= 0x80)
                                goto NonAscii;

                            *bytes = (byte)t;
                            bytes++;
                            chars++;
                        }
                        return charCount;

                    NonAscii:
                        byte* bytesMax = _bytes + buffer.Length - offset;
                        return (int)(bytes - _bytes)
                            + (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    }
                }
                else
                {
                    return (_encoding ?? s_UTF8Encoding).GetBytes(new ReadOnlySpan<char>(chars, charCount), buffer);
                }
            }
            return 0;
        }



        protected unsafe int UnsafeGetUTF8CharsSimd(char* chars, int charCount, byte[] buffer, int offset)
        {

            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    byte* bytesMax = &bytes[buffer.Length - offset];
                    char* charsMax = &chars[charCount];
                    char* lastSimd = &chars[charCount - Vector128<ushort>.Count];

                    if (chars <= lastSimd)
                    {
                        var mask = Vector128.Create(unchecked((short)0xff80));
                        var mask2 = Vector128.Create(unchecked((short)0x00ff));

                        do
                        {
                            var l = Vector128.Load((short*)chars);

                            if (!Vector128.BitwiseAnd(l, mask).Equals(Vector128<short>.Zero))
                                break;

                            // This trick ensures we get 2 vpand instructions 
                            // otherwise Narrow will load 0x00ff from memory and do vpAnd
                            // TODO: Beter to use SSe2 here
                            l = Vector128.BitwiseAnd(l, mask2);
                            *((long*)bytes) = Vector128.Narrow(l, l).AsInt64().ToScalar();
                            bytes += Vector128<short>.Count;
                            chars += Vector128<short>.Count;
                        } while (chars <= lastSimd);
                    }


                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }


        protected unsafe int UnsafeGetUTF8CharsSimdSSE_v4(char* chars, int charCount, byte[] buffer, int offset)
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
                    char* simdLast = chars + charCount - Vector128<ushort>.Count;
                    char* longMax = &chars[charCount - (LongsPerChar - 1)];

                    if (Vector128.IsHardwareAccelerated)
                    {
                        if (chars <= simdLast)
                        {
                            var mask = Vector128.Create(unchecked((short)0xff80));

                            while (chars < simdLast)
                            {
                                var v = Sse2.LoadVector128((short*)chars);
                                if (!Sse41.TestZ(v, mask))
                                    goto NonAscii;

                                Sse2.StoreScalar((long*)bytes, Sse2.PackUnsignedSaturate(v, v).AsInt64());
                                bytes += Vector128<ushort>.Count;
                                chars += Vector128<ushort>.Count;
                            }

                            var v2 = Sse2.LoadVector128((short*)simdLast);
                            if (!Sse41.TestZ(v2, mask))
                                goto NonAscii;

                            Sse2.StoreScalar((long*)(_bytes + charCount - sizeof(long)), Sse2.PackUnsignedSaturate(v2, v2).AsInt64());
                            return charCount;
                        }
                    }

                    while (chars < longMax)
                    {
                        ulong l = *(ulong*)chars;
                        if ((l & Pattern) != 0)
                            goto NonAscii;

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
                            goto NonAscii;

                        *bytes = (byte)t;
                        bytes++;
                        chars++;
                    }

                    return charCount;
                NonAscii:
                    if (chars < charsMax)
                        bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));

                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }

        protected unsafe int UnsafeGetUTF8CharsSSE_2_128(char* chars, int charCount, byte[] buffer, int offset)
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
                    char* simdMax256 = &chars[charCount - (2 * Vector128<ushort>.Count - 1)];


                    var mask = Vector128.Create(unchecked((short)0xff80));
                    while (chars < simdMax256)
                    {
                        var v1 = Vector128.Load((short*)chars);
                        var v2 = Vector128.Load((short*)(chars + Vector128<ushort>.Count));
                        if (!Sse41.TestZ(Sse2.Or(v1, v2), mask))
                        {
                            if (Sse41.TestZ(v1, mask))
                            {
                                Sse2.StoreScalar((long*)bytes, Sse2.PackUnsignedSaturate(v1, v1).AsInt64());
                                bytes += Vector128<ushort>.Count;
                                chars += Vector128<ushort>.Count;
                            }
                            break;
                        }

                        Sse2.PackUnsignedSaturate(v1, v2).Store(bytes);
                        bytes += 2 * Vector128<ushort>.Count;
                        chars += 2 * Vector128<ushort>.Count;
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

        protected unsafe int UnsafeGetUTF8CharsSimdAVX(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    byte* bytesMax = &bytes[buffer.Length - offset];
                    char* charsMax = &chars[charCount];

                    if (charCount >= 8)
                    {
                        var mask = Vector256.Create(unchecked((short)0xff80));
                        if (/*charCount >= 8 &&*/ charCount <= 16)
                        {
                            var v1 = Sse2.LoadVector128((short*)chars);
                            var v2 = Sse2.LoadVector128((short*)(chars + charCount - Vector128<ushort>.Count));
                            if (!Sse41.TestZ(Sse2.Or(v1, v2), mask.GetLower()))
                                goto NonAscii;

                            var packed = Sse2.PackUnsignedSaturate(v1, v2).AsInt64();
                            Sse2.StoreScalar((long*)bytes, packed);
                            Sse2.StoreHigh((double*)(bytesMax - sizeof(long)), packed.AsDouble());
                            return charCount;
                        }
                        else // > 16
                        {
                            char* simdLast = &chars[charCount - Vector256<ushort>.Count];

                            while (chars < simdLast)
                            {
                                var v = Avx.LoadVector256((short*)chars);
                                if (!Avx.TestZ(v, mask))
                                    goto NonAscii;

                                var packed = Avx2.PackUnsignedSaturate(v, v).AsInt64();
                                Sse2.StoreScalar((long*)bytes, packed.GetLower());
                                Sse2.StoreScalar((long*)(bytes + 8), packed.GetUpper());
                                bytes += Vector256<ushort>.Count;
                                chars += Vector256<ushort>.Count;
                            }

                            var v2 = Avx.LoadVector256((short*)simdLast);
                            if (!Avx.TestZ(v2, mask))
                                goto NonAscii;

                            var packedLast = Avx2.PackUnsignedSaturate(v2, v2).AsInt64();
                            // or 
                            //packedLast = Avx2.Permute4x64(packedLast, 0x02 /*d8*/);
                            //packedLast.Store(bytes);
                            Sse41.StoreScalar((long*)bytes, packedLast.GetLower());
                            Sse41.StoreScalar((long*)(bytes + 8), packedLast.GetUpper());
                            return charCount;
                        }
                    }

                    char* longMax = &chars[charCount - 3];
                    while (chars < longMax)
                    {
                        ulong l = *(ulong*)chars;
                        if ((l & 0xff80ff80ff80ff80) != 0)
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
                            goto NonAscii;

                        *bytes = (byte)t;
                        bytes++;
                        chars++;
                    }

                    return charCount;

                NonAscii:

                    bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }



        protected unsafe int UnsafeGetUTF8CharsSimdAVX2(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    byte* bytesMax = &bytes[buffer.Length - offset];
                    char* charsMax = &chars[charCount];

                    if (charCount >= 8)
                    {
                        var mask = Vector256.Create(unchecked((short)0xff80));
                        if (/*charCount >= 8 &&*/ charCount <= 16)
                        {
                            var v1 = Sse2.LoadVector128((short*)chars);
                            var v2 = Sse2.LoadVector128((short*)(chars + charCount - Vector128<ushort>.Count));
                            if (!Sse41.TestZ(v1 | v2, mask.GetLower()))
                                goto NonAscii;

                            var packed = FastNarrow(v1, v2).AsDouble();
                            Sse2.StoreScalar((double*)bytes, packed);
                            Sse2.StoreHigh((double*)(_bytes + charCount - sizeof(long)), packed);
                            return charCount;
                        }
                        else // > 16
                        {
                            char* simdLast = &chars[charCount - Vector256<ushort>.Count];

                            while (chars < simdLast)
                            {
                                var v = *(Vector256<short>*)chars;
                                if (!Avx.TestZ(v, mask))
                                    goto NonAscii;

                                FastNarrow(v).GetLower().Store(bytes);
                                bytes += Vector256<ushort>.Count;
                                chars += Vector256<ushort>.Count;
                            }

                            var v2 = *(Vector256<short>*)simdLast;
                            if (!Avx.TestZ(v2, mask))
                                goto NonAscii;

                            FastNarrow(v2).GetLower().Store(_bytes + charCount - Vector128<byte>.Count);
                            return charCount;
                        }
                    }

                    while (chars < charsMax)
                    {
                        char t = *chars;
                        if (t >= 0x80)
                            goto NonAscii;

                        *bytes = (byte)t;
                        bytes++;
                        chars++;
                    }

                    return charCount;

                NonAscii:

                    bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        static Vector256<byte> FastNarrow(Vector256<short> v)
        {
            return Avx2.IsSupported ? Avx2.Permute4x64(Avx2.PackUnsignedSaturate(v, v).AsInt64(), 0x02 /*d8*/).AsByte()
                 : Vector256.Narrow(v.AsUInt16(), v.AsUInt16());
        }
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        static Vector128<byte> FastNarrow(Vector128<short> lower, Vector128<short> upper)
        {
            return Avx2.IsSupported ? Avx2.PackUnsignedSaturate(lower, upper)
                 : Vector128.Narrow(lower.AsUInt16(), upper.AsUInt16());
        }


        protected unsafe int UnsafeGetUTF8CharsSimdAVX_3(char* chars, int charCount, byte[] buffer, int offset)
        {
            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    byte* bytesMax = &bytes[buffer.Length - offset];
                    char* charsMax = &chars[charCount];

                    if (charCount >= 8)
                    {
                        var mask = Vector256.Create(unchecked((ushort)0xff80));
                        if (/*charCount >= 8 &&*/ charCount <= 16)
                        {
                            var v1 = *(Vector128<short>*)chars;
                            var v2 = *(Vector128<short>*)(chars + charCount - Vector128<ushort>.Count);
                            if (((v1 | v2) & mask.AsInt16().GetLower()) != Vector128<short>.Zero)
                                goto NonAscii;

                            var packed = Vector128.Narrow(v1, v2).AsDouble();
                            *(double*)bytes = packed.ToScalar();
                            *(double*)(_bytes + charCount - sizeof(double)) = packed.GetElement(1);
                            return charCount;
                        }
                        else // > 16
                        {
                            char* simdLast = &chars[charCount - Vector256<ushort>.Count];

                            while (chars < simdLast)
                            {
                                var v = *(Vector256<ushort>*)chars;
                                if ((v & mask) != Vector256<ushort>.Zero)
                                    goto NonAscii;

                                Vector256.Narrow(v, v).GetLower().Store(bytes);
                                bytes += Vector256<ushort>.Count;
                                chars += Vector256<ushort>.Count;
                            }

                            var v2 = *(Vector256<ushort>*)simdLast;
                            if (!Avx.TestZ(v2, mask))
                                goto NonAscii;

                            Vector256.Narrow(v2, v2).GetLower().Store(_bytes + charCount - Vector128<byte>.Count);
                            return charCount;
                        }
                    }

                    while (chars < charsMax)
                    {
                        char t = *chars;
                        if (t >= 0x80)
                            goto NonAscii;

                        *bytes = (byte)t;
                        bytes++;
                        chars++;
                    }

                    return charCount;

                NonAscii:

                    bytes += (_encoding ?? s_UTF8Encoding).GetBytes(chars, (int)(charsMax - chars), bytes, (int)(bytesMax - bytes));
                    return (int)(bytes - _bytes);
                }
            }
            return 0;
        }

    }
}

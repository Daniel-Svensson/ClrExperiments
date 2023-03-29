using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using Iced.Intel;

namespace ConsoleApp1
{
    public partial class AsciiBenchmarks
    {
        public Encoding? _encoding;
        private static readonly SealedEncoding s_UTF8Encoding = new SealedEncoding(false, true);
        private static readonly Encoding s_nonSealedUTF8Encoding = new UTF8Encoding(false, true);

        public string _input;
        public char[] _inputAsChars;
        public byte[] _buffer;

        //[Params(18, 24, 25, 31, 32)]
        [Params(16, 20, 25, 32)]
        //[Params(5, 8, 12, 16, 20, 30, 34, 50, /*85256 / 3,/*170*/512 / 3)]
        public int StringLengthInChars;

        [Params(Utf8Scenario.AsciiOnly)]
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

        //[Benchmark(Baseline = true)]
        public unsafe int System_Text_Ascii()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
            {
                System.Text.Ascii.FromUtf16(new ReadOnlySpan<char>(s, _input.Length), new Span<byte>(_bytes, _buffer.Length), out int bytesWritten);
                return bytesWritten;
            }
        }

        // [Benchmark]
        public unsafe int Ascii_Local()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
            {
                Test.Ascii.FromUtf16(new ReadOnlySpan<char>(s, _input.Length), new Span<byte>(_bytes, _buffer.Length), out int bytesWritten);
                return bytesWritten;
            }
        }

        [Benchmark(Baseline = true)]
        public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v2()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
                return Test.Ascii.NarrowUtf16ToAscii_Original(s, _bytes, (uint)Math.Min(_input.Length, _buffer.Length));
        }

        [Benchmark]
        public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_simple_loop()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
                return Test.Ascii.NarrowUtf16ToAscii(s, _bytes, (uint)Math.Min(_input.Length, _buffer.Length));
        }

        //[Benchmark]
        public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v3()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
                return Test.Ascii.NarrowUtf16ToAscii_v3_store(s, _bytes, (uint)Math.Min(_input.Length, _buffer.Length));
        }

        [Benchmark]
        public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v4_if()
        {
            fixed (char* s = _input)
            fixed (byte* _bytes = _buffer)
                return Test.Ascii.NarrowUtf16ToAscii_v4(s, _bytes, (uint)Math.Min(_input.Length, _buffer.Length));
        }


        // [Benchmark]
        public unsafe nint SSE_v42()
        {
            fixed (char* s = _input)
                return UnsafeGetUTF8CharsSimdSSE_v42(s, _input.Length, _buffer, 0);
        }

        // [Benchmark]// - slower than encoding
        public int System_Text_Utf8()
        {
            System.Text.Unicode.Utf8.FromUtf16(_input, _buffer, out int _, out int bytesWritten);
            return bytesWritten;
        }

        static readonly Vector128<short> Mask128 = Vector128.Create(unchecked((short)0xff80));
        static readonly Vector256<short> Mask256 = Vector256.Create(unchecked((short)0xff80));

        protected unsafe nint UnsafeGetUTF8CharsSimdSSE_v42(char* chars, nint charCount, byte[] buffer, nint offset)
        {
            if (charCount > 0)
            {
                fixed (byte* _bytes = &buffer[offset])
                {
                    byte* bytes = _bytes;
                    byte* bytesMax = &bytes[buffer.Length - offset];
                    char* charsMax = &chars[charCount];
                    char* simdLast = chars + charCount - Vector128<ushort>.Count;

                    uint i = 0;
                    if (Vector128.IsHardwareAccelerated)
                    {
                        if (charCount >= Vector128<ushort>.Count)
                        {
                            // Read from static variable to prevent multiple reads
                            var mask = Mask128;

                            //					nuint numIterations = (uint)(charCount - 1) / (uint)Vector128<ushort>.Count;
                            uint lastSimd = (uint)(charCount - Vector128<short>.Count);
                            for (i = 0; i < lastSimd; i += (uint)Vector128<ushort>.Count)
                            {
                                var v = *(Vector128<short>*)(chars + i);
                                if (!Sse41.TestZ(v, mask))
                                    goto NonAscii;

                                Sse2.StoreScalar((long*)(bytes + i), Sse2.PackUnsignedSaturate(v, v).AsInt64());
                            }

                            var v2 = *(Vector128<short>*)(chars + charCount - Vector128<ushort>.Count);
                            if (!Sse41.TestZ(v2, mask))
                                goto NonAscii;

                            var packed = Sse2.PackUnsignedSaturate(v2, v2).AsInt64();
                            Sse2.StoreScalar((long*)(_bytes + charCount - sizeof(long)), packed);
                            return charCount;
                        }
                    }


                    for (; i < (uint)charCount; ++i)
                    {
                        char t = chars[i];
                        if (t >= 0x80)
                            goto NonAscii;

                        bytes[i] = (byte)t;
                    }

                    return charCount;

                NonAscii:
                    return (int)i + (_encoding ?? s_UTF8Encoding).GetBytes(chars + i, (int)(charCount - i), bytes + i, (buffer.Length - (int)(offset + (int)i)));
                }
            }
            return 0;
        }



        //[Benchmark]
        public unsafe int SimdAVX2()
        {
            fixed (char* s = _input)
            {
                return UnsafeGetUTF8CharsSimdAVX_Narrow2(s, _input.Length, _buffer, 0);
            }
        }

        protected unsafe int UnsafeGetUTF8CharsSimdAVX_Narrow2(char* chars, int charCount, byte[] buffer, int offset)
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
                        uint i = 0;
                        var mask = Mask256;
                        if (/*charCount >= 8 &&*/ charCount <= 16)
                        {
                            var v1 = Sse2.LoadVector128((short*)chars);
                            var v2 = Sse2.LoadVector128((short*)(chars + charCount - Vector128<ushort>.Count));
                            if (!Sse41.TestZ(v1 | v2, mask.GetLower()))
                            {
                                if (Sse41.TestZ(v1, mask.GetLower()))
                                    i = (uint)Vector128<ushort>.Count;
                                goto NonAscii_Vector;
                            }

                            var packed = FastNarrow(v1, v2).AsDouble();
                            Sse2.StoreScalar((double*)bytes, packed);
                            Sse2.StoreHigh((double*)(_bytes + charCount - sizeof(long)), packed);
                            return charCount;
                        }
                        else // > 16
                        {
                            uint lastSimd = (uint)(charCount - Vector256<short>.Count);
                            for (i = 0; i < lastSimd; i += (uint)Vector256<ushort>.Count)
                            {
                                var v = *(Vector256<short>*)(chars + i);
                                if (!Avx.TestZ(v, mask))
                                    goto NonAscii_Vector;

                                FastNarrowAndStore(v, bytes + i);
                            }

                            var v2 = *(Vector256<short>*)(chars + charCount - Vector256<ushort>.Count);
                            if (!Avx.TestZ(v2, mask))
                                goto NonAscii;

                            FastNarrowAndStore(v2, _bytes + charCount - Vector128<byte>.Count);
                            return charCount;
                        }

                    NonAscii_Vector:
                        chars += i;
                        bytes += i;
                        goto NonAscii;
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
        static unsafe void FastNarrowAndStore(Vector256<short> v, byte* destination)
        {
            FastNarrow(v.GetUpper(), v.GetLower())
                .Store(destination);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        static Vector256<byte> FastNarrow(Vector256<short> v)
        {
            // This or is 2 stores faster ? 
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

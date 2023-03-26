using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.Intrinsics.X86;
using System.Text;

namespace ConsoleApp1
{
    public partial class Utf8Benchmarks2
    {
        sealed class SealedEncoding : UTF8Encoding
        {
            public SealedEncoding(bool a, bool b) : base(a, b) { }

            public override unsafe int GetBytes(char* chars, int charCount, byte* bytes, int byteCount)
            {
                if (chars != null
                   && bytes != null
                   && byteCount >= charCount
                   // Break even slightly above 256 for x64
                   && ((Vector128.IsHardwareAccelerated && (uint)charCount < 200) || (!Vector128.IsHardwareAccelerated && (uint)charCount < 16)))
                {
                    uint i = 0;
                    if (Vector128.IsHardwareAccelerated && charCount >= Vector128<ushort>.Count)
                    {
                        var mask = Vector128.Create(unchecked((ushort)0xff80));

                        uint maxSimdIndex = (uint)(charCount - Vector128<ushort>.Count);
                        for (i = 0; i < maxSimdIndex; i += (uint)Vector128<ushort>.Count)
                        {
                            Vector128<ushort> v = *(Vector128<ushort>*)(chars + i);
                            if (VectorContainsNonAsciiChar(v, mask))
                                goto NonAscii;

                            StoreLower((long*)(bytes + i), ExtractAsciiVector(v, v));
                        }

                        // Read last full vector and do a (possibly overlapping) store if successfull
                        Vector128<ushort> v2 = *(Vector128<ushort>*)(chars + maxSimdIndex);
                        if (VectorContainsNonAsciiChar(v2, mask))
                            goto NonAscii;

                        Vector128<byte> packed = ExtractAsciiVector(v2, v2);
                        StoreLower((long*)(bytes + charCount - sizeof(long)), packed);
                        return charCount;
                    }
                    else
                    {
                        for (; i < (uint)charCount; ++i)
                        {
                            char t = chars[i];
                            if (t >= 0x80)
                                goto NonAscii;

                            bytes[i] = (byte)t;
                        }
                        return charCount;
                    }

                NonAscii:
                    return (int)i + base.GetBytes(chars + i, charCount - (int)i, bytes + i, byteCount - (int)i);
                }
                else
                {
                    return base.GetBytes(chars, charCount, bytes, byteCount);
                }
            }

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            private unsafe static void StoreLower(long* address, Vector128<byte> source)
            {
                // Allow a single 8 byte store on 32bit for x86
                if (Sse2.IsSupported)
                    Sse2.StoreScalar(address, source.AsInt64());
                else
                    *(long*)address = source.AsInt64().ToScalar();
            }


            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            private unsafe static void StoreLower(ref byte address, Vector128<byte> source)
            {
                // Allow a single 8 byte store on 32bit for x86
                Unsafe.As<byte, double>(ref address) = source.AsDouble().ToScalar();
            }

            // Is it OK to make this System.Text.Ascii.ExtractAsciiVector internal and use it instead ?
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            private static Vector128<byte> ExtractAsciiVector(Vector128<ushort> vectorFirst, Vector128<ushort> vectorSecond)
            {
                // Narrows two vectors of words [ w7 w6 w5 w4 w3 w2 w1 w0 ] and [ w7' w6' w5' w4' w3' w2' w1' w0' ]
                // to a vector of bytes [ b7 ... b0 b7' ... b0'].

                // prefer architecture specific intrinsic as they don't perform additional AND like Vector128.Narrow does
                if (Sse2.IsSupported)
                {
                    return Sse2.PackUnsignedSaturate(vectorFirst.AsInt16(), vectorSecond.AsInt16());
                }
                else if (AdvSimd.Arm64.IsSupported)
                {
                    return AdvSimd.Arm64.UnzipEven(vectorFirst.AsByte(), vectorSecond.AsByte());
                }
                else
                {
                    return Vector128.Narrow(vectorFirst, vectorSecond);
                }
            }


            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            private static bool VectorContainsNonAsciiChar(Vector128<ushort> utf16Vector, Vector128<ushort> mask)
            {
                // prefer architecture specific intrinsic as they offer better perf
                if (Sse41.IsSupported)
                {
                    // If a non-ASCII bit is set in any WORD of the vector, we have seen non-ASCII data.
                    return !Sse41.TestZ(utf16Vector.AsInt16(), mask.AsInt16());
                }
                else if (AdvSimd.Arm64.IsSupported)
                {
                    // First we pick four chars, a larger one from all four pairs of adjecent chars in the vector.
                    // If any of those four chars has a non-ASCII bit set, we have seen non-ASCII data.
                    Vector128<ushort> maxChars = AdvSimd.Arm64.MaxPairwise(utf16Vector, utf16Vector);
                    return (maxChars.AsUInt64().ToScalar() & 0xFF80FF80FF80FF80) != 0;
                }
                else
                {
                    // If a non-ASCII bit is set in any WORD of the vector, we have seen non-ASCII data.
                    return (utf16Vector & mask) != Vector128<ushort>.Zero;
                }
            }

            public unsafe override int GetBytes(ReadOnlySpan<char> chars, Span<byte> bytes)
            {
                // Throwaway span accesses - workaround for https://github.com/dotnet/runtime/issues/12332
                _ = chars.Length;
                _ = bytes.Length;

                fixed (char* pChars = &MemoryMarshal.GetReference(chars))
                fixed (byte* pBytes = &MemoryMarshal.GetReference(bytes))
                    return GetBytes(pChars, chars.Length, pBytes, bytes.Length);

#if OLD
                if (chars.Length <= bytes.Length
                    && chars.Length < 2048
                    //&& ((Vector128.IsHardwareAccelerated && chars.Length < 1024)
                    //    || (!Vector128.IsHardwareAccelerated && chars.Length < 32))
                    )
                {
                    uint i = 0;
                    ref ushort sourceRef = ref MemoryMarshal.GetReference(MemoryMarshal.Cast<char, ushort>(chars));
                    ref byte destinationRef = ref MemoryMarshal.GetReference(bytes);

                    if (Vector128.IsHardwareAccelerated && chars.Length >= Vector128<short>.Count)
                    {
                        var mask = Vector128.Create(unchecked((ushort)0xff80));
                        uint lastSimd = (uint)(chars.Length - Vector128<ushort>.Count);

                        for (i = 0; i < lastSimd; i += (uint)Vector128<ushort>.Count)
                        {
                            var v = Vector128.LoadUnsafe(ref Unsafe.Add(ref sourceRef, i));
                            if (VectorContainsNonAsciiChar(v, mask))
                                goto NonAscii;

                            StoreLower(ref Unsafe.Add(ref destinationRef, i), ExtractAsciiVector(v, v));
                        }

                        var v2 = Vector128.LoadUnsafe(ref Unsafe.Add(ref sourceRef, (int)lastSimd));
                        if (VectorContainsNonAsciiChar(v2, mask))
                            goto NonAscii;

                        var packed = ExtractAsciiVector(v2, v2);
                        StoreLower(ref Unsafe.Add(ref destinationRef, chars.Length - sizeof(long)), packed);

                        return chars.Length;
                    }
                    else if (Vector128.IsHardwareAccelerated || chars.Length < 32)
                    {
                        for (; i < chars.Length; ++i)
                        {
                            char t = chars[(int)i];
                            if (t >= 0x80)
                                goto NonAscii;

                            Unsafe.Add(ref destinationRef, i) = (byte)t;
                        }
                    }

                    return chars.Length;
                NonAscii:
                    return (int)i + base.GetBytes(chars.Slice((int)i), bytes.Slice((int)i));
                }
                else
                {
                    return base.GetBytes(chars, bytes);
                }
#endif
            }
        }
    }
}

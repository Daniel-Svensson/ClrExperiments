// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Buffers.Binary;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Runtime.Versioning;

#nullable enable

namespace Managed.Main
{
    // Implements the Decimal data type. The Decimal data type can
    // represent values ranging from -79,228,162,514,264,337,593,543,950,335 to
    // 79,228,162,514,264,337,593,543,950,335 with 28 significant digits. The
    // Decimal data type is ideally suited to financial calculations that
    // require a large number of significant digits and no round-off errors.
    //
    // The finite set of values of type Decimal are of the form m
    // / 10e, where m is an integer such that
    // -296 <; m <; 296, and e is an integer
    // between 0 and 28 inclusive.
    //
    // Contrary to the float and double data types, Decimal
    // fractional numbers such as 0.1 can be represented exactly in the
    // Decimal representation. In the float and double
    // representations, such numbers are often infinite fractions, making those
    // representations more prone to round-off errors.
    //
    // The Decimal class implements widening conversions from the
    // ubyte, char, short, int, and long types
    // to Decimal. These widening conversions never lose any information
    // and never throw exceptions. The Decimal class also implements
    // narrowing conversions from Decimal to ubyte, char,
    // short, int, and long. These narrowing conversions round
    // the Decimal value towards zero to the nearest integer, and then
    // converts that integer to the destination type. An OverflowException
    // is thrown if the result is not within the range of the destination type.
    //
    // The Decimal class provides a widening conversion from
    // Currency to Decimal. This widening conversion never loses any
    // information and never throws exceptions. The Currency class provides
    // a narrowing conversion from Decimal to Currency. This
    // narrowing conversion rounds the Decimal to four decimals and then
    // converts that number to a Currency. An OverflowException
    // is thrown if the result is not within the range of the Currency type.
    //
    // The Decimal class provides narrowing conversions to and from the
    // float and double types. A conversion from Decimal to
    // float or double may lose precision, but will not lose
    // information about the overall magnitude of the numeric value, and will never
    // throw an exception. A conversion from float or double to
    // Decimal throws an OverflowException if the value is not within
    // the range of the Decimal type.
    [StructLayout(LayoutKind.Sequential)]
    [TypeForwardedFrom("mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089")]
    public readonly partial struct Decimal
        :
          IComparable,
          IComparable<Decimal>,
          IEquatable<Decimal>
    {
        // Sign mask for the flags field. A value of zero in this bit indicates a
        // positive Decimal value, and a value of one in this bit indicates a
        // negative Decimal value.
        //
        // Look at OleAut's DECIMAL_NEG constant to check for negative values
        // in native code.
        private const int SignMask = unchecked((int)0x80000000);

        // Scale mask for the flags field. This byte in the flags field contains
        // the power of 10 to divide the Decimal value by. The scale byte must
        // contain a value between 0 and 28 inclusive.
        private const int ScaleMask = 0x00FF0000;

        // Number of bits scale is shifted by.
        private const int ScaleShift = 16;

        // Constant representing the Decimal value 0.
        public const decimal Zero = 0m;

        // Constant representing the decimal value 1.
        public const decimal One = 1m;

        // Constant representing the decimal value -1.
        public const decimal MinusOne = -1m;

        // Constant representing the largest possible decimal value. The value of
        // this constant is 79,228,162,514,264,337,593,543,950,335.
        public const decimal MaxValue = 79228162514264337593543950335m;

        // Constant representing the smallest possible decimal value. The value of
        // this constant is -79,228,162,514,264,337,593,543,950,335.
        public const decimal MinValue = -79228162514264337593543950335m;

        /// <summary>Represents the additive identity (0).</summary>
        private const decimal AdditiveIdentity = 0m;

        /// <summary>Represents the multiplicative identity (1).</summary>
        private const decimal MultiplicativeIdentity = 1m;

        /// <summary>Represents the number negative one (-1).</summary>
        private const decimal NegativeOne = -1m;

        // The lo, mid, hi, and flags fields contain the representation of the
        // Decimal value. The lo, mid, and hi fields contain the 96-bit integer
        // part of the Decimal. Bits 0-15 (the lower word) of the flags field are
        // unused and must be zero; bits 16-23 contain must contain a value between
        // 0 and 28, indicating the power of 10 to divide the 96-bit integer part
        // by to produce the Decimal value; bits 24-30 are unused and must be zero;
        // and finally bit 31 indicates the sign of the Decimal value, 0 meaning
        // positive and 1 meaning negative.
        //
        // NOTE: Do not change the order and types of these fields. The layout has to
        // match Win32 DECIMAL type.
        private readonly int _flags;
        private readonly uint _hi32;
        private readonly ulong _lo64;

        // Constructs a Decimal from an integer value.
        //
        public Decimal(int value)
        {
            if (value >= 0)
            {
                _flags = 0;
            }
            else
            {
                _flags = SignMask;
                value = -value;
            }
            _lo64 = (uint)value;
            _hi32 = 0;
        }

        // Constructs a Decimal from an unsigned integer value.
        //
        [CLSCompliant(false)]
        public Decimal(uint value)
        {
            _flags = 0;
            _lo64 = value;
            _hi32 = 0;
        }

        // Constructs a Decimal from a long value.
        //
        public Decimal(long value)
        {
            if (value >= 0)
            {
                _flags = 0;
            }
            else
            {
                _flags = SignMask;
                value = -value;
            }
            _lo64 = (ulong)value;
            _hi32 = 0;
        }

        // Constructs a Decimal from an unsigned long value.
        //
        [CLSCompliant(false)]
        public Decimal(ulong value)
        {
            _flags = 0;
            _lo64 = value;
            _hi32 = 0;
        }

        // Constructs a Decimal from a float value.
        //
        public Decimal(float value)
        {
            DecCalc_Main.VarDecFromR4(value, out AsMutable(ref this));
        }

        // Constructs a Decimal from a double value.
        //
        public Decimal(double value)
        {
            DecCalc_Main.VarDecFromR8(value, out AsMutable(ref this));
        }

        private Decimal(SerializationInfo info, StreamingContext context)
        {
            ArgumentNullException.ThrowIfNull(info);

            _flags = info.GetInt32("flags");
            _hi32 = (uint)info.GetInt32("hi");
            _lo64 = (uint)info.GetInt32("lo") + ((ulong)info.GetInt32("mid") << 32);
        }

        //
        // Decimal <==> Currency conversion.
        //
        // A Currency represents a positive or negative Decimal value with 4 digits past the Decimal point. The actual Int64 representation used by these methods
        // is the currency value multiplied by 10,000. For example, a currency value of $12.99 would be represented by the Int64 value 129,900.
        //
        public static Decimal FromOACurrency(long cy)
        {
            ulong absoluteCy; // has to be ulong to accommodate the case where cy == long.MinValue.
            bool isNegative = false;
            if (cy < 0)
            {
                isNegative = true;
                absoluteCy = (ulong)(-cy);
            }
            else
            {
                absoluteCy = (ulong)cy;
            }

            // In most cases, FromOACurrency() produces a Decimal with Scale set to 4. Unless, that is, some of the trailing digits past the Decimal point are zero,
            // in which case, for compatibility with .Net, we reduce the Scale by the number of zeros. While the result is still numerically equivalent, the scale does
            // affect the ToString() value. In particular, it prevents a converted currency value of $12.95 from printing uglily as "12.9500".
            int scale = 4;
            if (absoluteCy != 0)  // For compatibility, a currency of 0 emits the Decimal "0.0000" (scale set to 4).
            {
                while (scale != 0 && ((absoluteCy % 10) == 0))
                {
                    scale--;
                    absoluteCy /= 10;
                }
            }

            return new Decimal((int)absoluteCy, (int)(absoluteCy >> 32), 0, isNegative, (byte)scale);
        }

        public static long ToOACurrency(Decimal value)
        {
            return DecCalc_Main.VarCyFromDec(ref AsMutable(ref value));
        }

        private static bool IsValid(int flags) => (flags & ~(SignMask | ScaleMask)) == 0 && ((uint)(flags & ScaleMask) <= (28 << ScaleShift));

        // Constructs a Decimal from an integer array containing a binary
        // representation. The bits argument must be a non-null integer
        // array with four elements. bits[0], bits[1], and
        // bits[2] contain the low, middle, and high 32 bits of the 96-bit
        // integer part of the Decimal. bits[3] contains the scale factor
        // and sign of the Decimal: bits 0-15 (the lower word) are unused and must
        // be zero; bits 16-23 must contain a value between 0 and 28, indicating
        // the power of 10 to divide the 96-bit integer part by to produce the
        // Decimal value; bits 24-30 are unused and must be zero; and finally bit
        // 31 indicates the sign of the Decimal value, 0 meaning positive and 1
        // meaning negative.
        //
        // Note that there are several possible binary representations for the
        // same numeric value. For example, the value 1 can be represented as {1,
        // 0, 0, 0} (integer value 1 with a scale factor of 0) and equally well as
        // {1000, 0, 0, 0x30000} (integer value 1000 with a scale factor of 3).
        // The possible binary representations of a particular value are all
        // equally valid, and all are numerically equivalent.
        //
        public Decimal(int[] bits) :
            this((ReadOnlySpan<int>)(bits ?? throw new ArgumentNullException(nameof(bits))))
        {
        }

        /// <summary>
        /// Initializes a new instance of <see cref="Decimal"/> to a Decimal value represented in binary and contained in the specified span.
        /// </summary>
        /// <param name="bits">A span of four <see cref="int"/>s containing a binary representation of a Decimal value.</param>
        /// <exception cref="ArgumentException">The length of <paramref name="bits"/> is not 4, or the representation of the Decimal value in <paramref name="bits"/> is not valid.</exception>
        public Decimal(ReadOnlySpan<int> bits)
        {
            if (bits.Length == 4)
            {
                int f = bits[3];
                if (IsValid(f))
                {
                    _lo64 = (uint)bits[0] + ((ulong)(uint)bits[1] << 32);
                    _hi32 = (uint)bits[2];
                    _flags = f;
                    return;
                }
            }
            throw new ArgumentException(SR.Arg_DecBitCtor);
        }

        // Constructs a Decimal from its constituent parts.
        //
        public Decimal(int lo, int mid, int hi, bool isNegative, byte scale)
        {
            ArgumentOutOfRangeException.ThrowIfGreaterThan(scale, 28);
            _lo64 = (uint)lo + ((ulong)(uint)mid << 32);
            _hi32 = (uint)hi;
            _flags = ((int)scale) << 16;
            if (isNegative)
                _flags |= SignMask;
        }

        // Constructs a Decimal from its constituent parts.
        private Decimal(int lo, int mid, int hi, int flags)
        {
            if (IsValid(flags))
            {
                _lo64 = (uint)lo + ((ulong)(uint)mid << 32);
                _hi32 = (uint)hi;
                _flags = flags;
                return;
            }
            throw new ArgumentException(SR.Arg_DecBitCtor);
        }

        private Decimal(in Decimal d, int flags)
        {
            this = d;
            _flags = flags;
        }

        /// <summary>
        /// Gets the scaling factor of the Decimal, which is a number from 0 to 28 that represents the number of Decimal digits.
        /// </summary>
        public byte Scale => (byte)(_flags >> ScaleShift);

        private sbyte Exponent
        {
            get
            {
                // Decimal tracks its exponent as a scale between 0 and 28. This scale is used
                // with the significand as `significand / 10^scale`
                //
                // The IFloatingPoint contract however follows the general IEEE 754 algorithm
                // which is `-1^s * b^e * (b^(1-p) * m)`
                //
                // In this algorithm
                // * `s` is the sign
                // * `b` is the radix (10 for Decimal)
                // * `e` is the exponent
                // * `p` is the number of bits in the significand
                // * `m` is the significand itself
                //
                // For a value such as Decimal.MaxValue, the significand is 79228162514264337593543950335
                // and the scale is 0. Since Decimal tracks 96 significand bits, the required algorithm (simplified)
                // gives us 7.9228162514264337593543950335 * 10^-67 * 10^e. To get back to our original value we
                // then need the exponent to be 95.
                //
                // For a value such as 1E-28, the significand is 1 and the scale is 28. The required algorithm (simplified)
                // gives us 1.0 * 10^-95 * 10^e. To get back to our original value we need the exponent to be 67.
                //
                // Given that scale is bound by 0 and 28, inclusive, the returned exponent will be between 95
                // and 67, inclusive. That is between `(p - 1)` and `(p - 1) - MaxScale`.
                //
                // The generalized algorithm for converting from scale to exponent is then `exponent = 95 - scale`.

                sbyte exponent = (sbyte)(95 - Scale);
                Debug.Assert((exponent >= 67) && (exponent <= 95));
                return exponent;
            }
        }

        // Adds two Decimal values.
        //
        public static Decimal Add(Decimal d1, Decimal d2)
        {
            DecCalc_Main.DecAddSub(ref AsMutable(ref d1), ref AsMutable(ref d2), false);
            return d1;
        }

        // Rounds a Decimal to an integer value. The Decimal argument is rounded
        // towards positive infinity.
        public static Decimal Ceiling(Decimal d)
        {
            int flags = d._flags;
            if ((flags & ScaleMask) != 0)
                DecCalc_Main.InternalRound(ref AsMutable(ref d), (byte)(flags >> ScaleShift), MidpointRounding.ToPositiveInfinity);
            return d;
        }

        // Compares two Decimal values, returning an integer that indicates their
        // relationship.
        //
        public static int Compare(Decimal d1, Decimal d2)
        {
            return DecCalc_Main.VarDecCmp(in d1, in d2);
        }

        // Compares this object to another object, returning an integer that
        // indicates the relationship.
        // Returns a value less than zero if this  object
        // null is considered to be less than any instance.
        // If object is not of type Decimal, this method throws an ArgumentException.
        //
        public int CompareTo(object? value)
        {
            if (value == null)
                return 1;
            if (!(value is Decimal))
                throw new ArgumentException(SR.Arg_MustBeDecimal);

            Decimal other = (Decimal)value;
            return DecCalc_Main.VarDecCmp(in this, in other);
        }

        public int CompareTo(Decimal value)
        {
            return DecCalc_Main.VarDecCmp(in this, in value);
        }

        // Divides two Decimal values.
        //
        public static Decimal Divide(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecDiv_Main(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        // Checks if this Decimal is equal to a given object. Returns true
        // if the given object is a boxed Decimal and its value is equal to the
        // value of this Decimal. Returns false otherwise.
        //
        public override bool Equals([NotNullWhen(true)] object? value) =>
            value is Decimal other &&
            DecCalc_Main.VarDecCmp(in this, in other) == 0;

        public bool Equals(Decimal value) =>
            DecCalc_Main.VarDecCmp(in this, in value) == 0;

        // Returns the hash code for this Decimal.
        //
        public override int GetHashCode() => DecCalc_Main.GetHashCode(in this);

        // Compares two Decimal values for equality. Returns true if the two
        // Decimal values are equal, or false if they are not equal.
        //
        public static bool Equals(Decimal d1, Decimal d2)
        {
            return DecCalc_Main.VarDecCmp(in d1, in d2) == 0;
        }

        // Rounds a Decimal to an integer value. The Decimal argument is rounded
        // towards negative infinity.
        //
        public static Decimal Floor(Decimal d)
        {
            int flags = d._flags;
            if ((flags & ScaleMask) != 0)
                DecCalc_Main.InternalRound(ref AsMutable(ref d), (byte)(flags >> ScaleShift), MidpointRounding.ToNegativeInfinity);
            return d;
        }

        // Returns a binary representation of a Decimal. The return value is an
        // integer array with four elements. Elements 0, 1, and 2 contain the low,
        // middle, and high 32 bits of the 96-bit integer part of the Decimal.
        // Element 3 contains the scale factor and sign of the Decimal: bits 0-15
        // (the lower word) are unused; bits 16-23 contain a value between 0 and
        // 28, indicating the power of 10 to divide the 96-bit integer part by to
        // produce the Decimal value; bits 24-30 are unused; and finally bit 31
        // indicates the sign of the Decimal value, 0 meaning positive and 1
        // meaning negative.
        //
        public static int[] GetBits(Decimal d)
        {
            return new int[] { (int)d.Low, (int)d.Mid, (int)d.High, d._flags };
        }

        /// <summary>
        /// Converts the value of a specified instance of <see cref="Decimal"/> to its equivalent binary representation.
        /// </summary>
        /// <param name="d">The value to convert.</param>
        /// <param name="destination">The span into which to store the four-integer binary representation.</param>
        /// <returns>Four, the number of integers in the binary representation.</returns>
        /// <exception cref="ArgumentException">The destination span was not long enough to store the binary representation.</exception>
        public static int GetBits(Decimal d, Span<int> destination)
        {
            destination[0] = (int)d.Low;
            destination[1] = (int)d.Mid;
            destination[2] = (int)d.High;
            destination[3] = d._flags;
            return 4;
        }

        /// <summary>
        /// Tries to convert the value of a specified instance of <see cref="Decimal"/> to its equivalent binary representation.
        /// </summary>
        /// <param name="d">The value to convert.</param>
        /// <param name="destination">The span into which to store the binary representation.</param>
        /// <param name="valuesWritten">The number of integers written to the destination.</param>
        /// <returns>true if the Decimal's binary representation was written to the destination; false if the destination wasn't long enough.</returns>
        public static bool TryGetBits(Decimal d, Span<int> destination, out int valuesWritten)
        {
            if (destination.Length <= 3)
            {
                valuesWritten = 0;
                return false;
            }

            destination[0] = (int)d.Low;
            destination[1] = (int)d.Mid;
            destination[2] = (int)d.High;
            destination[3] = d._flags;
            valuesWritten = 4;
            return true;
        }

        internal static void GetBytes(in Decimal d, Span<byte> buffer)
        {
            Debug.Assert(buffer.Length >= 16, "buffer.Length >= 16");

            BinaryPrimitives.WriteInt32LittleEndian(buffer, (int)d.Low);
            BinaryPrimitives.WriteInt32LittleEndian(buffer.Slice(4), (int)d.Mid);
            BinaryPrimitives.WriteInt32LittleEndian(buffer.Slice(8), (int)d.High);
            BinaryPrimitives.WriteInt32LittleEndian(buffer.Slice(12), d._flags);
        }

        internal static Decimal ToDecimal(ReadOnlySpan<byte> span)
        {
            Debug.Assert(span.Length >= 16, "span.Length >= 16");
            int lo = BinaryPrimitives.ReadInt32LittleEndian(span);
            int mid = BinaryPrimitives.ReadInt32LittleEndian(span.Slice(4));
            int hi = BinaryPrimitives.ReadInt32LittleEndian(span.Slice(8));
            int flags = BinaryPrimitives.ReadInt32LittleEndian(span.Slice(12));
            return new Decimal(lo, mid, hi, flags);
        }

        public static Decimal Remainder(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecMod(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        // Multiplies two Decimal values.
        //
        public static Decimal Multiply(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecMul_Main(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        // Returns the negated value of the given Decimal. If d is non-zero,
        // the result is -d. If d is zero, the result is zero.
        //
        public static Decimal Negate(Decimal d)
        {
            return new Decimal(in d, d._flags ^ SignMask);
        }

        // Rounds a Decimal value to a given number of Decimal places. The value
        // given by d is rounded to the number of Decimal places given by
        // decimals. The decimals argument must be an integer between
        // 0 and 28 inclusive.
        //
        // By default a mid-point value is rounded to the nearest even number. If the mode is
        // passed in, it can also round away from zero.

        public static Decimal Round(Decimal d) => Round(ref d, 0, MidpointRounding.ToEven);
        public static Decimal Round(Decimal d, int decimals) => Round(ref d, decimals, MidpointRounding.ToEven);
        public static Decimal Round(Decimal d, MidpointRounding mode) => Round(ref d, 0, mode);
        public static Decimal Round(Decimal d, int decimals, MidpointRounding mode) => Round(ref d, decimals, mode);

        private static Decimal Round(ref Decimal d, int decimals, MidpointRounding mode)
        {
            if ((uint)decimals > 28)
                throw new ArgumentOutOfRangeException(nameof(decimals), SR.ArgumentOutOfRange_DecimalRound);
            if ((uint)mode > (uint)MidpointRounding.ToPositiveInfinity)
                throw new ArgumentException(SR.Format(SR.Argument_InvalidEnumValue, mode, nameof(MidpointRounding)), nameof(mode));

            int scale = d.Scale - decimals;
            if (scale > 0)
                DecCalc_Main.InternalRound(ref AsMutable(ref d), (uint)scale, mode);
            return d;
        }

        // Subtracts two Decimal values.
        //
        public static Decimal Subtract(Decimal d1, Decimal d2)
        {
            DecCalc_Main.DecAddSub(ref AsMutable(ref d1), ref AsMutable(ref d2), true);
            return d1;
        }

        // Truncates a Decimal to an integer value. The Decimal argument is rounded
        // towards zero to the nearest integer value, corresponding to removing all
        // digits after the Decimal point.
        //
        public static Decimal Truncate(Decimal d)
        {
            Truncate(ref d);
            return d;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void Truncate(ref Decimal d)
        {
            int flags = d._flags;
            if ((flags & ScaleMask) != 0)
                DecCalc_Main.InternalRound(ref AsMutable(ref d), (byte)(flags >> ScaleShift), MidpointRounding.ToZero);
        }

        public static implicit operator Decimal(System.Decimal value) 
            => Unsafe.As<System.Decimal, Decimal>(ref value);

        public static implicit operator Decimal(byte value) => new Decimal((uint)value);

        [CLSCompliant(false)]
        public static implicit operator Decimal(sbyte value) => new Decimal(value);

        public static implicit operator Decimal(short value) => new Decimal(value);

        [CLSCompliant(false)]
        public static implicit operator Decimal(ushort value) => new Decimal((uint)value);

        public static implicit operator Decimal(char value) => new Decimal((uint)value);

        public static implicit operator Decimal(int value) => new Decimal(value);

        [CLSCompliant(false)]
        public static implicit operator Decimal(uint value) => new Decimal(value);

        public static implicit operator Decimal(long value) => new Decimal(value);

        [CLSCompliant(false)]
        public static implicit operator Decimal(ulong value) => new Decimal(value);

        public static explicit operator Decimal(float value) => new Decimal(value);

        public static explicit operator Decimal(double value) => new Decimal(value);


        public static implicit operator System.Decimal(Decimal value) => Unsafe.As<Decimal, System.Decimal>(ref value);

        public static explicit operator float(Decimal value) => DecCalc_Main.VarR4FromDec(in value);

        public static explicit operator double(Decimal value) => DecCalc_Main.VarR8FromDec(in value);

        public static Decimal operator +(Decimal d) => d;

        public static Decimal operator -(Decimal d) => new Decimal(in d, d._flags ^ SignMask);

        /// <inheritdoc cref="IIncrementOperators{TSelf}.op_Increment(TSelf)" />
        public static Decimal operator ++(Decimal d) => Add(d, One);

        /// <inheritdoc cref="IDecrementOperators{TSelf}.op_Decrement(TSelf)" />
        public static Decimal operator --(Decimal d) => Subtract(d, One);

        public static Decimal operator +(Decimal d1, Decimal d2)
        {
            DecCalc_Main.DecAddSub(ref AsMutable(ref d1), ref AsMutable(ref d2), false);
            return d1;
        }

        public static Decimal operator -(Decimal d1, Decimal d2)
        {
            DecCalc_Main.DecAddSub(ref AsMutable(ref d1), ref AsMutable(ref d2), true);
            return d1;
        }

        /// <inheritdoc cref="IMultiplyOperators{TSelf, TOther, TResult}.op_Multiply(TSelf, TOther)" />
        public static Decimal operator *(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecMul_Main(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        /// <inheritdoc cref="IDivisionOperators{TSelf, TOther, TResult}.op_Division(TSelf, TOther)" />
        public static Decimal operator /(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecDiv_Main(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        /// <inheritdoc cref="IModulusOperators{TSelf, TOther, TResult}.op_Modulus(TSelf, TOther)" />
        public static Decimal operator %(Decimal d1, Decimal d2)
        {
            DecCalc_Main.VarDecMod(ref AsMutable(ref d1), ref AsMutable(ref d2));
            return d1;
        }

        /// <inheritdoc cref="IEqualityOperators{TSelf, TOther, TResult}.op_Equality(TSelf, TOther)" />
        public static bool operator ==(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) == 0;

        /// <inheritdoc cref="IEqualityOperators{TSelf, TOther, TResult}.op_Inequality(TSelf, TOther)" />
        public static bool operator !=(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) != 0;

        /// <inheritdoc cref="IComparisonOperators{TSelf, TOther, TResult}.op_LessThan(TSelf, TOther)" />
        public static bool operator <(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) < 0;

        /// <inheritdoc cref="IComparisonOperators{TSelf, TOther, TResult}.op_LessThanOrEqual(TSelf, TOther)" />
        public static bool operator <=(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) <= 0;

        /// <inheritdoc cref="IComparisonOperators{TSelf, TOther, TResult}.op_GreaterThan(TSelf, TOther)" />
        public static bool operator >(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) > 0;

        /// <inheritdoc cref="IComparisonOperators{TSelf, TOther, TResult}.op_GreaterThanOrEqual(TSelf, TOther)" />
        public static bool operator >=(Decimal d1, Decimal d2) => DecCalc_Main.VarDecCmp(in d1, in d2) >= 0;

        //
        // IConvertible implementation
        //

        public TypeCode GetTypeCode()
        {
            return TypeCode.Decimal;
        }

        /// <inheritdoc cref="INumberBase{TSelf}.IsNegative(TSelf)" />
        public static bool IsNegative(Decimal value) => value._flags < 0;
    }
}

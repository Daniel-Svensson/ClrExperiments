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
	public unsafe partial class AsciiBenchmarks
	{
		public Encoding? _encoding;
		private static readonly SealedEncoding s_UTF8Encoding = new SealedEncoding(false, true);
		private static readonly Encoding s_nonSealedUTF8Encoding = new UTF8Encoding(false, true);

		public string _input;
		public char[] _inputAsChars;
		public byte[] _buffer;
		private GCHandle _bufferHandle;
		private GCHandle _stringHandle;
		private char* _pString;
		private byte* _pBuffer;

		//[Params(32)]
		//[Params(18, 24, 25, 31, 32)]
		//[Params(16, 20, 25, 32)]
		// TODO:
		[Params(5, 8, 15, 16, 19, 31, 33, 36, 39, 41, 44, 48, 63, 64, 71, 79, 87, 95)]
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


			_bufferHandle = GCHandle.Alloc(_buffer, GCHandleType.Pinned);
			_stringHandle = GCHandle.Alloc(_input, GCHandleType.Pinned);

			_pString = (char*)_stringHandle.AddrOfPinnedObject();
			_pBuffer = (byte*)_bufferHandle.AddrOfPinnedObject();
		}

		[GlobalCleanup]
		public void Cleanup()
		{
			_bufferHandle.Free();
			_stringHandle.Free();
		}

#if NET8_0_OR_GREATER
		//[Benchmark(Baseline = true)]
		public unsafe int System_Text_Ascii()
		{

			System.Text.Ascii.FromUtf16(new ReadOnlySpan<char>(_pString, _input.Length), new Span<byte>(_pBuffer, _buffer.Length), out int bytesWritten);
			return bytesWritten;
		}
#endif
		// [Benchmark]
		public unsafe int Ascii_Local()
		{
			Test.Ascii.FromUtf16(new ReadOnlySpan<char>(_pString, StringLengthInChars), new Span<byte>(_pBuffer, StringLengthInChars), out int bytesWritten);
			return bytesWritten;
		}

		[Benchmark(Baseline = true)]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower()
		{
			return Test.Ascii.NarrowUtf16ToAscii_Original(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		[Benchmark()]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v2_Inline()
		{
			return Test.Ascii.NarrowUtf16ToAscii_v2(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		[Benchmark]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_simple_loop()
		{
			return Test.Ascii.NarrowUtf16ToAscii_simple(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		[Benchmark]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v3()
		{
			return Test.Ascii.NarrowUtf16ToAscii_v3_store(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		[Benchmark]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v3b_singlev256()
		{
			return Test.Ascii.NarrowUtf16ToAscii_v3b_store(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		[Benchmark]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v3c_doublev256()
		{
			return Test.Ascii.NarrowUtf16ToAscii_v3c_store(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		//[Benchmark]
		public unsafe nuint Ascii_Local_NarrowUtf16ToAscii_v4_if()
		{
			return Test.Ascii.NarrowUtf16ToAscii_v4(_pString, _pBuffer, (uint)Math.Min(_input.Length, _buffer.Length));
		}

		// [Benchmark]// - slower than encoding
		public int System_Text_Utf8()
		{
			System.Text.Unicode.Utf8.FromUtf16(_input, _buffer, out int _, out int bytesWritten);
			return bytesWritten;
		}
	}
}

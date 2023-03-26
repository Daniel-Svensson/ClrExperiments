using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Diagnosers;
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

namespace ConsoleApp1;

[MemoryDiagnoser]
//[DisassemblyDiagnoser(maxDepth: 10, printSource: true)]
[HardwareCounters(HardwareCounter.Timer, HardwareCounter.InstructionRetired, HardwareCounter.TotalIssues, HardwareCounter.TotalCycles)]
public class BinaryXmlBenchmarks
{
   private XmlDictionaryWriter? _writer;
   private Guid _guid = Guid.NewGuid();
   const int ArraySize = 100;
   private Guid[] _guids = Enumerable.Repeat(Guid.NewGuid(), ArraySize).ToArray();
   private Double[] _doubles = Enumerable.Repeat(4.0/3.0, ArraySize).ToArray();

   [GlobalSetup]
   public void Setup()
   {
      _writer = XmlDictionaryWriter.CreateBinaryWriter(new NullWriteStream());
      _writer.WriteStartDocument();
      _writer.WriteStartElement("root");

      var asm = _writer.GetType().Assembly;
      var version = FileVersionInfo.GetVersionInfo(asm.Location);

      Console.WriteLine($"Assembly: {asm.Location}");
      Console.WriteLine($"Version: {version.FileVersion}");
   }

   [Benchmark]
   public void WriteInt32()
   {
      _writer.WriteValue(unchecked ((int)0xdeadbeef));
   }

	[Benchmark]
	public void WriteInt64()
	{
		_writer.WriteValue(long.MaxValue);
	}

	[Benchmark]
	public void WriteDouble()
	{
		_writer.WriteValue(1.0 / 3.0);
	}

	[Benchmark]
	public void WriteDecimal()
	{
		_writer.WriteValue(1.2m);
	}

	[Benchmark]
	public void WriteGuid()
	{
		_writer.WriteValue(_guid);
	}

   [Benchmark]
   public void WriteTimespan()
   {
      _writer.WriteValue(TimeSpan.Zero);
   }

   [Benchmark]
	public void WriteGuidArray()
	{
		_writer.WriteArray(null, "a", null, _guids, 0, _guids.Length);
	}

	[Benchmark]
	public void WriteDoubleArray()
	{
		_writer.WriteArray(null, "a", null, _doubles, 0, _doubles.Length);
	}
}

[MemoryDiagnoser]

public class BinaryWriterRunner_Extended
{
   private string? _input;
   private char[]? _inputAsChars;
   private XmlDictionaryWriter? _writer;

   [Params(4, 8, 16, 32, 64, 100, 256, 512)]
   public int StringLengthInChars;

   [GlobalSetup]
   public void Setup()
   {
      _writer = XmlDictionaryWriter.CreateBinaryWriter(new NullWriteStream());
      _writer.WriteStartDocument();
      _writer.WriteStartElement("root");

      var asm = _writer.GetType().Assembly;
      var version = FileVersionInfo.GetVersionInfo(asm.Location);

      Console.WriteLine($"Assembly: {asm.Location}");
      Console.WriteLine($"Version: {version.FileVersion}");
   
      _input = new string('x', StringLengthInChars);
      _inputAsChars = _input.ToCharArray();
   }


   //[Benchmark]
   //public void WriteChars()
   //{
   //   _writer.WriteChars(_inputAsChars, 0, _inputAsChars.Length);
   //}

   [Benchmark]
   public void WriteString()
   {
      _writer.WriteString(_input);
   }

   //[Benchmark]
   //public void WriteElement()
   //{
   //   _writer.WriteStartElement(_input);
   //   _writer.WriteEndElement();
   //}
}

internal class NullWriteStream : Stream
{
   public override bool CanRead => false;

   public override bool CanSeek => false;

   public override bool CanWrite => true;

   public override long Length => throw new NotSupportedException();

   public override long Position { get => throw new NotSupportedException(); set => throw new NotSupportedException(); }

   public override void Flush() { }

   public override int Read(byte[] buffer, int offset, int count)
   {
      throw new NotSupportedException();
   }

   public override long Seek(long offset, SeekOrigin origin)
   {
      throw new NotSupportedException();
   }

   public override void SetLength(long value)
   {
      throw new NotSupportedException();
   }

   public override void Write(byte[] buffer, int offset, int count) { }

   public override void Write(ReadOnlySpan<byte> buffer) { }

   public override Task WriteAsync(byte[] buffer, int offset, int count, CancellationToken cancellationToken)
   {
      return Task.CompletedTask;
   }

   public override void WriteByte(byte value) { }

   public override ValueTask WriteAsync(ReadOnlyMemory<byte> buffer, CancellationToken cancellationToken = default)
   {
      return ValueTask.CompletedTask;
   }
}
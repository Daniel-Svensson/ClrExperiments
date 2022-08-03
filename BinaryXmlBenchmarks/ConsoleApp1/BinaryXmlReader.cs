using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Diagnosers;
using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

namespace ConsoleApp1;

public enum Source { MemoryStream, Bytes };

//[MemoryDiagnoser]
//[DisassemblyDiagnoser(maxDepth: 10, printSource: true)]
//[HardwareCounters(HardwareCounter.Timer, HardwareCounter.InstructionRetired, HardwareCounter.TotalIssues, HardwareCounter.TotalCycles)]
public class BinaryXmlReader
{
	private MemoryStream _longMs;
	private MemoryStream _byteMs;
	private MemoryStream _shortMs;
	private MemoryStream _guidMs;
	private MemoryStream _doubleArrayMs;
	private Guid _guid = Guid.NewGuid();
	const int ArraySize = 1000;
	const int NumElements = 1_000_000;
	private Guid[] _guids = Enumerable.Repeat(Guid.NewGuid(), ArraySize).ToArray();
	private Guid[] _guidDest = new Guid[ArraySize];
	private Double[] _doubles = Enumerable.Repeat(4.0 / 3.0, ArraySize).ToArray();
	private Double[] _doubleDest = new double[ArraySize];
	private long _valueStartPosition;
	private XmlDictionaryReader _longRreader;
	private XmlDictionaryReader _byteReader;
	private XmlDictionaryReader _doubleArrReader;
	private XmlDictionaryReader _shortReader;
	private XmlDictionaryReader _guidsReader;

	[Params(Source.MemoryStream, Source.Bytes)]
	public Source Source { get; set; }

	[GlobalSetup]
	public void Setup()
	{
		_longMs = GenerateBinaryXml((writer, i) => writer.WriteValue(((long)i | 1) << 32), NumElements);
		_byteMs = GenerateBinaryXml((writer, i) => writer.WriteValue(((byte)i | 4)), NumElements);
		_shortMs = GenerateBinaryXml((writer, i) => writer.WriteValue((short)((i | 1) << 8)), NumElements);
		_doubleArrayMs = GenerateBinaryXml((writer, i) => writer.WriteArray(null, "b", null, _doubles, 0, _doubles.Length), NumElements / ArraySize);
		_guidMs = GenerateBinaryXml((writer, i) => writer.WriteArray(null, "b", null, _guids, 0, _doubles.Length), NumElements / ArraySize);

		_longRreader = XmlDictionaryReader.CreateBinaryReader(_longMs, XmlDictionaryReaderQuotas.Max);
		_byteReader = XmlDictionaryReader.CreateBinaryReader(_byteMs, XmlDictionaryReaderQuotas.Max);
		_doubleArrReader = XmlDictionaryReader.CreateBinaryReader(_doubleArrayMs, XmlDictionaryReaderQuotas.Max);
		_shortReader = XmlDictionaryReader.CreateBinaryReader(_shortMs, XmlDictionaryReaderQuotas.Max);
		_guidsReader = XmlDictionaryReader.CreateBinaryReader(_guidMs, XmlDictionaryReaderQuotas.Max);

		var asm = _longRreader.GetType().Assembly;
		var version = FileVersionInfo.GetVersionInfo(asm.Location);

		Console.WriteLine($"Assembly: {asm.Location}");
		Console.WriteLine($"Version: {version.FileVersion}");
	}


	private XmlDictionaryReader InitReader(XmlDictionaryReader reader, MemoryStream ms)
	{
		ms.Position = 0;
		if (Source == Source.MemoryStream)
		{
			((IXmlBinaryReaderInitializer)reader).SetInput(ms, null, XmlDictionaryReaderQuotas.Max, null, null);
		}
		else
		{
			ms.TryGetBuffer(out var segment);
			((IXmlBinaryReaderInitializer)reader).SetInput(segment.Array!, segment.Offset, segment.Count, null, XmlDictionaryReaderQuotas.Max, null, null);
		}
		reader.ReadStartElement("root");
		return reader;
	}

	private MemoryStream GenerateBinaryXml(Action<XmlDictionaryWriter, int> writeContent, int num)
	{
		var ms = new MemoryStream();
		var writer = XmlDictionaryWriter.CreateBinaryWriter(ms, null, null, ownsStream: false);
		writer.WriteStartDocument();
		writer.WriteStartElement("root");
		writer.Flush();
		_valueStartPosition = ms.Position;
		for (int i = 0; i < num; i++)
		{
			writer.WriteStartElement("a");
			writeContent(writer, i);
			writer.WriteEndElement();
		}
		writer.WriteEndElement();
		writer.WriteEndDocument();
		writer.Close();

		ms.Seek(0, SeekOrigin.Begin);
		return ms;
	}

	[Benchmark(OperationsPerInvoke = NumElements)]
	public long ReadInt64()
	{
		var reader = InitReader(_longRreader, _longMs);
		long res = 0;
		for (int i = 0; i < NumElements; ++i)
		{
			reader.ReadStartElement();
			res = reader.ReadContentAsLong();
			reader.ReadEndElement();
		}
		return res;
	}

	//[Benchmark(OperationsPerInvoke = NumElements)]
	public int ReadInt8()
	{
		var reader = InitReader(_byteReader, _byteMs);
		int res = 0;
		for (int i = 0; i < NumElements; ++i)
		{
			reader.ReadStartElement();
			res = reader.ReadContentAsInt();
			reader.ReadEndElement();
		}
		return res;
	}

	[Benchmark(OperationsPerInvoke = NumElements)]
	public int ReadInt16()
	{
		var reader = InitReader(_shortReader, _shortMs);
		int res = 0;
		for (int i = 0; i < NumElements; ++i)
		{
			reader.ReadStartElement();
			res = reader.ReadContentAsInt();
			reader.ReadEndElement();
		}
		return res;
	}

	[Benchmark(OperationsPerInvoke = NumElements / ArraySize)]
	public int ReadDoubleArray()
	{
		var reader = InitReader(_doubleArrReader, _doubleArrayMs);
		int read = 0;

		for (int i = 0; i < NumElements / ArraySize; ++i)
		{
			reader.ReadStartElement();
			read = reader.ReadArray("b", string.Empty, _doubleDest, 0, _doubleDest.Length);
			reader.ReadEndElement();
		}

		return read;
	}


	[Benchmark(OperationsPerInvoke = NumElements / ArraySize)]
	public int ReadGuidArray()
	{
		var reader = InitReader(_guidsReader, _guidMs);
		int read = 0;

		for (int i = 0; i < NumElements / ArraySize; ++i)
		{
			reader.ReadStartElement();
			read = reader.ReadArray("b", string.Empty, _guidDest, 0, _guidDest.Length);
			reader.ReadEndElement();
		}

		return read;
	}
}


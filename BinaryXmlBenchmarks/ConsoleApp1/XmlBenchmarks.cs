using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Xml;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Diagnosers;

[HardwareCounters(HardwareCounter.Timer, HardwareCounter.InstructionRetired, HardwareCounter.TotalIssues, HardwareCounter.TotalCycles)]
public class XmlBenchmarks
{
	private const int N = 10000;
	private readonly byte[] data;
	readonly BaselineWriter baselineWriter = new BaselineWriter();
	readonly SpanWriter spanWriter = new SpanWriter();
	readonly UnsafeWriter unsafeWriter = new UnsafeWriter();
	private MemoryStream _ms;

	public XmlBenchmarks()
	{
		data = new byte[N];
		var rand = new Random(42);
		rand.NextBytes(data);

		var ints = MemoryMarshal.Cast<byte, Int64>(data).Slice(0, 1000);
		foreach(ref var i in ints)
		{
			switch (rand.Next()  & 0x3)
			{
				case 0: i = (sbyte)rand.Next(); break;
				case 1: i = (short)rand.Next(); break;
				case 2: i = rand.Next(); break;
				case 3: i = ((long)rand.Next() << 32) | (long)rand.Next(); break;
			}
		}
		_ms = new MemoryStream();
		baselineWriter.SetOutput(_ms, false, null);
		spanWriter.SetOutput(_ms, false, null);
		unsafeWriter.SetOutput(_ms, false, null);
	}

	[Benchmark(Baseline = true)]
	public void WriteInt64()
	{
		_ms.Seek(0, SeekOrigin.Begin);

		var ints = MemoryMarshal.Cast<byte, Int64>(data).Slice(0, 1000);
		foreach (var i in ints)
			baselineWriter.WriteInt64Text(i);
	}

	[Benchmark()]
	public void SpanWriter_Int64()
	{
		_ms.Seek(0, SeekOrigin.Begin);

		var ints = MemoryMarshal.Cast<byte, Int64>(data).Slice(0, 1000);
		foreach (var i in ints)
			spanWriter.WriteInt64Text(i);
	}

	[Benchmark()]
	public void UnsafeWriter_Int64()
	{
		_ms.Seek(0, SeekOrigin.Begin);

		var ints = MemoryMarshal.Cast<byte, Int64>(data).Slice(0, 1000);
		foreach (var i in ints)
			unsafeWriter.WriteInt64Text(i);
	}

}

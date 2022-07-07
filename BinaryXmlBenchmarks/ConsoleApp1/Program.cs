using System;
using System.Text;
using System.Xml;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Running;
using ConsoleApp1;
using Perfolizer.Horology;

// See https://aka.ms/new-console-template for more information
Console.WriteLine("Hello, World!");

var ms = new MemoryStream();
var writer = XmlDictionaryWriter.CreateBinaryWriter(ms);

var reader = XmlDictionaryReader.CreateBinaryReader(ms, XmlDictionaryReaderQuotas.Max);

writer.WriteStartDocument();
writer.WriteStartElement("root");
writer.WriteStartElement("long");
writer.WriteValue(long.MaxValue);
writer.WriteEndElement();
writer.WriteStartElement("int");
writer.WriteValue(int.MaxValue);
writer.WriteEndElement();
writer.WriteStartElement("float");
writer.WriteValue(1.2343);
writer.WriteEndElement();
writer.WriteEndElement();
writer.WriteEndDocument();
writer.Flush();
ms.Position = 0;

reader.ReadStartElement("root");
Console.WriteLine($"long: {reader.ReadElementContentAsLong("long","")}");
Console.WriteLine($"int: {reader.ReadElementContentAsInt("int","")}");
Console.WriteLine($"float: {reader.ReadElementContentAsFloat("float","")}");
reader.ReadEndElement();
reader.Close();

var baseJob = Job.Default
//                   .WithWarmupCount(1) // 1 warmup is enough for our purpose
                   .WithIterationTime(TimeInterval.FromMilliseconds(250.0)) // the default is 0.5s per iteration
                   .WithMaxRelativeError(0.01)
                   ;

var jobBefore = baseJob.WithId("Before").AsBaseline();
var jobAfter = baseJob.WithCustomBuildConfiguration("LocalBuild").WithId("Span");
var config = DefaultConfig.Instance.AddJob(jobBefore).AddJob(jobAfter).KeepBenchmarkFiles();

//var summary = BenchmarkRunner.Run<BinaryXmlBenchmarks>(config);

var a = new Utf8Benchmarks();
a.StringLengthInChars = 20;
a.Setup();

//a.Original();
//a.Encoding();
//a.Systemtext();
//a.Long();
//a.Simd1();
//a.Simd2();

BenchmarkRunner.Run<Utf8Benchmarks>(DefaultConfig.Instance.AddJob(baseJob).KeepBenchmarkFiles());


//BenchmarkRunner.Run<Utf8BenchmarksLength>(DefaultConfig.Instance.AddJob(baseJob).KeepBenchmarkFiles());


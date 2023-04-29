using System;
using System.Text;
using System.Xml;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Toolchains.CoreRun;
using ConsoleApp1;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Perfolizer.Horology;

//var b = new AsciiBenchmarks();
//b.StringLengthInChars = 63;
//b.Scenario = Utf8Scenario.AsciiOnly;
//b.Setup();
//b.Ascii_Local_NarrowUtf16ToAscii_simple_loop();
//b.Ascii_Local_NarrowUtf16ToAscii_v2();
//b.Ascii_Local_NarrowUtf16ToAscii_v3();

//if (args.Length < 3)
//   return;



var baseJob = Job.Default
                   .WithWarmupCount(1) // 1 warmup is enough for our purpose
                   .WithIterationTime(TimeInterval.FromMilliseconds(300.0)) // the default is 0.5s per iteration
                   .WithMaxRelativeError(0.01)
                   //.WithToolchain(new CoreRunToolchain(new FileInfo(@"C:\dev\github\dotnet\runtime\artifacts\bin\coreclr\windows.x64.Release\corerun.exe")))
                   ;


var jobBefore = baseJob.WithId("Before").AsBaseline();
var jobAfter = baseJob.WithCustomBuildConfiguration("LocalBuild");
var config = DefaultConfig.Instance.AddJob(jobBefore).AddJob(jobAfter).KeepBenchmarkFiles();


//var summary = BenchmarkRunner.Run<BinaryXmlBenchmarks>(config);

//var a = new Utf8BenchmarksLength();
//a.StringLengthInChars = 2048;
//a.Setup();
//a.VectorLength_Aligned();

//a.Original();
//a.Encoding();
//a.Avx1();
//a.Long();

//BenchmarkRunner.Run<JWTDecoded>(DefaultConfig.Instance.AddJob(baseJob).KeepBenchmarkFiles(), args);

//var b = new Utf8Benchmarks2();
//for (int i = 1; i < 34; ++i)
//{
//    b.StringLengthInChars = i;
//    b.Setup();

//    var expected = Encoding.UTF8.GetBytes(b._input);
//    int count = b.SealedEncoding_If_Span();
//    var actual = b._buffer;

//    if (count != expected.Length)
//        throw new Exception();
//    if (!expected.AsSpan().SequenceEqual(actual.AsSpan(0, count)))
//        throw new Exception("wrong");
//    b.Setup();

//    count = b.SealedEncoding_If_Ptr();
//    actual = b._buffer;
//    if (count != expected.Length)
//        throw new Exception();
//    if (!expected.AsSpan().SequenceEqual(actual.AsSpan(0, count)))
//        throw new Exception("wrong");
//}

BenchmarkRunner.Run<AsciiBenchmarks>(DefaultConfig.Instance.AddJob(baseJob), args);
//BenchmarkRunner.Run<Utf8Benchmarks2>(DefaultConfig.Instance.AddJob(baseJob).KeepBenchmarkFiles(), args);

//BenchmarkRunner.Run<Utf8BenchmarksLength>(DefaultConfig.Instance.AddJob(baseJob).KeepBenchmarkFiles(), args);


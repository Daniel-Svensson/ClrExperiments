``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-HYFPMI : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |       Mean |    Error |   StdDev |     Median | Ratio | RatioSD |
|----------------------- |----------- |------------------- |-----------:|---------:|---------:|-----------:|------:|--------:|
| DataContractSerializer | Job-HYFPMI |         LocalBuild |   534.7 ns |  5.13 ns |  4.55 ns |   534.8 ns |  1.01 |    0.01 |
| DataContractSerializer |     Before |            Default |   528.4 ns |  4.91 ns |  4.82 ns |   528.9 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild |   304.7 ns |  3.96 ns | 10.98 ns |   308.9 ns |  0.92 |    0.03 |
|    XmlDictionaryWriter |     Before |            Default |   311.9 ns |  2.19 ns |  1.71 ns |   311.8 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 1,160.1 ns | 25.77 ns | 73.11 ns | 1,144.6 ns |  1.09 |    0.08 |
|          XmlSerializer |     Before |            Default | 1,067.3 ns | 15.55 ns | 43.08 ns | 1,059.3 ns |  1.00 |    0.00 |

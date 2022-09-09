``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-HYFPMI : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |    Error |   StdDev | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|---------:|---------:|------:|--------:|
| DataContractSerializer | Job-HYFPMI |         LocalBuild | 239.8 ns |  4.78 ns | 13.80 ns |  1.03 |    0.06 |
| DataContractSerializer |     Before |            Default | 239.4 ns |  2.36 ns |  3.81 ns |  1.00 |    0.00 |
|                        |            |                    |          |          |          |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild | 113.8 ns |  1.14 ns |  1.93 ns |  1.04 |    0.02 |
|    XmlDictionaryWriter |     Before |            Default | 110.3 ns |  1.12 ns |  1.10 ns |  1.00 |    0.00 |
|                        |            |                    |          |          |          |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 607.2 ns | 10.75 ns | 30.68 ns |  1.07 |    0.06 |
|          XmlSerializer |     Before |            Default | 566.9 ns |  6.05 ns | 17.73 ns |  1.00 |    0.00 |

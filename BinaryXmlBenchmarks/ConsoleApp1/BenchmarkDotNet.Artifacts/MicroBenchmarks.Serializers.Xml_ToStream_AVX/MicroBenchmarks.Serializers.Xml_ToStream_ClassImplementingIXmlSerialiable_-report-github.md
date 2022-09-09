``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-HYFPMI : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |    Error |   StdDev |   Median | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|---------:|---------:|---------:|------:|--------:|
| DataContractSerializer | Job-HYFPMI |         LocalBuild | 392.4 ns |  3.88 ns |  5.81 ns | 392.0 ns |  0.90 |    0.03 |
| DataContractSerializer |     Before |            Default | 431.7 ns |  4.31 ns | 10.88 ns | 428.0 ns |  1.00 |    0.00 |
|                        |            |                    |          |          |          |          |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild | 196.7 ns |  1.89 ns |  2.39 ns | 195.9 ns |  0.87 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 227.3 ns |  1.16 ns |  0.97 ns | 227.1 ns |  1.00 |    0.00 |
|                        |            |                    |          |          |          |          |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 740.3 ns | 10.02 ns | 28.44 ns | 737.9 ns |  1.10 |    0.06 |
|          XmlSerializer |     Before |            Default | 671.8 ns |  8.34 ns | 24.33 ns | 668.3 ns |  1.00 |    0.00 |

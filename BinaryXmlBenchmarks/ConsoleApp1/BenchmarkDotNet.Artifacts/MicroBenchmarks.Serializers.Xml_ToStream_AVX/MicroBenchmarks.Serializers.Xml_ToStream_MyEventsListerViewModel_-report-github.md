``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-HYFPMI : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |   Error |  StdDev |   Median | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|--------:|--------:|---------:|------:|--------:|
| DataContractSerializer | Job-HYFPMI |         LocalBuild | 378.6 μs | 2.34 μs | 1.96 μs | 378.5 μs |  0.90 |    0.01 |
| DataContractSerializer |     Before |            Default | 419.2 μs | 3.76 μs | 3.52 μs | 418.6 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |          |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild | 225.1 μs | 2.04 μs | 1.81 μs | 225.0 μs |  0.70 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 319.9 μs | 2.76 μs | 2.58 μs | 319.0 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |          |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 195.3 μs | 1.92 μs | 1.89 μs | 195.1 μs |  0.98 |    0.03 |
|          XmlSerializer |     Before |            Default | 195.3 μs | 1.93 μs | 4.54 μs | 193.6 μs |  1.00 |    0.00 |

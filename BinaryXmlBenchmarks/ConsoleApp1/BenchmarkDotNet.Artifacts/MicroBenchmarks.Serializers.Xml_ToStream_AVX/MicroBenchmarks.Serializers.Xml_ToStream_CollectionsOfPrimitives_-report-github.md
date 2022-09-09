``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-HYFPMI : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |   Error |  StdDev | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|--------:|--------:|------:|--------:|
| DataContractSerializer | Job-HYFPMI |         LocalBuild | 353.9 μs | 3.52 μs | 3.77 μs |  0.95 |    0.01 |
| DataContractSerializer |     Before |            Default | 374.2 μs | 2.28 μs | 2.13 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild | 188.3 μs | 1.82 μs | 1.52 μs |  0.95 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 198.3 μs | 1.87 μs | 1.92 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 196.2 μs | 1.96 μs | 2.55 μs |  0.98 |    0.02 |
|          XmlSerializer |     Before |            Default | 200.9 μs | 1.39 μs | 1.24 μs |  1.00 |    0.00 |

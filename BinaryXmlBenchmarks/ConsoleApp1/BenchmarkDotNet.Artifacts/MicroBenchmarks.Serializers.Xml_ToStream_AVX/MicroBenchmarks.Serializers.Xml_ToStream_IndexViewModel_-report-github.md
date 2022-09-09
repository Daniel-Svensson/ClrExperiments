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
| DataContractSerializer | Job-HYFPMI |         LocalBuild | 33.76 μs | 0.332 μs | 0.356 μs |  0.87 |    0.01 |
| DataContractSerializer |     Before |            Default | 38.69 μs | 0.319 μs | 0.283 μs |  1.00 |    0.00 |
|                        |            |                    |          |          |          |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild | 17.10 μs | 0.170 μs | 0.142 μs |  0.63 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 27.08 μs | 0.212 μs | 0.199 μs |  1.00 |    0.00 |
|                        |            |                    |          |          |          |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 21.19 μs | 0.212 μs | 0.597 μs |  1.00 |    0.02 |
|          XmlSerializer |     Before |            Default | 20.88 μs | 0.158 μs | 0.140 μs |  1.00 |    0.00 |

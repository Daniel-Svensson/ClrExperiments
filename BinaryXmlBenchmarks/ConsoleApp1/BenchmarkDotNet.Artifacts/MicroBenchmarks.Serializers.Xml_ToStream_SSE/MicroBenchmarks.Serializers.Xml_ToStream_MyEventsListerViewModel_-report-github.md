``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-MHNDBR : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |   Error |  StdDev | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|--------:|--------:|------:|--------:|
| DataContractSerializer | Job-MHNDBR |         LocalBuild | 369.2 μs | 3.31 μs | 3.09 μs |  0.89 |    0.01 |
| DataContractSerializer |     Before |            Default | 412.5 μs | 3.47 μs | 3.24 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild | 277.0 μs | 2.15 μs | 2.01 μs |  0.82 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 336.6 μs | 2.56 μs | 2.14 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 192.2 μs | 1.92 μs | 3.74 μs |  1.00 |    0.02 |
|          XmlSerializer |     Before |            Default | 190.0 μs | 1.83 μs | 2.44 μs |  1.00 |    0.00 |

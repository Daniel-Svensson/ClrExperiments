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
| DataContractSerializer | Job-MHNDBR |         LocalBuild | 345.7 μs | 3.45 μs | 4.36 μs |  0.95 |    0.01 |
| DataContractSerializer |     Before |            Default | 361.6 μs | 1.97 μs | 1.75 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild | 187.5 μs | 0.84 μs | 0.74 μs |  0.97 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 192.6 μs | 1.16 μs | 1.03 μs |  1.00 |    0.00 |
|                        |            |                    |          |         |         |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 192.8 μs | 1.17 μs | 1.03 μs |  0.98 |    0.02 |
|          XmlSerializer |     Before |            Default | 198.7 μs | 1.95 μs | 3.03 μs |  1.00 |    0.00 |

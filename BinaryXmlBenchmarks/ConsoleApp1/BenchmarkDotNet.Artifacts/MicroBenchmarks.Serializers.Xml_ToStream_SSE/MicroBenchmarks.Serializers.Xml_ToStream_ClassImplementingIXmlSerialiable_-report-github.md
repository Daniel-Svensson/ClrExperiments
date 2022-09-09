``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-MHNDBR : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |   Error |   StdDev | Ratio | RatioSD |
|----------------------- |----------- |------------------- |---------:|--------:|---------:|------:|--------:|
| DataContractSerializer | Job-MHNDBR |         LocalBuild | 342.7 ns | 3.42 ns |  5.72 ns |  0.85 |    0.02 |
| DataContractSerializer |     Before |            Default | 403.1 ns | 3.58 ns |  4.90 ns |  1.00 |    0.00 |
|                        |            |                    |          |         |          |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild | 182.0 ns | 1.41 ns |  1.32 ns |  0.82 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default | 221.0 ns | 1.54 ns |  1.29 ns |  1.00 |    0.00 |
|                        |            |                    |          |         |          |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 614.8 ns | 6.16 ns | 14.51 ns |  0.96 |    0.03 |
|          XmlSerializer |     Before |            Default | 642.2 ns | 6.40 ns | 17.73 ns |  1.00 |    0.00 |

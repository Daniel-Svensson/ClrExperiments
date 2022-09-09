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
| DataContractSerializer | Job-HYFPMI |         LocalBuild |   975.3 ns |  5.08 ns |  4.51 ns |   975.6 ns |  0.85 |    0.01 |
| DataContractSerializer |     Before |            Default | 1,152.4 ns |  8.84 ns |  7.84 ns | 1,151.6 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild |   607.5 ns |  6.06 ns | 13.67 ns |   601.1 ns |  0.82 |    0.02 |
|    XmlDictionaryWriter |     Before |            Default |   741.6 ns |  6.96 ns |  6.84 ns |   738.9 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 1,652.6 ns | 19.21 ns | 55.72 ns | 1,640.8 ns |  0.97 |    0.06 |
|          XmlSerializer |     Before |            Default | 1,711.1 ns | 24.99 ns | 70.47 ns | 1,689.3 ns |  1.00 |    0.00 |

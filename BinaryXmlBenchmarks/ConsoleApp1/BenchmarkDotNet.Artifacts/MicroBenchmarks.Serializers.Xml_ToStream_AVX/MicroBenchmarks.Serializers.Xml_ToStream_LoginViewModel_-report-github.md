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
| DataContractSerializer | Job-HYFPMI |         LocalBuild |   657.1 ns |  8.74 ns | 25.21 ns |   649.7 ns |  0.94 |    0.05 |
| DataContractSerializer |     Before |            Default |   702.6 ns |  6.83 ns | 12.83 ns |   700.5 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|    XmlDictionaryWriter | Job-HYFPMI |         LocalBuild |   317.7 ns |  3.03 ns |  4.05 ns |   317.8 ns |  0.83 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default |   384.7 ns |  2.61 ns |  2.31 ns |   384.5 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|          XmlSerializer | Job-HYFPMI |         LocalBuild | 1,207.9 ns | 12.01 ns | 31.44 ns | 1,198.8 ns |  0.99 |    0.04 |
|          XmlSerializer |     Before |            Default | 1,221.6 ns | 12.20 ns | 34.02 ns | 1,215.0 ns |  1.00 |    0.00 |

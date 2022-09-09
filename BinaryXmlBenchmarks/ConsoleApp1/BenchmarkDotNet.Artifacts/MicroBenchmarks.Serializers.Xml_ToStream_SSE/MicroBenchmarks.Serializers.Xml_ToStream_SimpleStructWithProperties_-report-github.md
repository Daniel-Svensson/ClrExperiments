``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-MHNDBR : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |       Mean |    Error |   StdDev | Ratio | RatioSD |
|----------------------- |----------- |------------------- |-----------:|---------:|---------:|------:|--------:|
| DataContractSerializer | Job-MHNDBR |         LocalBuild |   509.3 ns |  5.02 ns |  7.67 ns |  0.96 |    0.02 |
| DataContractSerializer |     Before |            Default |   531.0 ns |  5.19 ns |  6.38 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild |   275.3 ns |  1.18 ns |  1.04 ns |  0.86 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default |   319.2 ns |  3.07 ns |  2.72 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 1,079.8 ns | 10.80 ns | 20.80 ns |  1.04 |    0.02 |
|          XmlSerializer |     Before |            Default | 1,033.5 ns | 10.26 ns | 15.36 ns |  1.00 |    0.00 |

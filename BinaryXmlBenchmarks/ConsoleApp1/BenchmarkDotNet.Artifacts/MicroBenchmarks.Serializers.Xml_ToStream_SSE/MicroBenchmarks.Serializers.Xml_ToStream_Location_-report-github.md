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
| DataContractSerializer | Job-MHNDBR |         LocalBuild |   982.4 ns |  9.51 ns | 13.64 ns |  0.79 |    0.01 |
| DataContractSerializer |     Before |            Default | 1,245.6 ns |  5.07 ns |  4.23 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild |   660.2 ns |  5.89 ns |  5.51 ns |  0.92 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default |   719.9 ns |  3.64 ns |  3.22 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 1,541.1 ns | 15.24 ns | 42.97 ns |  1.03 |    0.03 |
|          XmlSerializer |     Before |            Default | 1,523.4 ns | 15.22 ns | 24.14 ns |  1.00 |    0.00 |

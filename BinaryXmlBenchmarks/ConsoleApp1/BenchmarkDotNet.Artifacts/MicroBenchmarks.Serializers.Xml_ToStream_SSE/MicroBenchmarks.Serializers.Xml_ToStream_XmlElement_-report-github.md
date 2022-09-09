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
| DataContractSerializer | Job-MHNDBR |         LocalBuild | 206.3 ns | 1.81 ns |  3.48 ns |  0.95 |    0.02 |
| DataContractSerializer |     Before |            Default | 218.0 ns | 1.98 ns |  2.90 ns |  1.00 |    0.00 |
|                        |            |                    |          |         |          |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild | 112.2 ns | 1.13 ns |  1.26 ns |  1.06 |    0.02 |
|    XmlDictionaryWriter |     Before |            Default | 105.9 ns | 0.97 ns |  0.86 ns |  1.00 |    0.00 |
|                        |            |                    |          |         |          |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 548.5 ns | 5.40 ns |  9.60 ns |  0.89 |    0.02 |
|          XmlSerializer |     Before |            Default | 616.5 ns | 6.10 ns | 11.76 ns |  1.00 |    0.00 |

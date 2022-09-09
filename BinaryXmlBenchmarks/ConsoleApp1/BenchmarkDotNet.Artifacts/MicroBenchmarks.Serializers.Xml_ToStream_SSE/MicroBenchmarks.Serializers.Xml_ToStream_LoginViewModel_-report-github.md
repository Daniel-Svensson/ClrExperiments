``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-MHNDBR : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |       Mean |    Error |   StdDev |     Median | Ratio | RatioSD |
|----------------------- |----------- |------------------- |-----------:|---------:|---------:|-----------:|------:|--------:|
| DataContractSerializer | Job-MHNDBR |         LocalBuild |   577.3 ns |  5.59 ns | 15.86 ns |   572.4 ns |  0.86 |    0.03 |
| DataContractSerializer |     Before |            Default |   665.6 ns |  6.57 ns |  9.84 ns |   664.1 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild |   315.1 ns |  2.01 ns |  1.88 ns |   314.6 ns |  0.80 |    0.01 |
|    XmlDictionaryWriter |     Before |            Default |   392.6 ns |  3.64 ns |  3.41 ns |   392.4 ns |  1.00 |    0.00 |
|                        |            |                    |            |          |          |            |       |         |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 1,082.3 ns | 10.58 ns | 17.38 ns | 1,082.5 ns |  0.92 |    0.02 |
|          XmlSerializer |     Before |            Default | 1,173.6 ns | 11.47 ns | 23.42 ns | 1,170.2 ns |  1.00 |    0.00 |

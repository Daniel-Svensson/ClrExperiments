``` ini

BenchmarkDotNet=v0.13.1.1823-nightly, OS=Windows 11 (10.0.22000.856/21H2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-MHNDBR : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Before     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                 Method |        Job | BuildConfiguration |     Mean |    Error |   StdDev | Ratio |
|----------------------- |----------- |------------------- |---------:|---------:|---------:|------:|
| DataContractSerializer | Job-MHNDBR |         LocalBuild | 33.78 μs | 0.335 μs | 0.399 μs |  0.91 |
| DataContractSerializer |     Before |            Default | 36.90 μs | 0.237 μs | 0.222 μs |  1.00 |
|                        |            |                    |          |          |          |       |
|    XmlDictionaryWriter | Job-MHNDBR |         LocalBuild | 20.15 μs | 0.134 μs | 0.119 μs |  0.78 |
|    XmlDictionaryWriter |     Before |            Default | 25.98 μs | 0.159 μs | 0.149 μs |  1.00 |
|                        |            |                    |          |          |          |       |
|          XmlSerializer | Job-MHNDBR |         LocalBuild | 20.60 μs | 0.180 μs | 0.184 μs |  1.00 |
|          XmlSerializer |     Before |            Default | 20.56 μs | 0.148 μs | 0.131 μs |  1.00 |

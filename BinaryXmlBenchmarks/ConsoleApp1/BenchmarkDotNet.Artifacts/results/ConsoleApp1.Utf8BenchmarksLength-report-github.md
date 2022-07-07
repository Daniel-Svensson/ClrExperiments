``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-UUVRQZ : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|   Method | StringLengthInChars |     Scenario |      Mean |    Error |    StdDev | Ratio |
|--------- |-------------------- |------------- |----------:|---------:|----------:|------:|
| **Original** |                 **512** |    **AsciiOnly** | **222.36 ns** | **2.033 ns** |  **2.916 ns** |  **1.00** |
| Encoding |                 512 |    AsciiOnly |  16.45 ns | 0.088 ns |  0.078 ns |  0.07 |
|     Avx1 |                 512 |    AsciiOnly |  15.60 ns | 0.107 ns |  0.100 ns |  0.07 |
|          |                     |              |           |          |           |       |
| **Original** |                 **512** |        **Mixed** | **246.56 ns** | **1.514 ns** |  **1.416 ns** |  **1.00** |
| Encoding |                 512 |        Mixed |  51.90 ns | 0.241 ns |  0.214 ns |  0.21 |
|     Avx1 |                 512 |        Mixed |  54.90 ns | 0.217 ns |  0.203 ns |  0.22 |
|          |                     |              |           |          |           |       |
| **Original** |                 **512** | **OnlyNonAscii** | **244.44 ns** | **2.412 ns** |  **2.778 ns** |  **1.00** |
| Encoding |                 512 | OnlyNonAscii |  57.23 ns | 0.563 ns |  0.732 ns |  0.23 |
|     Avx1 |                 512 | OnlyNonAscii |  55.21 ns | 0.114 ns |  0.101 ns |  0.23 |
|          |                     |              |           |          |           |       |
| **Original** |                **1024** |    **AsciiOnly** | **223.03 ns** | **0.788 ns** |  **0.737 ns** |  **1.00** |
| Encoding |                1024 |    AsciiOnly |  30.02 ns | 0.114 ns |  0.095 ns |  0.13 |
|     Avx1 |                1024 |    AsciiOnly |  29.16 ns | 0.131 ns |  0.122 ns |  0.13 |
|          |                     |              |           |          |           |       |
| **Original** |                **1024** |        **Mixed** | **469.23 ns** | **2.619 ns** |  **2.322 ns** |  **1.00** |
| Encoding |                1024 |        Mixed | 106.48 ns | 0.462 ns |  0.385 ns |  0.23 |
|     Avx1 |                1024 |        Mixed | 110.19 ns | 0.381 ns |  0.338 ns |  0.23 |
|          |                     |              |           |          |           |       |
| **Original** |                **1024** | **OnlyNonAscii** | **466.51 ns** | **3.825 ns** |  **3.390 ns** |  **1.00** |
| Encoding |                1024 | OnlyNonAscii | 106.56 ns | 0.995 ns |  1.222 ns |  0.23 |
|     Avx1 |                1024 | OnlyNonAscii | 109.67 ns | 0.520 ns |  0.461 ns |  0.24 |
|          |                     |              |           |          |           |       |
| **Original** |                **2048** |    **AsciiOnly** | **439.89 ns** | **2.161 ns** |  **1.805 ns** |  **1.00** |
| Encoding |                2048 |    AsciiOnly |  60.43 ns | 0.176 ns |  0.165 ns |  0.14 |
|     Avx1 |                2048 |    AsciiOnly |  59.10 ns | 0.252 ns |  0.235 ns |  0.13 |
|          |                     |              |           |          |           |       |
| **Original** |                **2048** |        **Mixed** | **866.47 ns** | **4.215 ns** |  **5.627 ns** |  **1.00** |
| Encoding |                2048 |        Mixed | 201.72 ns | 0.597 ns |  0.558 ns |  0.23 |
|     Avx1 |                2048 |        Mixed | 204.89 ns | 0.209 ns |  0.195 ns |  0.24 |
|          |                     |              |           |          |           |       |
| **Original** |                **2048** | **OnlyNonAscii** | **915.96 ns** | **8.901 ns** | **13.047 ns** |  **1.00** |
| Encoding |                2048 | OnlyNonAscii | 202.49 ns | 0.827 ns |  0.733 ns |  0.22 |
|     Avx1 |                2048 | OnlyNonAscii | 207.47 ns | 0.880 ns |  0.823 ns |  0.23 |

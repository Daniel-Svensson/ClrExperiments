``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 5 5600U with Radeon Graphics, 1 CPU, 12 logical and 6 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-JPAVPN : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|       Method | StringLengthInChars |  Scenario |       Mean |     Error |    StdDev | Ratio | RatioSD |
|------------- |-------------------- |---------- |-----------:|----------:|----------:|------:|--------:|
|     **Original** |                  **42** | **AsciiOnly** |  **22.904 ns** | **0.4901 ns** | **1.4373 ns** |  **1.00** |    **0.00** |
|     Encoding |                  42 | AsciiOnly |   6.083 ns | 0.0381 ns | 0.0318 ns |  0.27 |    0.02 |
|         Avx1 |                  42 | AsciiOnly |   2.341 ns | 0.0213 ns | 0.0166 ns |  0.10 |    0.01 |
| VectorLength |                  42 | AsciiOnly |   2.347 ns | 0.0359 ns | 0.0318 ns |  0.10 |    0.01 |
|              |                     |           |            |           |           |       |         |
|     **Original** |                  **85** | **AsciiOnly** |  **41.736 ns** | **0.2125 ns** | **0.1884 ns** |  **1.00** |    **0.00** |
|     Encoding |                  85 | AsciiOnly |   6.798 ns | 0.0107 ns | 0.0095 ns |  0.16 |    0.00 |
|         Avx1 |                  85 | AsciiOnly |   8.483 ns | 0.0138 ns | 0.0115 ns |  0.20 |    0.00 |
| VectorLength |                  85 | AsciiOnly |   5.755 ns | 0.0702 ns | 0.2049 ns |  0.14 |    0.00 |
|              |                     |           |            |           |           |       |         |
|     **Original** |                 **256** | **AsciiOnly** | **125.900 ns** | **0.1582 ns** | **0.1402 ns** |  **1.00** |    **0.00** |
|     Encoding |                 256 | AsciiOnly |  11.239 ns | 0.0307 ns | 0.0287 ns |  0.09 |    0.00 |
|         Avx1 |                 256 | AsciiOnly |   7.538 ns | 0.0863 ns | 0.1091 ns |  0.06 |    0.00 |
| VectorLength |                 256 | AsciiOnly |   8.583 ns | 0.0355 ns | 0.0296 ns |  0.07 |    0.00 |
|              |                     |           |            |           |           |       |         |
|     **Original** |                 **512** | **AsciiOnly** | **128.879 ns** | **0.2082 ns** | **0.1739 ns** |  **1.00** |    **0.00** |
|     Encoding |                 512 | AsciiOnly |  18.845 ns | 0.1287 ns | 0.1141 ns |  0.15 |    0.00 |
|         Avx1 |                 512 | AsciiOnly |  13.763 ns | 0.0767 ns | 0.0599 ns |  0.11 |    0.00 |
| VectorLength |                 512 | AsciiOnly |  16.360 ns | 0.0318 ns | 0.0297 ns |  0.13 |    0.00 |
|              |                     |           |            |           |           |       |         |
|     **Original** |                **2024** | **AsciiOnly** | **487.134 ns** | **0.6565 ns** | **0.5819 ns** |  **1.00** |    **0.00** |
|     Encoding |                2024 | AsciiOnly |  66.817 ns | 0.2334 ns | 0.2183 ns |  0.14 |    0.00 |
|         Avx1 |                2024 | AsciiOnly |  53.405 ns | 0.0573 ns | 0.0478 ns |  0.11 |    0.00 |
| VectorLength |                2024 | AsciiOnly |  65.010 ns | 0.4029 ns | 0.4796 ns |  0.13 |    0.00 |

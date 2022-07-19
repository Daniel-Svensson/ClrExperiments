``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.19043.1826 (21H1/May2021Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-SULAVN : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|               Method | StringLengthInChars |  Scenario |       Mean |     Error |     StdDev |     Median | Ratio | RatioSD |
|--------------------- |-------------------- |---------- |-----------:|----------:|-----------:|-----------:|------:|--------:|
|             **Original** |                  **42** | **AsciiOnly** |  **18.590 ns** | **0.2776 ns** |  **0.8184 ns** |  **18.582 ns** |  **1.00** |    **0.00** |
|             Encoding |                  42 | AsciiOnly |   7.540 ns | 0.0834 ns |  0.0780 ns |   7.493 ns |  0.41 |    0.02 |
|                 Avx1 |                  42 | AsciiOnly |   3.035 ns | 0.0409 ns |  0.0649 ns |   3.017 ns |  0.16 |    0.01 |
|         VectorLength |                  42 | AsciiOnly |   2.920 ns | 0.0441 ns |  0.0737 ns |   2.895 ns |  0.16 |    0.01 |
| VectorLength_Aligned |                  42 | AsciiOnly |   3.027 ns | 0.0455 ns |  0.0773 ns |   3.012 ns |  0.16 |    0.01 |
|                      |                     |           |            |           |            |            |       |         |
|             **Original** |                  **85** | **AsciiOnly** |  **32.953 ns** | **0.3491 ns** |  **0.7130 ns** |  **32.956 ns** |  **1.00** |    **0.00** |
|             Encoding |                  85 | AsciiOnly |   9.750 ns | 0.0913 ns |  0.0854 ns |   9.762 ns |  0.30 |    0.01 |
|                 Avx1 |                  85 | AsciiOnly |  11.353 ns | 0.1269 ns |  0.1187 ns |  11.297 ns |  0.34 |    0.01 |
|         VectorLength |                  85 | AsciiOnly |   4.875 ns | 0.0640 ns |  0.0978 ns |   4.846 ns |  0.15 |    0.00 |
| VectorLength_Aligned |                  85 | AsciiOnly |   4.966 ns | 0.0631 ns |  0.1138 ns |   4.952 ns |  0.15 |    0.00 |
|                      |                     |           |            |           |            |            |       |         |
|             **Original** |                 **256** | **AsciiOnly** |  **88.923 ns** | **0.9032 ns** |  **2.2325 ns** |  **88.480 ns** |  **1.00** |    **0.00** |
|             Encoding |                 256 | AsciiOnly |  16.194 ns | 0.2143 ns |  0.6319 ns |  16.158 ns |  0.18 |    0.01 |
|                 Avx1 |                 256 | AsciiOnly |  11.095 ns | 0.1822 ns |  0.5342 ns |  11.110 ns |  0.12 |    0.01 |
|         VectorLength |                 256 | AsciiOnly |  12.768 ns | 0.1427 ns |  0.3906 ns |  12.791 ns |  0.14 |    0.01 |
| VectorLength_Aligned |                 256 | AsciiOnly |  13.505 ns | 0.1509 ns |  0.3405 ns |  13.520 ns |  0.15 |    0.01 |
|                      |                     |           |            |           |            |            |       |         |
|             **Original** |                 **512** | **AsciiOnly** | **163.531 ns** | **1.6442 ns** |  **3.8106 ns** | **162.862 ns** |  **1.00** |    **0.00** |
|             Encoding |                 512 | AsciiOnly |  24.082 ns | 0.2595 ns |  0.6365 ns |  24.069 ns |  0.15 |    0.01 |
|                 Avx1 |                 512 | AsciiOnly |  19.518 ns | 0.2969 ns |  0.8754 ns |  19.652 ns |  0.12 |    0.01 |
|         VectorLength |                 512 | AsciiOnly |  22.783 ns | 0.2421 ns |  0.6588 ns |  22.801 ns |  0.14 |    0.01 |
| VectorLength_Aligned |                 512 | AsciiOnly |  22.900 ns | 0.2460 ns |  0.5295 ns |  22.872 ns |  0.14 |    0.00 |
|                      |                     |           |            |           |            |            |       |         |
|             **Original** |                **2024** | **AsciiOnly** | **609.824 ns** | **6.0732 ns** | **13.8318 ns** | **604.147 ns** |  **1.00** |    **0.00** |
|             Encoding |                2024 | AsciiOnly |  74.845 ns | 0.7674 ns |  1.5848 ns |  74.468 ns |  0.12 |    0.00 |
|                 Avx1 |                2024 | AsciiOnly |  69.816 ns | 0.7109 ns |  1.0640 ns |  69.538 ns |  0.11 |    0.00 |
|         VectorLength |                2024 | AsciiOnly |  83.631 ns | 0.8207 ns |  0.7677 ns |  83.362 ns |  0.14 |    0.00 |
| VectorLength_Aligned |                2024 | AsciiOnly |  81.057 ns | 0.8251 ns |  1.0133 ns |  80.798 ns |  0.13 |    0.00 |

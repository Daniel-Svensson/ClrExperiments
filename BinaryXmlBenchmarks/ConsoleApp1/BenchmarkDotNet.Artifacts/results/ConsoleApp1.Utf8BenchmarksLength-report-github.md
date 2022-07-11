``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 5 5600U with Radeon Graphics, 1 CPU, 12 logical and 6 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-TDRFWV : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|   Method | StringLengthInChars |  Scenario |       Mean |     Error |    StdDev |     Median | Ratio | RatioSD |
|--------- |-------------------- |---------- |-----------:|----------:|----------:|-----------:|------:|--------:|
| **Original** |                  **42** | **AsciiOnly** |  **21.599 ns** | **0.2225 ns** | **0.2971 ns** |  **21.499 ns** |  **1.00** |    **0.00** |
| Encoding |                  42 | AsciiOnly |   6.187 ns | 0.0545 ns | 0.0864 ns |   6.166 ns |  0.29 |    0.01 |
|     Avx1 |                  42 | AsciiOnly |   3.364 ns | 0.0357 ns | 0.0334 ns |   3.375 ns |  0.16 |    0.00 |
|   Vector |                  42 | AsciiOnly |   5.140 ns | 0.0531 ns | 0.0470 ns |   5.123 ns |  0.24 |    0.00 |
|          |                     |           |            |           |           |            |       |         |
| **Original** |                  **85** | **AsciiOnly** |  **41.901 ns** | **0.4080 ns** | **0.3617 ns** |  **41.899 ns** |  **1.00** |    **0.00** |
| Encoding |                  85 | AsciiOnly |   6.816 ns | 0.0452 ns | 0.0377 ns |   6.798 ns |  0.16 |    0.00 |
|     Avx1 |                  85 | AsciiOnly |   5.612 ns | 0.3636 ns | 1.0720 ns |   6.178 ns |  0.14 |    0.03 |
|   Vector |                  85 | AsciiOnly |   5.440 ns | 0.0522 ns | 0.0463 ns |   5.424 ns |  0.13 |    0.00 |
|          |                     |           |            |           |           |            |       |         |
| **Original** |                 **112** | **AsciiOnly** |  **58.304 ns** | **0.5864 ns** | **0.7201 ns** |  **58.191 ns** |  **1.00** |    **0.00** |
| Encoding |                 112 | AsciiOnly |   7.454 ns | 0.0868 ns | 0.1159 ns |   7.397 ns |  0.13 |    0.00 |
|     Avx1 |                 112 | AsciiOnly |   5.321 ns | 0.0643 ns | 0.0836 ns |   5.265 ns |  0.09 |    0.00 |
|   Vector |                 112 | AsciiOnly |   5.202 ns | 0.0494 ns | 0.0438 ns |   5.183 ns |  0.09 |    0.00 |
|          |                     |           |            |           |           |            |       |         |
| **Original** |                 **256** | **AsciiOnly** | **126.057 ns** | **0.3034 ns** | **0.2369 ns** | **125.995 ns** |  **1.00** |    **0.00** |
| Encoding |                 256 | AsciiOnly |  11.358 ns | 0.1222 ns | 0.1500 ns |  11.351 ns |  0.09 |    0.00 |
|     Avx1 |                 256 | AsciiOnly |  10.049 ns | 0.1125 ns | 0.3154 ns |   9.986 ns |  0.08 |    0.00 |
|   Vector |                 256 | AsciiOnly |   9.908 ns | 0.1100 ns | 0.1309 ns |   9.942 ns |  0.08 |    0.00 |
|          |                     |           |            |           |           |            |       |         |
| **Original** |                 **512** | **AsciiOnly** | **129.746 ns** | **0.8915 ns** | **0.8339 ns** | **129.559 ns** |  **1.00** |    **0.00** |
| Encoding |                 512 | AsciiOnly |  18.278 ns | 0.1813 ns | 0.1607 ns |  18.235 ns |  0.14 |    0.00 |
|     Avx1 |                 512 | AsciiOnly |  17.826 ns | 0.1126 ns | 0.1053 ns |  17.782 ns |  0.14 |    0.00 |
|   Vector |                 512 | AsciiOnly |  18.274 ns | 0.3467 ns | 0.9550 ns |  18.135 ns |  0.14 |    0.01 |
|          |                     |           |            |           |           |            |       |         |
| **Original** |                **1024** | **AsciiOnly** | **253.566 ns** | **0.9812 ns** | **0.7661 ns** | **253.415 ns** |  **1.00** |    **0.00** |
| Encoding |                1024 | AsciiOnly |  33.536 ns | 0.1039 ns | 0.0972 ns |  33.521 ns |  0.13 |    0.00 |
|     Avx1 |                1024 | AsciiOnly |  33.056 ns | 0.3416 ns | 0.4677 ns |  33.149 ns |  0.13 |    0.00 |
|   Vector |                1024 | AsciiOnly |  39.296 ns | 0.4035 ns | 0.5656 ns |  39.048 ns |  0.15 |    0.00 |

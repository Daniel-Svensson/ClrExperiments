``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-GEAOSY : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|     Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio | RatioSD |
|----------- |-------------------- |---------- |----------:|----------:|----------:|------:|--------:|
|       **Long** |                   **8** | **AsciiOnly** |  **3.214 ns** | **0.0419 ns** | **0.0392 ns** |  **1.22** |    **0.02** |
| SimdSSE_v4 |                   8 | AsciiOnly |  2.502 ns | 0.0197 ns | 0.0175 ns |  0.95 |    0.01 |
|    SimdAVX |                   8 | AsciiOnly |  2.630 ns | 0.0179 ns | 0.0168 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **10** | **AsciiOnly** |  **3.656 ns** | **0.0115 ns** | **0.0102 ns** |  **1.33** |    **0.01** |
| SimdSSE_v4 |                  10 | AsciiOnly |  2.913 ns | 0.0112 ns | 0.0100 ns |  1.06 |    0.01 |
|    SimdAVX |                  10 | AsciiOnly |  2.739 ns | 0.0112 ns | 0.0093 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **16** | **AsciiOnly** |  **4.006 ns** | **0.0189 ns** | **0.0177 ns** |  **1.48** |    **0.01** |
| SimdSSE_v4 |                  16 | AsciiOnly |  2.926 ns | 0.0286 ns | 0.0267 ns |  1.08 |    0.01 |
|    SimdAVX |                  16 | AsciiOnly |  2.711 ns | 0.0184 ns | 0.0172 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **20** | **AsciiOnly** |  **4.456 ns** | **0.0143 ns** | **0.0134 ns** |  **1.52** |    **0.01** |
| SimdSSE_v4 |                  20 | AsciiOnly |  3.341 ns | 0.0143 ns | 0.0127 ns |  1.14 |    0.01 |
|    SimdAVX |                  20 | AsciiOnly |  2.926 ns | 0.0144 ns | 0.0127 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **30** | **AsciiOnly** |  **7.260 ns** | **0.0207 ns** | **0.0193 ns** |  **2.50** |    **0.01** |
| SimdSSE_v4 |                  30 | AsciiOnly |  3.754 ns | 0.0147 ns | 0.0138 ns |  1.29 |    0.00 |
|    SimdAVX |                  30 | AsciiOnly |  2.907 ns | 0.0054 ns | 0.0042 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **34** | **AsciiOnly** |  **7.452 ns** | **0.0312 ns** | **0.0276 ns** |  **2.35** |    **0.02** |
| SimdSSE_v4 |                  34 | AsciiOnly |  4.159 ns | 0.0068 ns | 0.0060 ns |  1.31 |    0.00 |
|    SimdAVX |                  34 | AsciiOnly |  3.170 ns | 0.0157 ns | 0.0139 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                  **50** | **AsciiOnly** |  **9.064 ns** | **0.0175 ns** | **0.0146 ns** |  **2.55** |    **0.00** |
| SimdSSE_v4 |                  50 | AsciiOnly |  4.994 ns | 0.0144 ns | 0.0134 ns |  1.41 |    0.00 |
|    SimdAVX |                  50 | AsciiOnly |  3.552 ns | 0.0043 ns | 0.0036 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|       **Long** |                 **170** | **AsciiOnly** | **21.707 ns** | **0.0515 ns** | **0.0457 ns** |  **2.67** |    **0.02** |
| SimdSSE_v4 |                 170 | AsciiOnly | 13.815 ns | 0.1518 ns | 0.3548 ns |  1.87 |    0.20 |
|    SimdAVX |                 170 | AsciiOnly |  7.471 ns | 0.2964 ns | 0.8645 ns |  1.00 |    0.00 |

``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X86 RyuJIT
  Job-ERFKKT : .NET 7.0.0 (7.0.22.32404), X86 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|     Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio | RatioSD |
|----------- |-------------------- |---------- |----------:|----------:|----------:|------:|--------:|
|   **Original** |                   **8** | **AsciiOnly** |  **4.804 ns** | **0.0203 ns** | **0.0169 ns** |  **1.14** |    **0.01** |
|       Long |                   8 | AsciiOnly |  4.944 ns | 0.0203 ns | 0.0180 ns |  1.17 |    0.01 |
| SimdSSE_v4 |                   8 | AsciiOnly |  4.310 ns | 0.0160 ns | 0.0142 ns |  1.02 |    0.01 |
|    SimdAVX |                   8 | AsciiOnly |  4.214 ns | 0.0234 ns | 0.0207 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **10** | **AsciiOnly** |  **5.322 ns** | **0.0550 ns** | **0.0515 ns** |  **1.29** |    **0.02** |
|       Long |                  10 | AsciiOnly |  5.385 ns | 0.0354 ns | 0.0295 ns |  1.30 |    0.01 |
| SimdSSE_v4 |                  10 | AsciiOnly |  4.348 ns | 0.0152 ns | 0.0135 ns |  1.05 |    0.01 |
|    SimdAVX |                  10 | AsciiOnly |  4.131 ns | 0.0222 ns | 0.0197 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **16** | **AsciiOnly** |  **7.180 ns** | **0.0533 ns** | **0.0498 ns** |  **1.66** |    **0.02** |
|       Long |                  16 | AsciiOnly |  7.222 ns | 0.0844 ns | 0.1183 ns |  1.66 |    0.03 |
| SimdSSE_v4 |                  16 | AsciiOnly |  4.347 ns | 0.0132 ns | 0.0117 ns |  1.00 |    0.01 |
|    SimdAVX |                  16 | AsciiOnly |  4.338 ns | 0.0377 ns | 0.0353 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **20** | **AsciiOnly** |  **7.851 ns** | **0.0450 ns** | **0.0421 ns** |  **1.83** |    **0.01** |
|       Long |                  20 | AsciiOnly |  8.055 ns | 0.0199 ns | 0.0186 ns |  1.87 |    0.01 |
| SimdSSE_v4 |                  20 | AsciiOnly |  4.486 ns | 0.0117 ns | 0.0110 ns |  1.04 |    0.00 |
|    SimdAVX |                  20 | AsciiOnly |  4.300 ns | 0.0179 ns | 0.0167 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **30** | **AsciiOnly** | **10.580 ns** | **0.0852 ns** | **0.0755 ns** |  **2.44** |    **0.02** |
|       Long |                  30 | AsciiOnly | 10.585 ns | 0.0364 ns | 0.0341 ns |  2.44 |    0.01 |
| SimdSSE_v4 |                  30 | AsciiOnly |  4.695 ns | 0.0136 ns | 0.0127 ns |  1.08 |    0.00 |
|    SimdAVX |                  30 | AsciiOnly |  4.337 ns | 0.0170 ns | 0.0151 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **34** | **AsciiOnly** | **11.225 ns** | **0.0717 ns** | **0.0671 ns** |  **2.50** |    **0.01** |
|       Long |                  34 | AsciiOnly | 11.601 ns | 0.0227 ns | 0.0212 ns |  2.58 |    0.01 |
| SimdSSE_v4 |                  34 | AsciiOnly |  4.922 ns | 0.0102 ns | 0.0096 ns |  1.09 |    0.00 |
|    SimdAVX |                  34 | AsciiOnly |  4.497 ns | 0.0175 ns | 0.0146 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                  **50** | **AsciiOnly** | **17.419 ns** | **0.1307 ns** | **0.1091 ns** |  **3.83** |    **0.03** |
|       Long |                  50 | AsciiOnly | 16.006 ns | 0.0750 ns | 0.0701 ns |  3.52 |    0.02 |
| SimdSSE_v4 |                  50 | AsciiOnly |  5.628 ns | 0.0623 ns | 0.1443 ns |  1.24 |    0.04 |
|    SimdAVX |                  50 | AsciiOnly |  4.543 ns | 0.0078 ns | 0.0069 ns |  1.00 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Original** |                 **170** | **AsciiOnly** | **42.476 ns** | **0.1116 ns** | **0.1044 ns** |  **6.28** |    **0.02** |
|       Long |                 170 | AsciiOnly | 46.426 ns | 0.0856 ns | 0.0759 ns |  6.86 |    0.02 |
| SimdSSE_v4 |                 170 | AsciiOnly | 11.071 ns | 0.0223 ns | 0.0186 ns |  1.64 |    0.00 |
|    SimdAVX |                 170 | AsciiOnly |  6.767 ns | 0.0145 ns | 0.0129 ns |  1.00 |    0.00 |

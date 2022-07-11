``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 5 5600U with Radeon Graphics, 1 CPU, 12 logical and 6 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-PWFQDY : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|        Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |    Median | Ratio | RatioSD |
|-------------- |-------------------- |---------- |----------:|----------:|----------:|----------:|------:|--------:|
|      **Original** |                   **8** | **AsciiOnly** |  **5.542 ns** | **0.0353 ns** | **0.0313 ns** |  **5.548 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                   8 | AsciiOnly |  3.517 ns | 0.0244 ns | 0.0217 ns |  3.516 ns |  0.63 |    0.01 |
|    SimdSSE_v3 |                   8 | AsciiOnly |  3.034 ns | 0.0181 ns | 0.0141 ns |  3.035 ns |  0.55 |    0.00 |
|    SimdSSE_v4 |                   8 | AsciiOnly |  3.028 ns | 0.0268 ns | 0.0251 ns |  3.016 ns |  0.55 |    0.00 |
| SimdSSE_2_128 |                   8 | AsciiOnly |  4.027 ns | 0.0441 ns | 0.0412 ns |  4.003 ns |  0.73 |    0.01 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **10** | **AsciiOnly** |  **6.763 ns** | **0.0502 ns** | **0.0445 ns** |  **6.763 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  10 | AsciiOnly |  3.973 ns | 0.0330 ns | 0.0276 ns |  3.965 ns |  0.59 |    0.00 |
|    SimdSSE_v3 |                  10 | AsciiOnly |  3.017 ns | 0.0263 ns | 0.0234 ns |  3.016 ns |  0.45 |    0.00 |
|    SimdSSE_v4 |                  10 | AsciiOnly |  3.032 ns | 0.0415 ns | 0.0407 ns |  3.011 ns |  0.45 |    0.00 |
| SimdSSE_2_128 |                  10 | AsciiOnly |  4.405 ns | 0.0409 ns | 0.0363 ns |  4.392 ns |  0.65 |    0.00 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **16** | **AsciiOnly** |  **9.619 ns** | **0.0533 ns** | **0.0445 ns** |  **9.608 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  16 | AsciiOnly |  3.752 ns | 0.0330 ns | 0.0309 ns |  3.748 ns |  0.39 |    0.00 |
|    SimdSSE_v3 |                  16 | AsciiOnly |  3.309 ns | 0.0453 ns | 0.0664 ns |  3.287 ns |  0.35 |    0.01 |
|    SimdSSE_v4 |                  16 | AsciiOnly |  3.355 ns | 0.0247 ns | 0.0231 ns |  3.359 ns |  0.35 |    0.00 |
| SimdSSE_2_128 |                  16 | AsciiOnly |  3.773 ns | 0.0344 ns | 0.0287 ns |  3.788 ns |  0.39 |    0.00 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **20** | **AsciiOnly** | **14.145 ns** | **0.1056 ns** | **0.0988 ns** | **14.146 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  20 | AsciiOnly |  4.202 ns | 0.0216 ns | 0.0192 ns |  4.193 ns |  0.30 |    0.00 |
|    SimdSSE_v3 |                  20 | AsciiOnly |  3.514 ns | 0.0305 ns | 0.0285 ns |  3.504 ns |  0.25 |    0.00 |
|    SimdSSE_v4 |                  20 | AsciiOnly |  3.504 ns | 0.0323 ns | 0.0302 ns |  3.494 ns |  0.25 |    0.00 |
| SimdSSE_2_128 |                  20 | AsciiOnly |  4.029 ns | 0.0378 ns | 0.0335 ns |  4.019 ns |  0.29 |    0.00 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **30** | **AsciiOnly** | **16.556 ns** | **0.0611 ns** | **0.0571 ns** | **16.532 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  30 | AsciiOnly |  4.977 ns | 0.0265 ns | 0.0235 ns |  4.971 ns |  0.30 |    0.00 |
|    SimdSSE_v3 |                  30 | AsciiOnly |  3.993 ns | 0.0207 ns | 0.0193 ns |  3.987 ns |  0.24 |    0.00 |
|    SimdSSE_v4 |                  30 | AsciiOnly |  3.771 ns | 0.0272 ns | 0.0227 ns |  3.766 ns |  0.23 |    0.00 |
| SimdSSE_2_128 |                  30 | AsciiOnly |  6.293 ns | 0.2747 ns | 0.7926 ns |  6.027 ns |  0.41 |    0.06 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **34** | **AsciiOnly** | **18.292 ns** | **0.1172 ns** | **0.1096 ns** | **18.273 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  34 | AsciiOnly |  5.004 ns | 0.0239 ns | 0.0223 ns |  5.007 ns |  0.27 |    0.00 |
|    SimdSSE_v3 |                  34 | AsciiOnly |  4.948 ns | 0.0493 ns | 0.0411 ns |  4.943 ns |  0.27 |    0.00 |
|    SimdSSE_v4 |                  34 | AsciiOnly |  4.251 ns | 0.0166 ns | 0.0155 ns |  4.247 ns |  0.23 |    0.00 |
| SimdSSE_2_128 |                  34 | AsciiOnly |  4.485 ns | 0.0275 ns | 0.0257 ns |  4.475 ns |  0.25 |    0.00 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                  **50** | **AsciiOnly** | **29.884 ns** | **0.4651 ns** | **1.2966 ns** | **29.228 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                  50 | AsciiOnly |  6.068 ns | 0.0446 ns | 0.0418 ns |  6.061 ns |  0.21 |    0.01 |
|    SimdSSE_v3 |                  50 | AsciiOnly |  5.697 ns | 0.0687 ns | 0.1069 ns |  5.690 ns |  0.19 |    0.01 |
|    SimdSSE_v4 |                  50 | AsciiOnly |  4.961 ns | 0.0623 ns | 0.0612 ns |  4.960 ns |  0.17 |    0.01 |
| SimdSSE_2_128 |                  50 | AsciiOnly |  4.914 ns | 0.0388 ns | 0.0303 ns |  4.908 ns |  0.17 |    0.00 |
|               |                     |           |           |           |           |           |       |         |
|      **Original** |                 **170** | **AsciiOnly** | **86.634 ns** | **0.7850 ns** | **0.7343 ns** | **86.290 ns** |  **1.00** |    **0.00** |
|       SimdSSE |                 170 | AsciiOnly | 12.688 ns | 0.0813 ns | 0.0721 ns | 12.650 ns |  0.15 |    0.00 |
|    SimdSSE_v3 |                 170 | AsciiOnly | 15.783 ns | 0.1695 ns | 0.1741 ns | 15.711 ns |  0.18 |    0.00 |
|    SimdSSE_v4 |                 170 | AsciiOnly | 12.037 ns | 0.0618 ns | 0.0483 ns | 12.048 ns |  0.14 |    0.00 |
| SimdSSE_2_128 |                 170 | AsciiOnly |  9.500 ns | 0.0587 ns | 0.0491 ns |  9.478 ns |  0.11 |    0.00 |

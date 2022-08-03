``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 5 5600U with Radeon Graphics, 1 CPU, 12 logical and 6 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-JIVEGC : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|        Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |    Median |
|-------------- |-------------------- |---------- |----------:|----------:|----------:|----------:|
|      **Original** |                   **8** | **AsciiOnly** |  **5.891 ns** | **0.0711 ns** | **0.1811 ns** |  **5.951 ns** |
|      Encoding |                   8 | AsciiOnly |  8.253 ns | 0.0891 ns | 0.0834 ns |  8.213 ns |
|    SimdSSE_v4 |                   8 | AsciiOnly |  2.870 ns | 0.0403 ns | 0.0566 ns |  2.870 ns |
|     SimdAVX_2 |                   8 | AsciiOnly |  2.847 ns | 0.0358 ns | 0.0491 ns |  2.843 ns |
| SimdVector256 |                   8 | AsciiOnly |  3.281 ns | 0.0435 ns | 0.0407 ns |  3.259 ns |
|      **Original** |                  **10** | **AsciiOnly** |  **6.708 ns** | **0.0133 ns** | **0.0118 ns** |  **6.706 ns** |
|      Encoding |                  10 | AsciiOnly |  8.778 ns | 0.0954 ns | 0.1099 ns |  8.747 ns |
|    SimdSSE_v4 |                  10 | AsciiOnly |  3.318 ns | 0.0454 ns | 0.0693 ns |  3.335 ns |
|     SimdAVX_2 |                  10 | AsciiOnly |  2.994 ns | 0.0422 ns | 0.0953 ns |  2.955 ns |
| SimdVector256 |                  10 | AsciiOnly |  3.312 ns | 0.0369 ns | 0.0327 ns |  3.299 ns |
|      **Original** |                  **16** | **AsciiOnly** |  **9.725 ns** | **0.1093 ns** | **0.1532 ns** |  **9.733 ns** |
|      Encoding |                  16 | AsciiOnly |  9.254 ns | 0.0569 ns | 0.0475 ns |  9.266 ns |
|    SimdSSE_v4 |                  16 | AsciiOnly |  3.363 ns | 0.0461 ns | 0.0832 ns |  3.392 ns |
|     SimdAVX_2 |                  16 | AsciiOnly |  2.946 ns | 0.0411 ns | 0.0562 ns |  2.959 ns |
| SimdVector256 |                  16 | AsciiOnly |  3.322 ns | 0.0225 ns | 0.0199 ns |  3.323 ns |
|      **Original** |                  **20** | **AsciiOnly** | **14.666 ns** | **0.1590 ns** | **0.2011 ns** | **14.632 ns** |
|      Encoding |                  20 | AsciiOnly |  9.830 ns | 0.1102 ns | 0.1581 ns |  9.848 ns |
|    SimdSSE_v4 |                  20 | AsciiOnly |  3.852 ns | 0.0493 ns | 0.0568 ns |  3.864 ns |
|     SimdAVX_2 |                  20 | AsciiOnly |  3.379 ns | 0.0459 ns | 0.0672 ns |  3.375 ns |
| SimdVector256 |                  20 | AsciiOnly |  3.961 ns | 0.1104 ns | 0.3202 ns |  3.855 ns |
|      **Original** |                  **34** | **AsciiOnly** | **20.364 ns** | **0.2333 ns** | **0.6348 ns** | **20.334 ns** |
|      Encoding |                  34 | AsciiOnly | 11.288 ns | 0.1271 ns | 0.3501 ns | 11.254 ns |
|    SimdSSE_v4 |                  34 | AsciiOnly |  5.170 ns | 0.0647 ns | 0.1737 ns |  5.144 ns |
|     SimdAVX_2 |                  34 | AsciiOnly |  4.267 ns | 0.0544 ns | 0.1525 ns |  4.276 ns |
| SimdVector256 |                  34 | AsciiOnly |  4.746 ns | 0.0621 ns | 0.1311 ns |  4.723 ns |
|      **Original** |                  **84** | **AsciiOnly** | **44.480 ns** | **0.4617 ns** | **1.1669 ns** | **44.205 ns** |
|      Encoding |                  84 | AsciiOnly | 12.920 ns | 0.1417 ns | 0.3856 ns | 12.840 ns |
|    SimdSSE_v4 |                  84 | AsciiOnly |  8.542 ns | 0.0975 ns | 0.2316 ns |  8.508 ns |
|     SimdAVX_2 |                  84 | AsciiOnly |  9.308 ns | 0.1042 ns | 0.1559 ns |  9.302 ns |
| SimdVector256 |                  84 | AsciiOnly |  6.525 ns | 0.0781 ns | 0.1731 ns |  6.480 ns |
|      **Original** |                 **170** | **AsciiOnly** | **92.314 ns** | **0.9422 ns** | **2.6880 ns** | **92.222 ns** |
|      Encoding |                 170 | AsciiOnly | 15.137 ns | 0.1604 ns | 0.4337 ns | 15.098 ns |
|    SimdSSE_v4 |                 170 | AsciiOnly | 17.204 ns | 0.5010 ns | 1.4211 ns | 16.706 ns |
|     SimdAVX_2 |                 170 | AsciiOnly | 10.308 ns | 0.1142 ns | 0.2459 ns | 10.315 ns |
| SimdVector256 |                 170 | AsciiOnly | 11.992 ns | 0.1352 ns | 0.3835 ns | 11.960 ns |

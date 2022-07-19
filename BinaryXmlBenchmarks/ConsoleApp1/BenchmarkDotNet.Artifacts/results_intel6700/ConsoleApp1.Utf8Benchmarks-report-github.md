``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.19043.1826 (21H1/May2021Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-SULAVN : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|     Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |    Median | Ratio | RatioSD |
|----------- |-------------------- |---------- |----------:|----------:|----------:|----------:|------:|--------:|
|   **Original** |                   **8** | **AsciiOnly** |  **6.548 ns** | **0.0104 ns** | **0.0092 ns** |  **6.548 ns** |  **1.95** |    **0.00** |
|   Encoding |                   8 | AsciiOnly | 10.513 ns | 0.0242 ns | 0.0226 ns | 10.511 ns |  3.13 |    0.01 |
|       Long |                   8 | AsciiOnly |  4.637 ns | 0.0101 ns | 0.0090 ns |  4.635 ns |  1.38 |    0.00 |
| SimdSSE_v4 |                   8 | AsciiOnly |  3.357 ns | 0.0070 ns | 0.0062 ns |  3.354 ns |  1.00 |    0.00 |
|    SimdAVX |                   8 | AsciiOnly |  3.695 ns | 0.0084 ns | 0.0074 ns |  3.695 ns |  1.10 |    0.00 |
|  SimdAVX_2 |                   8 | AsciiOnly |  3.721 ns | 0.0092 ns | 0.0086 ns |  3.718 ns |  1.11 |    0.00 |
|  SimdAVX_3 |                   8 | AsciiOnly |  3.713 ns | 0.0049 ns | 0.0043 ns |  3.714 ns |  1.11 |    0.00 |
|            |                     |           |           |           |           |           |       |         |
|   **Original** |                  **10** | **AsciiOnly** |  **7.355 ns** | **0.0236 ns** | **0.0221 ns** |  **7.357 ns** |  **1.83** |    **0.01** |
|   Encoding |                  10 | AsciiOnly | 10.636 ns | 0.0173 ns | 0.0153 ns | 10.634 ns |  2.65 |    0.00 |
|       Long |                  10 | AsciiOnly |  5.723 ns | 0.0050 ns | 0.0045 ns |  5.722 ns |  1.43 |    0.00 |
| SimdSSE_v4 |                  10 | AsciiOnly |  4.014 ns | 0.0040 ns | 0.0031 ns |  4.013 ns |  1.00 |    0.00 |
|    SimdAVX |                  10 | AsciiOnly |  3.694 ns | 0.0048 ns | 0.0040 ns |  3.692 ns |  0.92 |    0.00 |
|  SimdAVX_2 |                  10 | AsciiOnly |  3.710 ns | 0.0052 ns | 0.0044 ns |  3.708 ns |  0.92 |    0.00 |
|  SimdAVX_3 |                  10 | AsciiOnly |  3.968 ns | 0.0073 ns | 0.0065 ns |  3.966 ns |  0.99 |    0.00 |
|            |                     |           |           |           |           |           |       |         |
|   **Original** |                  **16** | **AsciiOnly** | **11.534 ns** | **0.2718 ns** | **0.8013 ns** | **11.493 ns** |  **2.78** |    **0.22** |
|   Encoding |                  16 | AsciiOnly | 11.723 ns | 0.0142 ns | 0.0111 ns | 11.726 ns |  2.91 |    0.01 |
|       Long |                  16 | AsciiOnly |  6.422 ns | 0.0104 ns | 0.0097 ns |  6.420 ns |  1.60 |    0.00 |
| SimdSSE_v4 |                  16 | AsciiOnly |  4.024 ns | 0.0094 ns | 0.0088 ns |  4.022 ns |  1.00 |    0.00 |
|    SimdAVX |                  16 | AsciiOnly |  3.716 ns | 0.0051 ns | 0.0045 ns |  3.716 ns |  0.92 |    0.00 |
|  SimdAVX_2 |                  16 | AsciiOnly |  3.744 ns | 0.0081 ns | 0.0076 ns |  3.740 ns |  0.93 |    0.00 |
|  SimdAVX_3 |                  16 | AsciiOnly |  3.715 ns | 0.0059 ns | 0.0055 ns |  3.715 ns |  0.92 |    0.00 |
|            |                     |           |           |           |           |           |       |         |
|   **Original** |                  **34** | **AsciiOnly** | **18.042 ns** | **0.2771 ns** | **0.8171 ns** | **18.102 ns** |  **3.05** |    **0.06** |
|   Encoding |                  34 | AsciiOnly | 13.107 ns | 0.0122 ns | 0.0102 ns | 13.109 ns |  2.13 |    0.00 |
|       Long |                  34 | AsciiOnly |  9.816 ns | 0.0261 ns | 0.0232 ns |  9.818 ns |  1.59 |    0.01 |
| SimdSSE_v4 |                  34 | AsciiOnly |  6.159 ns | 0.0163 ns | 0.0152 ns |  6.162 ns |  1.00 |    0.00 |
|    SimdAVX |                  34 | AsciiOnly |  5.933 ns | 0.0770 ns | 0.2223 ns |  5.833 ns |  0.99 |    0.02 |
|  SimdAVX_2 |                  34 | AsciiOnly |  5.786 ns | 0.0731 ns | 0.1354 ns |  5.807 ns |  0.96 |    0.02 |
|  SimdAVX_3 |                  34 | AsciiOnly |  5.711 ns | 0.0720 ns | 0.1077 ns |  5.688 ns |  0.93 |    0.02 |
|            |                     |           |           |           |           |           |       |         |
|   **Original** |                  **85** | **AsciiOnly** | **37.646 ns** | **1.0995 ns** | **3.1724 ns** | **36.345 ns** |  **4.12** |    **0.28** |
|   Encoding |                  85 | AsciiOnly | 15.085 ns | 0.1651 ns | 0.2892 ns | 15.077 ns |  1.56 |    0.03 |
|       Long |                  85 | AsciiOnly | 20.678 ns | 0.2259 ns | 0.6068 ns | 20.685 ns |  2.12 |    0.06 |
| SimdSSE_v4 |                  85 | AsciiOnly |  9.701 ns | 0.1115 ns | 0.1193 ns |  9.693 ns |  1.00 |    0.00 |
|    SimdAVX |                  85 | AsciiOnly |  8.095 ns | 0.0958 ns | 0.1140 ns |  8.069 ns |  0.84 |    0.02 |
|  SimdAVX_2 |                  85 | AsciiOnly |  8.254 ns | 0.0966 ns | 0.2238 ns |  8.271 ns |  0.82 |    0.03 |
|  SimdAVX_3 |                  85 | AsciiOnly |  8.724 ns | 0.1010 ns | 0.1277 ns |  8.652 ns |  0.90 |    0.02 |
|            |                     |           |           |           |           |           |       |         |
|   **Original** |                 **170** | **AsciiOnly** | **69.779 ns** | **0.6965 ns** | **0.6174 ns** | **69.688 ns** |  **4.19** |    **0.07** |
|   Encoding |                 170 | AsciiOnly | 18.202 ns | 0.1971 ns | 0.3238 ns | 18.024 ns |  1.10 |    0.03 |
|       Long |                 170 | AsciiOnly | 38.209 ns | 0.3996 ns | 0.8253 ns | 38.167 ns |  2.29 |    0.05 |
| SimdSSE_v4 |                 170 | AsciiOnly | 16.671 ns | 0.1798 ns | 0.2273 ns | 16.700 ns |  1.00 |    0.00 |
|    SimdAVX |                 170 | AsciiOnly | 11.968 ns | 0.1346 ns | 0.3840 ns | 11.863 ns |  0.74 |    0.03 |
|  SimdAVX_2 |                 170 | AsciiOnly | 10.823 ns | 0.1220 ns | 0.1405 ns | 10.784 ns |  0.65 |    0.01 |
|  SimdAVX_3 |                 170 | AsciiOnly | 13.724 ns | 0.1521 ns | 0.4214 ns | 13.769 ns |  0.80 |    0.04 |

``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 10 (10.0.19045.2846/22H2/2022Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2
  Job-SDDLBH : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=300.0000 ms  WarmupCount=1  

```
|                                       Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev |   Median | Ratio | RatioSD |
|--------------------------------------------- |-------------------- |---------- |---------:|----------:|----------:|---------:|------:|--------:|
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                   **5** | **AsciiOnly** | **2.402 ns** | **0.0095 ns** | **0.0084 ns** | **2.398 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                   5 | AsciiOnly | 2.401 ns | 0.0057 ns | 0.0051 ns | 2.401 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   5 | AsciiOnly | 2.445 ns | 0.0077 ns | 0.0072 ns | 2.447 ns |  1.02 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   5 | AsciiOnly | 2.404 ns | 0.0077 ns | 0.0072 ns | 2.401 ns |  1.00 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                   5 | AsciiOnly | 2.438 ns | 0.0088 ns | 0.0083 ns | 2.436 ns |  1.02 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                   **8** | **AsciiOnly** | **2.653 ns** | **0.0067 ns** | **0.0060 ns** | **2.651 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                   8 | AsciiOnly | 2.645 ns | 0.0092 ns | 0.0082 ns | 2.644 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   8 | AsciiOnly | 2.237 ns | 0.0041 ns | 0.0036 ns | 2.236 ns |  0.84 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   8 | AsciiOnly | 3.398 ns | 0.0350 ns | 0.0327 ns | 3.402 ns |  1.28 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                   8 | AsciiOnly | 3.504 ns | 0.0316 ns | 0.0296 ns | 3.501 ns |  1.32 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **15** | **AsciiOnly** | **3.944 ns** | **0.0083 ns** | **0.0078 ns** | **3.940 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  15 | AsciiOnly | 3.946 ns | 0.0082 ns | 0.0064 ns | 3.946 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  15 | AsciiOnly | 2.604 ns | 0.0074 ns | 0.0062 ns | 2.601 ns |  0.66 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  15 | AsciiOnly | 3.896 ns | 0.0059 ns | 0.0052 ns | 3.894 ns |  0.99 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  15 | AsciiOnly | 3.913 ns | 0.0093 ns | 0.0087 ns | 3.911 ns |  0.99 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **16** | **AsciiOnly** | **3.838 ns** | **0.0525 ns** | **0.0439 ns** | **3.824 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  16 | AsciiOnly | 3.814 ns | 0.0086 ns | 0.0080 ns | 3.814 ns |  0.99 |    0.01 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  16 | AsciiOnly | 2.613 ns | 0.0206 ns | 0.0161 ns | 2.608 ns |  0.68 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  16 | AsciiOnly | 3.601 ns | 0.0040 ns | 0.0036 ns | 3.601 ns |  0.94 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  16 | AsciiOnly | 2.762 ns | 0.0065 ns | 0.0055 ns | 2.760 ns |  0.72 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **19** | **AsciiOnly** | **4.703 ns** | **0.0087 ns** | **0.0081 ns** | **4.703 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  19 | AsciiOnly | 4.721 ns | 0.0050 ns | 0.0042 ns | 4.719 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  19 | AsciiOnly | 3.148 ns | 0.0046 ns | 0.0043 ns | 3.149 ns |  0.67 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  19 | AsciiOnly | 3.895 ns | 0.0095 ns | 0.0084 ns | 3.894 ns |  0.83 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  19 | AsciiOnly | 3.325 ns | 0.0108 ns | 0.0096 ns | 3.322 ns |  0.71 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **31** | **AsciiOnly** | **7.121 ns** | **0.4934 ns** | **1.3916 ns** | **6.657 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  31 | AsciiOnly | 6.720 ns | 0.4664 ns | 1.3003 ns | 6.485 ns |  0.98 |    0.25 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  31 | AsciiOnly | 3.864 ns | 0.0527 ns | 0.0440 ns | 3.856 ns |  0.59 |    0.07 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  31 | AsciiOnly | 3.587 ns | 0.0078 ns | 0.0069 ns | 3.586 ns |  0.55 |    0.07 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  31 | AsciiOnly | 3.310 ns | 0.0119 ns | 0.0105 ns | 3.308 ns |  0.50 |    0.06 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **33** | **AsciiOnly** | **4.628 ns** | **0.0037 ns** | **0.0029 ns** | **4.629 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  33 | AsciiOnly | 4.630 ns | 0.0126 ns | 0.0111 ns | 4.628 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  33 | AsciiOnly | 4.277 ns | 0.0069 ns | 0.0061 ns | 4.275 ns |  0.92 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  33 | AsciiOnly | 3.936 ns | 0.0055 ns | 0.0052 ns | 3.936 ns |  0.85 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  33 | AsciiOnly | 3.324 ns | 0.0058 ns | 0.0051 ns | 3.323 ns |  0.72 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **36** | **AsciiOnly** | **4.434 ns** | **0.0075 ns** | **0.0063 ns** | **4.433 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  36 | AsciiOnly | 4.409 ns | 0.0060 ns | 0.0050 ns | 4.410 ns |  0.99 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  36 | AsciiOnly | 4.278 ns | 0.0053 ns | 0.0047 ns | 4.278 ns |  0.96 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  36 | AsciiOnly | 4.467 ns | 0.0122 ns | 0.0114 ns | 4.466 ns |  1.01 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  36 | AsciiOnly | 3.297 ns | 0.0044 ns | 0.0036 ns | 3.298 ns |  0.74 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **39** | **AsciiOnly** | **4.929 ns** | **0.0043 ns** | **0.0036 ns** | **4.930 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  39 | AsciiOnly | 4.929 ns | 0.0088 ns | 0.0078 ns | 4.927 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  39 | AsciiOnly | 4.282 ns | 0.0103 ns | 0.0080 ns | 4.279 ns |  0.87 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  39 | AsciiOnly | 4.334 ns | 0.0089 ns | 0.0075 ns | 4.333 ns |  0.88 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  39 | AsciiOnly | 3.153 ns | 0.0167 ns | 0.0139 ns | 3.149 ns |  0.64 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **41** | **AsciiOnly** | **4.312 ns** | **0.0125 ns** | **0.0116 ns** | **4.306 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  41 | AsciiOnly | 4.308 ns | 0.0087 ns | 0.0081 ns | 4.307 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  41 | AsciiOnly | 4.739 ns | 0.0116 ns | 0.0097 ns | 4.740 ns |  1.10 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  41 | AsciiOnly | 4.763 ns | 0.0044 ns | 0.0037 ns | 4.762 ns |  1.10 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  41 | AsciiOnly | 3.299 ns | 0.0035 ns | 0.0031 ns | 3.299 ns |  0.77 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **44** | **AsciiOnly** | **6.085 ns** | **0.0200 ns** | **0.0187 ns** | **6.090 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  44 | AsciiOnly | 5.321 ns | 0.0101 ns | 0.0090 ns | 5.316 ns |  0.87 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  44 | AsciiOnly | 4.755 ns | 0.0130 ns | 0.0122 ns | 4.754 ns |  0.78 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  44 | AsciiOnly | 4.347 ns | 0.0063 ns | 0.0056 ns | 4.346 ns |  0.71 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  44 | AsciiOnly | 3.317 ns | 0.0056 ns | 0.0053 ns | 3.317 ns |  0.55 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **48** | **AsciiOnly** | **4.476 ns** | **0.0224 ns** | **0.0209 ns** | **4.483 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  48 | AsciiOnly | 4.470 ns | 0.0162 ns | 0.0152 ns | 4.467 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  48 | AsciiOnly | 4.748 ns | 0.0119 ns | 0.0111 ns | 4.745 ns |  1.06 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  48 | AsciiOnly | 4.344 ns | 0.0047 ns | 0.0044 ns | 4.343 ns |  0.97 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  48 | AsciiOnly | 3.271 ns | 0.0068 ns | 0.0060 ns | 3.270 ns |  0.73 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **63** | **AsciiOnly** | **7.260 ns** | **0.0282 ns** | **0.0250 ns** | **7.253 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  63 | AsciiOnly | 7.256 ns | 0.0260 ns | 0.0230 ns | 7.252 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  63 | AsciiOnly | 6.167 ns | 0.0651 ns | 0.0609 ns | 6.174 ns |  0.85 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  63 | AsciiOnly | 4.869 ns | 0.0062 ns | 0.0052 ns | 4.871 ns |  0.67 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  63 | AsciiOnly | 5.514 ns | 0.0285 ns | 0.0266 ns | 5.513 ns |  0.76 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **64** | **AsciiOnly** | **5.269 ns** | **0.0090 ns** | **0.0084 ns** | **5.268 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  64 | AsciiOnly | 5.293 ns | 0.0301 ns | 0.0281 ns | 5.283 ns |  1.00 |    0.01 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  64 | AsciiOnly | 6.119 ns | 0.0693 ns | 0.0614 ns | 6.137 ns |  1.16 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  64 | AsciiOnly | 4.873 ns | 0.0103 ns | 0.0097 ns | 4.869 ns |  0.92 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  64 | AsciiOnly | 5.502 ns | 0.0340 ns | 0.0318 ns | 5.499 ns |  1.04 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **71** | **AsciiOnly** | **7.004 ns** | **0.0280 ns** | **0.0248 ns** | **7.010 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  71 | AsciiOnly | 7.017 ns | 0.0477 ns | 0.0446 ns | 7.016 ns |  1.00 |    0.01 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  71 | AsciiOnly | 7.285 ns | 0.0395 ns | 0.0369 ns | 7.278 ns |  1.04 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  71 | AsciiOnly | 5.724 ns | 0.0262 ns | 0.0245 ns | 5.721 ns |  0.82 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  71 | AsciiOnly | 6.023 ns | 0.0308 ns | 0.0288 ns | 6.027 ns |  0.86 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **79** | **AsciiOnly** | **8.420 ns** | **0.0531 ns** | **0.0471 ns** | **8.406 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  79 | AsciiOnly | 8.021 ns | 0.0323 ns | 0.0287 ns | 8.021 ns |  0.95 |    0.01 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  79 | AsciiOnly | 7.844 ns | 0.0461 ns | 0.0385 ns | 7.843 ns |  0.93 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  79 | AsciiOnly | 5.717 ns | 0.0700 ns | 0.0688 ns | 5.715 ns |  0.68 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  79 | AsciiOnly | 6.652 ns | 0.0269 ns | 0.0238 ns | 6.651 ns |  0.79 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **87** | **AsciiOnly** | **8.030 ns** | **0.0445 ns** | **0.0417 ns** | **8.029 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  87 | AsciiOnly | 8.023 ns | 0.0437 ns | 0.0387 ns | 8.036 ns |  1.00 |    0.01 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  87 | AsciiOnly | 8.346 ns | 0.0394 ns | 0.0307 ns | 8.350 ns |  1.04 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  87 | AsciiOnly | 6.850 ns | 0.0154 ns | 0.0136 ns | 6.853 ns |  0.85 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  87 | AsciiOnly | 6.850 ns | 0.0288 ns | 0.0270 ns | 6.856 ns |  0.85 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **95** | **AsciiOnly** | **8.749 ns** | **0.0117 ns** | **0.0104 ns** | **8.750 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  95 | AsciiOnly | 8.757 ns | 0.0199 ns | 0.0186 ns | 8.755 ns |  1.00 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  95 | AsciiOnly | 8.919 ns | 0.0509 ns | 0.0476 ns | 8.913 ns |  1.02 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  95 | AsciiOnly | 6.859 ns | 0.0188 ns | 0.0176 ns | 6.859 ns |  0.78 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  95 | AsciiOnly | 7.447 ns | 0.0360 ns | 0.0337 ns | 7.454 ns |  0.85 |    0.00 |

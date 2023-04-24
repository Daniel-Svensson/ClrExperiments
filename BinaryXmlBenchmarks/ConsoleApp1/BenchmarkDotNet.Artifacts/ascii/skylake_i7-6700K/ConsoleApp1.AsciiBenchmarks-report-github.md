``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 10 (10.0.19045.2846/22H2/2022Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2
  Job-RJQJBO : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2

MaxRelativeError=0.005  WarmupCount=3  

```
|                                       Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev |   Median | Ratio | RatioSD |
|--------------------------------------------- |-------------------- |---------- |---------:|----------:|----------:|---------:|------:|--------:|
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                   **8** | **AsciiOnly** | **4.281 ns** | **0.0079 ns** | **0.0070 ns** | **4.280 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   8 | AsciiOnly | 3.419 ns | 0.0044 ns | 0.0041 ns | 3.418 ns |  0.80 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   8 | AsciiOnly | 4.928 ns | 0.0318 ns | 0.0435 ns | 4.921 ns |  1.15 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                   8 | AsciiOnly | 4.109 ns | 0.0078 ns | 0.0073 ns | 4.107 ns |  0.96 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **12** | **AsciiOnly** | **4.778 ns** | **0.0065 ns** | **0.0060 ns** | **4.778 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  12 | AsciiOnly | 4.004 ns | 0.0028 ns | 0.0026 ns | 4.005 ns |  0.84 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  12 | AsciiOnly | 7.243 ns | 0.0333 ns | 0.0311 ns | 7.234 ns |  1.52 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  12 | AsciiOnly | 4.105 ns | 0.0073 ns | 0.0065 ns | 4.102 ns |  0.86 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **15** | **AsciiOnly** | **5.707 ns** | **0.0028 ns** | **0.0025 ns** | **5.707 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  15 | AsciiOnly | 4.075 ns | 0.0077 ns | 0.0072 ns | 4.073 ns |  0.71 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  15 | AsciiOnly | 7.357 ns | 0.0084 ns | 0.0074 ns | 7.360 ns |  1.29 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  15 | AsciiOnly | 4.138 ns | 0.0059 ns | 0.0055 ns | 4.137 ns |  0.73 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **16** | **AsciiOnly** | **5.300 ns** | **0.0051 ns** | **0.0047 ns** | **5.298 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  16 | AsciiOnly | 4.088 ns | 0.0076 ns | 0.0071 ns | 4.086 ns |  0.77 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  16 | AsciiOnly | 5.365 ns | 0.1145 ns | 0.3037 ns | 5.251 ns |  1.10 |    0.09 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  16 | AsciiOnly | 4.134 ns | 0.0043 ns | 0.0034 ns | 4.133 ns |  0.78 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **20** | **AsciiOnly** | **5.923 ns** | **0.0139 ns** | **0.0116 ns** | **5.917 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  20 | AsciiOnly | 4.527 ns | 0.0154 ns | 0.0136 ns | 4.522 ns |  0.76 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  20 | AsciiOnly | 5.299 ns | 0.0326 ns | 0.0375 ns | 5.304 ns |  0.89 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  20 | AsciiOnly | 4.721 ns | 0.0130 ns | 0.0121 ns | 4.724 ns |  0.80 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **25** | **AsciiOnly** | **6.558 ns** | **0.0124 ns** | **0.0116 ns** | **6.555 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  25 | AsciiOnly | 5.155 ns | 0.0117 ns | 0.0110 ns | 5.149 ns |  0.79 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  25 | AsciiOnly | 5.663 ns | 0.0111 ns | 0.0098 ns | 5.660 ns |  0.86 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  25 | AsciiOnly | 4.632 ns | 0.0276 ns | 0.0258 ns | 4.634 ns |  0.71 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **33** | **AsciiOnly** | **6.392 ns** | **0.0074 ns** | **0.0069 ns** | **6.393 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  33 | AsciiOnly | 5.893 ns | 0.0108 ns | 0.0095 ns | 5.893 ns |  0.92 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  33 | AsciiOnly | 5.598 ns | 0.0341 ns | 0.0319 ns | 5.603 ns |  0.88 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  33 | AsciiOnly | 4.636 ns | 0.0229 ns | 0.0215 ns | 4.636 ns |  0.73 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **37** | **AsciiOnly** | **7.107 ns** | **0.0091 ns** | **0.0085 ns** | **7.110 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  37 | AsciiOnly | 5.885 ns | 0.0108 ns | 0.0101 ns | 5.886 ns |  0.83 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  37 | AsciiOnly | 5.542 ns | 0.0323 ns | 0.0287 ns | 5.541 ns |  0.78 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  37 | AsciiOnly | 4.649 ns | 0.0259 ns | 0.0230 ns | 4.648 ns |  0.65 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **41** | **AsciiOnly** | **7.696 ns** | **0.0162 ns** | **0.0144 ns** | **7.691 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  41 | AsciiOnly | 6.400 ns | 0.0175 ns | 0.0163 ns | 6.401 ns |  0.83 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  41 | AsciiOnly | 7.582 ns | 0.0219 ns | 0.0194 ns | 7.578 ns |  0.99 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  41 | AsciiOnly | 4.645 ns | 0.0134 ns | 0.0126 ns | 4.650 ns |  0.60 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **44** | **AsciiOnly** | **7.200 ns** | **0.0172 ns** | **0.0153 ns** | **7.197 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  44 | AsciiOnly | 6.399 ns | 0.0162 ns | 0.0152 ns | 6.397 ns |  0.89 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  44 | AsciiOnly | 5.880 ns | 0.0310 ns | 0.0290 ns | 5.887 ns |  0.82 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  44 | AsciiOnly | 4.623 ns | 0.0076 ns | 0.0072 ns | 4.623 ns |  0.64 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **48** | **AsciiOnly** | **7.680 ns** | **0.0081 ns** | **0.0072 ns** | **7.681 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  48 | AsciiOnly | 6.393 ns | 0.0124 ns | 0.0116 ns | 6.395 ns |  0.83 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  48 | AsciiOnly | 5.840 ns | 0.0079 ns | 0.0074 ns | 5.838 ns |  0.76 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  48 | AsciiOnly | 4.671 ns | 0.0104 ns | 0.0097 ns | 4.670 ns |  0.61 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **53** | **AsciiOnly** | **7.925 ns** | **0.0187 ns** | **0.0175 ns** | **7.921 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  53 | AsciiOnly | 8.496 ns | 0.0325 ns | 0.0304 ns | 8.498 ns |  1.07 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  53 | AsciiOnly | 6.488 ns | 0.0362 ns | 0.0496 ns | 6.480 ns |  0.82 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  53 | AsciiOnly | 7.212 ns | 0.0214 ns | 0.0200 ns | 7.212 ns |  0.91 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **57** | **AsciiOnly** | **8.621 ns** | **0.0229 ns** | **0.0214 ns** | **8.612 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  57 | AsciiOnly | 7.445 ns | 0.0121 ns | 0.0113 ns | 7.446 ns |  0.86 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  57 | AsciiOnly | 7.979 ns | 0.0450 ns | 0.0441 ns | 7.987 ns |  0.93 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  57 | AsciiOnly | 7.958 ns | 0.0167 ns | 0.0156 ns | 7.959 ns |  0.92 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **64** | **AsciiOnly** | **9.468 ns** | **0.0126 ns** | **0.0118 ns** | **9.471 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  64 | AsciiOnly | 7.449 ns | 0.0146 ns | 0.0136 ns | 7.452 ns |  0.79 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  64 | AsciiOnly | 6.962 ns | 0.0423 ns | 0.0453 ns | 6.969 ns |  0.73 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  64 | AsciiOnly | 7.796 ns | 0.0306 ns | 0.0286 ns | 7.788 ns |  0.82 |    0.00 |
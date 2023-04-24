``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1555/22H2/2022Update/SunValley2)
AMD Ryzen 9 5900X, 1 CPU, 24 logical and 12 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2
  Job-PLNGBV : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2

MaxRelativeError=0.005  WarmupCount=3  

```
|                                       Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev |   Median | Ratio | RatioSD |
|--------------------------------------------- |-------------------- |---------- |---------:|----------:|----------:|---------:|------:|--------:|
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                   **8** | **AsciiOnly** | **3.207 ns** | **0.0211 ns** | **0.0259 ns** | **3.193 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   8 | AsciiOnly | 2.800 ns | 0.0173 ns | 0.0161 ns | 2.791 ns |  0.87 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   8 | AsciiOnly | 3.609 ns | 0.0110 ns | 0.0092 ns | 3.606 ns |  1.13 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                   8 | AsciiOnly | 3.355 ns | 0.0571 ns | 0.1684 ns | 3.294 ns |  1.04 |    0.04 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **12** | **AsciiOnly** | **3.633 ns** | **0.0181 ns** | **0.0222 ns** | **3.623 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  12 | AsciiOnly | 2.965 ns | 0.0130 ns | 0.0122 ns | 2.960 ns |  0.81 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  12 | AsciiOnly | 4.754 ns | 0.0052 ns | 0.0046 ns | 4.755 ns |  1.31 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  12 | AsciiOnly | 3.208 ns | 0.0205 ns | 0.0319 ns | 3.209 ns |  0.88 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **15** | **AsciiOnly** | **4.091 ns** | **0.0086 ns** | **0.0076 ns** | **4.092 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  15 | AsciiOnly | 3.229 ns | 0.0095 ns | 0.0089 ns | 3.229 ns |  0.79 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  15 | AsciiOnly | 4.726 ns | 0.0284 ns | 0.0398 ns | 4.716 ns |  1.16 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  15 | AsciiOnly | 3.353 ns | 0.0224 ns | 0.0321 ns | 3.348 ns |  0.82 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **16** | **AsciiOnly** | **3.846 ns** | **0.0219 ns** | **0.0205 ns** | **3.856 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  16 | AsciiOnly | 3.022 ns | 0.0066 ns | 0.0051 ns | 3.023 ns |  0.79 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  16 | AsciiOnly | 3.472 ns | 0.0114 ns | 0.0095 ns | 3.468 ns |  0.90 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  16 | AsciiOnly | 3.230 ns | 0.0216 ns | 0.0361 ns | 3.215 ns |  0.85 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **20** | **AsciiOnly** | **4.275 ns** | **0.0254 ns** | **0.0225 ns** | **4.286 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  20 | AsciiOnly | 3.254 ns | 0.0090 ns | 0.0084 ns | 3.254 ns |  0.76 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  20 | AsciiOnly | 3.494 ns | 0.0061 ns | 0.0047 ns | 3.494 ns |  0.82 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  20 | AsciiOnly | 3.371 ns | 0.0216 ns | 0.0231 ns | 3.374 ns |  0.79 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **25** | **AsciiOnly** | **4.807 ns** | **0.0286 ns** | **0.0411 ns** | **4.806 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  25 | AsciiOnly | 3.721 ns | 0.0232 ns | 0.0228 ns | 3.719 ns |  0.77 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  25 | AsciiOnly | 3.908 ns | 0.0092 ns | 0.0072 ns | 3.909 ns |  0.81 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  25 | AsciiOnly | 3.566 ns | 0.0230 ns | 0.0344 ns | 3.567 ns |  0.74 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **33** | **AsciiOnly** | **4.871 ns** | **0.0292 ns** | **0.0324 ns** | **4.889 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  33 | AsciiOnly | 3.874 ns | 0.0064 ns | 0.0053 ns | 3.873 ns |  0.79 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  33 | AsciiOnly | 3.864 ns | 0.0190 ns | 0.0241 ns | 3.854 ns |  0.79 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  33 | AsciiOnly | 3.768 ns | 0.0077 ns | 0.0064 ns | 3.769 ns |  0.77 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **37** | **AsciiOnly** | **4.900 ns** | **0.0082 ns** | **0.0068 ns** | **4.898 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  37 | AsciiOnly | 3.880 ns | 0.0237 ns | 0.0263 ns | 3.875 ns |  0.79 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  37 | AsciiOnly | 3.969 ns | 0.0147 ns | 0.0137 ns | 3.972 ns |  0.81 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  37 | AsciiOnly | 3.511 ns | 0.0217 ns | 0.0213 ns | 3.508 ns |  0.72 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **41** | **AsciiOnly** | **5.322 ns** | **0.0244 ns** | **0.0228 ns** | **5.333 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  41 | AsciiOnly | 4.266 ns | 0.0149 ns | 0.0132 ns | 4.260 ns |  0.80 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  41 | AsciiOnly | 4.338 ns | 0.0095 ns | 0.0084 ns | 4.339 ns |  0.82 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  41 | AsciiOnly | 3.494 ns | 0.0093 ns | 0.0082 ns | 3.496 ns |  0.66 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **44** | **AsciiOnly** | **4.941 ns** | **0.0290 ns** | **0.0311 ns** | **4.926 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  44 | AsciiOnly | 4.271 ns | 0.0079 ns | 0.0074 ns | 4.272 ns |  0.86 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  44 | AsciiOnly | 3.877 ns | 0.0232 ns | 0.0217 ns | 3.886 ns |  0.78 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  44 | AsciiOnly | 3.340 ns | 0.0215 ns | 0.0179 ns | 3.347 ns |  0.68 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **48** | **AsciiOnly** | **5.318 ns** | **0.0321 ns** | **0.0356 ns** | **5.315 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  48 | AsciiOnly | 4.291 ns | 0.0266 ns | 0.0355 ns | 4.294 ns |  0.81 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  48 | AsciiOnly | 3.880 ns | 0.0249 ns | 0.0416 ns | 3.882 ns |  0.73 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  48 | AsciiOnly | 3.369 ns | 0.0206 ns | 0.0212 ns | 3.378 ns |  0.63 |    0.01 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **53** | **AsciiOnly** | **5.326 ns** | **0.0077 ns** | **0.0068 ns** | **5.325 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  53 | AsciiOnly | 4.574 ns | 0.0217 ns | 0.0223 ns | 4.567 ns |  0.86 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  53 | AsciiOnly | 4.296 ns | 0.0038 ns | 0.0030 ns | 4.295 ns |  0.81 |    0.00 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  53 | AsciiOnly | 4.588 ns | 0.0095 ns | 0.0074 ns | 4.589 ns |  0.86 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **57** | **AsciiOnly** | **5.765 ns** | **0.0340 ns** | **0.0302 ns** | **5.755 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  57 | AsciiOnly | 5.030 ns | 0.0075 ns | 0.0067 ns | 5.028 ns |  0.87 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  57 | AsciiOnly | 4.781 ns | 0.0234 ns | 0.0219 ns | 4.785 ns |  0.83 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  57 | AsciiOnly | 4.775 ns | 0.0099 ns | 0.0083 ns | 4.777 ns |  0.83 |    0.00 |
|                                              |                     |           |          |           |           |          |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v2_StoreLower** |                  **64** | **AsciiOnly** | **5.741 ns** | **0.0168 ns** | **0.0157 ns** | **5.731 ns** |  **1.00** |    **0.00** |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  64 | AsciiOnly | 4.905 ns | 0.0133 ns | 0.0124 ns | 4.900 ns |  0.85 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  64 | AsciiOnly | 4.285 ns | 0.0256 ns | 0.0240 ns | 4.289 ns |  0.75 |    0.01 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  64 | AsciiOnly | 4.657 ns | 0.0072 ns | 0.0056 ns | 4.656 ns |  0.81 |    0.00 |
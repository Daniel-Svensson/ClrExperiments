``` ini

BenchmarkDotNet=v0.13.5, OS=ubuntu 23.04
Unknown processor
.NET SDK=8.0.100-preview.3.23178.7
  [Host]     : .NET 8.0.0 (8.0.23.17408), Arm64 RyuJIT AdvSIMD
  Job-JWVYEB : .NET 8.0.0 (8.0.23.17408), Arm64 RyuJIT AdvSIMD

MaxRelativeError=0.01  IterationTime=300.0000 ms  WarmupCount=1  

```
|                                       Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio | RatioSD |
|--------------------------------------------- |-------------------- |---------- |----------:|----------:|----------:|------:|--------:|
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                   **8** | **AsciiOnly** |  **4.382 ns** | **0.0154 ns** | **0.0144 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                   8 | AsciiOnly |  4.349 ns | 0.0098 ns | 0.0087 ns |  0.99 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   8 | AsciiOnly |  5.387 ns | 0.0014 ns | 0.0012 ns |  1.23 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   8 | AsciiOnly |  4.688 ns | 0.0030 ns | 0.0025 ns |  1.07 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **15** | **AsciiOnly** |  **5.903 ns** | **0.0043 ns** | **0.0036 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  15 | AsciiOnly |  6.315 ns | 0.0023 ns | 0.0020 ns |  1.07 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  15 | AsciiOnly |  6.157 ns | 0.0032 ns | 0.0027 ns |  1.04 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  15 | AsciiOnly |  6.798 ns | 0.0020 ns | 0.0017 ns |  1.15 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **16** | **AsciiOnly** |  **6.099 ns** | **0.0188 ns** | **0.0176 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  16 | AsciiOnly |  6.222 ns | 0.0044 ns | 0.0037 ns |  1.02 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  16 | AsciiOnly |  4.633 ns | 0.0042 ns | 0.0037 ns |  0.76 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  16 | AsciiOnly |  5.761 ns | 0.0023 ns | 0.0019 ns |  0.94 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **31** | **AsciiOnly** | **12.419 ns** | **0.0070 ns** | **0.0062 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  31 | AsciiOnly | 13.180 ns | 0.0080 ns | 0.0067 ns |  1.06 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  31 | AsciiOnly |  6.888 ns | 0.0349 ns | 0.0310 ns |  0.55 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  31 | AsciiOnly |  7.259 ns | 0.0183 ns | 0.0171 ns |  0.58 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **33** | **AsciiOnly** |  **6.632 ns** | **0.0075 ns** | **0.0066 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  33 | AsciiOnly |  5.329 ns | 0.0322 ns | 0.0301 ns |  0.80 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  33 | AsciiOnly |  7.823 ns | 0.0221 ns | 0.0207 ns |  1.18 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  33 | AsciiOnly |  7.805 ns | 0.0023 ns | 0.0019 ns |  1.18 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **36** | **AsciiOnly** |  **9.021 ns** | **0.0043 ns** | **0.0038 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  36 | AsciiOnly |  7.817 ns | 0.0020 ns | 0.0019 ns |  0.87 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  36 | AsciiOnly |  5.939 ns | 0.0120 ns | 0.0100 ns |  0.66 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  36 | AsciiOnly |  6.140 ns | 0.0026 ns | 0.0021 ns |  0.68 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **39** | **AsciiOnly** | **10.794 ns** | **0.0509 ns** | **0.0476 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  39 | AsciiOnly |  9.089 ns | 0.0030 ns | 0.0025 ns |  0.84 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  39 | AsciiOnly |  7.118 ns | 0.0110 ns | 0.0103 ns |  0.66 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  39 | AsciiOnly |  7.360 ns | 0.0026 ns | 0.0023 ns |  0.68 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **41** | **AsciiOnly** |  **9.134 ns** | **0.0125 ns** | **0.0117 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  41 | AsciiOnly |  7.821 ns | 0.0063 ns | 0.0059 ns |  0.86 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  41 | AsciiOnly |  7.775 ns | 0.0065 ns | 0.0051 ns |  0.85 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  41 | AsciiOnly |  7.945 ns | 0.0105 ns | 0.0098 ns |  0.87 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **44** | **AsciiOnly** |  **7.175 ns** | **0.0087 ns** | **0.0082 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  44 | AsciiOnly |  6.428 ns | 0.1054 ns | 0.1127 ns |  0.90 |    0.02 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  44 | AsciiOnly |  7.128 ns | 0.0034 ns | 0.0029 ns |  0.99 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  44 | AsciiOnly |  6.462 ns | 0.0091 ns | 0.0085 ns |  0.90 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **48** | **AsciiOnly** |  **8.719 ns** | **0.0374 ns** | **0.0350 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  48 | AsciiOnly |  7.289 ns | 0.0106 ns | 0.0099 ns |  0.84 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  48 | AsciiOnly |  6.615 ns | 0.0089 ns | 0.0079 ns |  0.76 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  48 | AsciiOnly |  6.481 ns | 0.0132 ns | 0.0123 ns |  0.74 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **63** | **AsciiOnly** | **10.861 ns** | **0.0090 ns** | **0.0080 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  63 | AsciiOnly | 10.332 ns | 0.0052 ns | 0.0049 ns |  0.95 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  63 | AsciiOnly |  9.441 ns | 0.0052 ns | 0.0043 ns |  0.87 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  63 | AsciiOnly |  8.957 ns | 0.0156 ns | 0.0145 ns |  0.82 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **64** | **AsciiOnly** | **10.200 ns** | **0.0072 ns** | **0.0064 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  64 | AsciiOnly | 10.064 ns | 0.0021 ns | 0.0018 ns |  0.99 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  64 | AsciiOnly |  9.181 ns | 0.0146 ns | 0.0136 ns |  0.90 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  64 | AsciiOnly |  8.121 ns | 0.0180 ns | 0.0150 ns |  0.80 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **71** | **AsciiOnly** | **13.473 ns** | **0.0183 ns** | **0.0162 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  71 | AsciiOnly | 12.290 ns | 0.0065 ns | 0.0058 ns |  0.91 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  71 | AsciiOnly | 11.254 ns | 0.0098 ns | 0.0087 ns |  0.84 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  71 | AsciiOnly |  9.725 ns | 0.0205 ns | 0.0192 ns |  0.72 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **79** | **AsciiOnly** | **11.805 ns** | **0.0076 ns** | **0.0064 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  79 | AsciiOnly | 10.478 ns | 0.0152 ns | 0.0127 ns |  0.89 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  79 | AsciiOnly | 12.929 ns | 0.0604 ns | 0.0565 ns |  1.10 |    0.01 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  79 | AsciiOnly | 11.290 ns | 0.0249 ns | 0.0233 ns |  0.96 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **87** | **AsciiOnly** | **11.813 ns** | **0.0164 ns** | **0.0154 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  87 | AsciiOnly | 10.414 ns | 0.0182 ns | 0.0170 ns |  0.88 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  87 | AsciiOnly | 13.561 ns | 0.0467 ns | 0.0437 ns |  1.15 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  87 | AsciiOnly | 10.807 ns | 0.0340 ns | 0.0318 ns |  0.91 |    0.00 |
|                                              |                     |           |           |           |           |       |         |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **95** | **AsciiOnly** | **17.004 ns** | **0.0444 ns** | **0.0415 ns** |  **1.00** |    **0.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  95 | AsciiOnly | 14.706 ns | 0.0574 ns | 0.0537 ns |  0.86 |    0.00 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  95 | AsciiOnly | 15.063 ns | 0.0183 ns | 0.0171 ns |  0.89 |    0.00 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  95 | AsciiOnly | 12.036 ns | 0.0506 ns | 0.0449 ns |  0.71 |    0.00 |

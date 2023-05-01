``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 10 (10.0.19045.2846/22H2/2022Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2
  Job-AJZMLX : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=300.0000 ms  WarmupCount=1  

```
|                                       Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev | Ratio |
|--------------------------------------------- |-------------------- |---------- |---------:|----------:|----------:|------:|
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                   **5** | **AsciiOnly** | **2.874 ns** | **0.0104 ns** | **0.0093 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                   5 | AsciiOnly | 2.354 ns | 0.0026 ns | 0.0023 ns |  0.82 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                   5 | AsciiOnly | 2.411 ns | 0.0095 ns | 0.0084 ns |  0.84 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                   5 | AsciiOnly | 2.356 ns | 0.0044 ns | 0.0037 ns |  0.82 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                   5 | AsciiOnly | 2.362 ns | 0.0051 ns | 0.0045 ns |  0.82 |
|                                              |                     |           |          |           |           |       |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **15** | **AsciiOnly** | **4.295 ns** | **0.0079 ns** | **0.0070 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  15 | AsciiOnly | 4.124 ns | 0.0116 ns | 0.0109 ns |  0.96 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  15 | AsciiOnly | 2.693 ns | 0.0047 ns | 0.0042 ns |  0.63 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  15 | AsciiOnly | 3.973 ns | 0.0388 ns | 0.0363 ns |  0.93 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  15 | AsciiOnly | 3.971 ns | 0.0059 ns | 0.0049 ns |  0.92 |
|                                              |                     |           |          |           |           |       |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **31** | **AsciiOnly** | **5.997 ns** | **0.0130 ns** | **0.0115 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  31 | AsciiOnly | 5.606 ns | 0.0129 ns | 0.0108 ns |  0.93 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  31 | AsciiOnly | 3.793 ns | 0.0064 ns | 0.0057 ns |  0.63 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  31 | AsciiOnly | 3.937 ns | 0.0064 ns | 0.0059 ns |  0.66 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  31 | AsciiOnly | 3.323 ns | 0.0032 ns | 0.0028 ns |  0.55 |
|                                              |                     |           |          |           |           |       |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **33** | **AsciiOnly** | **4.650 ns** | **0.0127 ns** | **0.0113 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  33 | AsciiOnly | 4.244 ns | 0.0089 ns | 0.0083 ns |  0.91 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  33 | AsciiOnly | 4.272 ns | 0.0059 ns | 0.0055 ns |  0.92 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  33 | AsciiOnly | 4.327 ns | 0.0033 ns | 0.0026 ns |  0.93 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  33 | AsciiOnly | 3.315 ns | 0.0056 ns | 0.0049 ns |  0.71 |
|                                              |                     |           |          |           |           |       |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **41** | **AsciiOnly** | **5.491 ns** | **0.0172 ns** | **0.0152 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  41 | AsciiOnly | 5.086 ns | 0.0077 ns | 0.0068 ns |  0.93 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  41 | AsciiOnly | 4.755 ns | 0.0131 ns | 0.0109 ns |  0.87 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  41 | AsciiOnly | 4.668 ns | 0.0080 ns | 0.0071 ns |  0.85 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  41 | AsciiOnly | 3.301 ns | 0.0155 ns | 0.0129 ns |  0.60 |
|                                              |                     |           |          |           |           |       |
| **Ascii_Local_NarrowUtf16ToAscii_v1_StoreLower** |                  **64** | **AsciiOnly** | **6.827 ns** | **0.0420 ns** | **0.0393 ns** |  **1.00** |
|     Ascii_Local_NarrowUtf16ToAscii_v2_Inline |                  64 | AsciiOnly | 6.017 ns | 0.0211 ns | 0.0197 ns |  0.88 |
|   Ascii_Local_NarrowUtf16ToAscii_simple_loop |                  64 | AsciiOnly | 6.154 ns | 0.0631 ns | 0.0590 ns |  0.90 |
|            Ascii_Local_NarrowUtf16ToAscii_v3 |                  64 | AsciiOnly | 5.254 ns | 0.0134 ns | 0.0119 ns |  0.77 |
|         Ascii_Local_NarrowUtf16ToAscii_v4_if |                  64 | AsciiOnly | 5.221 ns | 0.0385 ns | 0.0360 ns |  0.76 |

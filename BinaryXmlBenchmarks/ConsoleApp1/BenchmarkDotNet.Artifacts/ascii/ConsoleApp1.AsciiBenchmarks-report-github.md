``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1413/22H2/2022Update/SunValley2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=8.0.100-preview.1.23115.2
  [Host]     : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2
  Job-HILNSM : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                            Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev |   Median | Ratio | RatioSD |
|---------------------------------- |-------------------- |---------- |---------:|----------:|----------:|---------:|------:|--------:|
|                 **System_Text_Ascii** |                   **5** | **AsciiOnly** | **5.526 ns** | **0.0089 ns** | **0.0070 ns** | **5.527 ns** |  **1.00** |    **0.00** |
|       System_Text_Ascii_NoPinning |                   5 | AsciiOnly | 5.046 ns | 0.0247 ns | 0.0193 ns | 5.043 ns |  0.91 |    0.00 |
|             Ascii_Local_NoPinning |                   5 | AsciiOnly | 5.099 ns | 0.0174 ns | 0.0154 ns | 5.100 ns |  0.92 |    0.00 |
| Ascii_Local_NarrowUtf16ToAscii_v2 |                   5 | AsciiOnly | 3.298 ns | 0.0076 ns | 0.0067 ns | 3.300 ns |  0.60 |    0.00 |
|                Ascii_Local_Unsafe |                   5 | AsciiOnly | 3.367 ns | 0.0069 ns | 0.0065 ns | 3.367 ns |  0.61 |    0.00 |
|    Ascii_Local_NarrowUtf16ToAscii |                   5 | AsciiOnly | 3.363 ns | 0.0158 ns | 0.0132 ns | 3.364 ns |  0.61 |    0.00 |
|                                   |                     |           |          |           |           |          |       |         |
|                 **System_Text_Ascii** |                  **20** | **AsciiOnly** | **6.808 ns** | **0.0203 ns** | **0.0180 ns** | **6.810 ns** |  **1.00** |    **0.00** |
|       System_Text_Ascii_NoPinning |                  20 | AsciiOnly | 6.347 ns | 0.0179 ns | 0.0159 ns | 6.347 ns |  0.93 |    0.00 |
|             Ascii_Local_NoPinning |                  20 | AsciiOnly | 5.904 ns | 0.0125 ns | 0.0104 ns | 5.904 ns |  0.87 |    0.00 |
| Ascii_Local_NarrowUtf16ToAscii_v2 |                  20 | AsciiOnly | 4.624 ns | 0.0305 ns | 0.0285 ns | 4.639 ns |  0.68 |    0.00 |
|                Ascii_Local_Unsafe |                  20 | AsciiOnly | 4.108 ns | 0.0143 ns | 0.0127 ns | 4.107 ns |  0.60 |    0.00 |
|    Ascii_Local_NarrowUtf16ToAscii |                  20 | AsciiOnly | 4.205 ns | 0.0280 ns | 0.0248 ns | 4.202 ns |  0.62 |    0.00 |
|                                   |                     |           |          |           |           |          |       |         |
|                 **System_Text_Ascii** |                  **32** | **AsciiOnly** | **7.501 ns** | **0.0481 ns** | **0.0426 ns** | **7.513 ns** |  **1.00** |    **0.00** |
|       System_Text_Ascii_NoPinning |                  32 | AsciiOnly | 7.402 ns | 0.0231 ns | 0.0217 ns | 7.404 ns |  0.99 |    0.00 |
|             Ascii_Local_NoPinning |                  32 | AsciiOnly | 6.568 ns | 0.0214 ns | 0.0200 ns | 6.571 ns |  0.88 |    0.00 |
| Ascii_Local_NarrowUtf16ToAscii_v2 |                  32 | AsciiOnly | 5.489 ns | 0.0087 ns | 0.0072 ns | 5.490 ns |  0.73 |    0.00 |
|                Ascii_Local_Unsafe |                  32 | AsciiOnly | 4.447 ns | 0.0247 ns | 0.0231 ns | 4.448 ns |  0.59 |    0.00 |
|    Ascii_Local_NarrowUtf16ToAscii |                  32 | AsciiOnly | 4.634 ns | 0.0145 ns | 0.0135 ns | 4.635 ns |  0.62 |    0.00 |
|                                   |                     |           |          |           |           |          |       |         |
|                 **System_Text_Ascii** |                  **40** | **AsciiOnly** | **7.445 ns** | **0.0272 ns** | **0.0255 ns** | **7.445 ns** |  **1.00** |    **0.00** |
|       System_Text_Ascii_NoPinning |                  40 | AsciiOnly | 7.479 ns | 0.0858 ns | 0.1085 ns | 7.511 ns |  1.00 |    0.02 |
|             Ascii_Local_NoPinning |                  40 | AsciiOnly | 6.555 ns | 0.0294 ns | 0.0275 ns | 6.555 ns |  0.88 |    0.00 |
| Ascii_Local_NarrowUtf16ToAscii_v2 |                  40 | AsciiOnly | 4.684 ns | 0.0132 ns | 0.0124 ns | 4.685 ns |  0.63 |    0.00 |
|                Ascii_Local_Unsafe |                  40 | AsciiOnly | 4.926 ns | 0.0126 ns | 0.0118 ns | 4.924 ns |  0.66 |    0.00 |
|    Ascii_Local_NarrowUtf16ToAscii |                  40 | AsciiOnly | 4.891 ns | 0.0326 ns | 0.0305 ns | 4.890 ns |  0.66 |    0.00 |
|                                   |                     |           |          |           |           |          |       |         |
|                 **System_Text_Ascii** |                  **63** | **AsciiOnly** | **8.214 ns** | **0.0917 ns** | **0.1401 ns** | **8.182 ns** |  **1.00** |    **0.00** |
|       System_Text_Ascii_NoPinning |                  63 | AsciiOnly | 8.000 ns | 0.0917 ns | 0.2526 ns | 7.857 ns |  1.00 |    0.03 |
|             Ascii_Local_NoPinning |                  63 | AsciiOnly | 7.441 ns | 0.0147 ns | 0.0130 ns | 7.443 ns |  0.91 |    0.01 |
| Ascii_Local_NarrowUtf16ToAscii_v2 |                  63 | AsciiOnly | 5.863 ns | 0.0381 ns | 0.0357 ns | 5.853 ns |  0.72 |    0.01 |
|                Ascii_Local_Unsafe |                  63 | AsciiOnly | 5.449 ns | 0.0260 ns | 0.0217 ns | 5.452 ns |  0.67 |    0.01 |
|    Ascii_Local_NarrowUtf16ToAscii |                  63 | AsciiOnly | 5.648 ns | 0.0662 ns | 0.0970 ns | 5.635 ns |  0.69 |    0.01 |

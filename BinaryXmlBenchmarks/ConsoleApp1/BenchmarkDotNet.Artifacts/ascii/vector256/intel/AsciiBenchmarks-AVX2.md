``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 10 (10.0.19045.2846/22H2/2022Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2
  Job-QMYPNO : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=300.0000 ms  WarmupCount=1  

```
|                             Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio |
|----------------------------------- |-------------------- |---------- |----------:|----------:|----------:|------:|
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                   **8** | **AsciiOnly** |  **4.018 ns** | **0.0219 ns** | **0.0205 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                   8 | AsciiOnly |  4.321 ns | 0.0196 ns | 0.0183 ns |  1.08 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                   8 | AsciiOnly |  4.534 ns | 0.0336 ns | 0.0314 ns |  1.13 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **20** | **AsciiOnly** |  **4.986 ns** | **0.0093 ns** | **0.0078 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  20 | AsciiOnly |  5.155 ns | 0.0154 ns | 0.0129 ns |  1.03 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  20 | AsciiOnly |  6.802 ns | 0.0406 ns | 0.0380 ns |  1.36 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **33** | **AsciiOnly** |  **5.834 ns** | **0.0281 ns** | **0.0249 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  33 | AsciiOnly |  5.227 ns | 0.0174 ns | 0.0145 ns |  0.90 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  33 | AsciiOnly |  5.064 ns | 0.0123 ns | 0.0103 ns |  0.87 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **65** | **AsciiOnly** |  **8.651 ns** | **0.0450 ns** | **0.0399 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  65 | AsciiOnly |  7.064 ns | 0.0385 ns | 0.0341 ns |  0.82 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  65 | AsciiOnly |  6.312 ns | 0.0275 ns | 0.0257 ns |  0.73 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                 **100** | **AsciiOnly** | **10.449 ns** | **0.0406 ns** | **0.0360 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                 100 | AsciiOnly |  8.860 ns | 0.0542 ns | 0.0452 ns |  0.85 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                 100 | AsciiOnly |  8.344 ns | 0.0839 ns | 0.0785 ns |  0.80 |

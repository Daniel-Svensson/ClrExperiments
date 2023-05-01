``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 10 (10.0.19045.2846/22H2/2022Update)
Intel Core i7-6700K CPU 4.00GHz (Skylake), 1 CPU, 8 logical and 4 physical cores
.NET SDK=8.0.100-preview.2.23157.25
  [Host]     : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT SSE4.2
  Job-LTKEMN : .NET 8.0.0 (8.0.23.12803), X64 RyuJIT SSE4.2

MaxRelativeError=0.01  IterationTime=300.0000 ms  WarmupCount=1  

```
|                             Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio |
|----------------------------------- |-------------------- |---------- |----------:|----------:|----------:|------:|
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                   **8** | **AsciiOnly** |  **4.677 ns** | **0.0303 ns** | **0.0268 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                   8 | AsciiOnly |  3.967 ns | 0.0219 ns | 0.0194 ns |  0.85 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                   8 | AsciiOnly |  4.253 ns | 0.0173 ns | 0.0154 ns |  0.91 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **20** | **AsciiOnly** |  **4.750 ns** | **0.0161 ns** | **0.0135 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  20 | AsciiOnly |  5.516 ns | 0.0188 ns | 0.0176 ns |  1.16 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  20 | AsciiOnly |  6.374 ns | 0.0061 ns | 0.0051 ns |  1.34 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **33** | **AsciiOnly** |  **5.862 ns** | **0.0246 ns** | **0.0230 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  33 | AsciiOnly |  7.006 ns | 0.0400 ns | 0.0374 ns |  1.20 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  33 | AsciiOnly |  6.340 ns | 0.0172 ns | 0.0152 ns |  1.08 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **65** | **AsciiOnly** |  **8.260 ns** | **0.0450 ns** | **0.0421 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  65 | AsciiOnly |  9.812 ns | 0.0391 ns | 0.0346 ns |  1.19 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  65 | AsciiOnly |  8.950 ns | 0.0369 ns | 0.0345 ns |  1.08 |
|                                    |                     |           |           |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                 **100** | **AsciiOnly** |  **9.950 ns** | **0.0233 ns** | **0.0206 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                 100 | AsciiOnly | 12.452 ns | 0.0442 ns | 0.0413 ns |  1.25 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                 100 | AsciiOnly | 10.278 ns | 0.0376 ns | 0.0352 ns |  1.03 |

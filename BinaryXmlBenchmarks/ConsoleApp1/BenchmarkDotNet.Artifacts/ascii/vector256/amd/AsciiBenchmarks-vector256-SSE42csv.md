``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1555/22H2/2022Update/SunValley2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=8.0.100-preview.1.23115.2
  [Host]     : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT SSE4.2
  Job-LVUIAG : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT SSE4.2

MaxRelativeError=0.01  IterationTime=250.0000 ms  WarmupCount=3  

```
|                             Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev | Ratio |
|----------------------------------- |-------------------- |---------- |---------:|----------:|----------:|------:|
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                   **8** | **AsciiOnly** | **3.535 ns** | **0.0189 ns** | **0.0157 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                   8 | AsciiOnly | 3.530 ns | 0.0423 ns | 0.0353 ns |  1.00 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                   8 | AsciiOnly | 3.750 ns | 0.0316 ns | 0.0281 ns |  1.06 |
|                                    |                     |           |          |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **20** | **AsciiOnly** | **4.150 ns** | **0.0190 ns** | **0.0178 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  20 | AsciiOnly | 4.196 ns | 0.0308 ns | 0.0273 ns |  1.01 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  20 | AsciiOnly | 4.676 ns | 0.0358 ns | 0.0317 ns |  1.13 |
|                                    |                     |           |          |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **33** | **AsciiOnly** | **4.349 ns** | **0.0195 ns** | **0.0183 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  33 | AsciiOnly | 4.351 ns | 0.0141 ns | 0.0118 ns |  1.00 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  33 | AsciiOnly | 4.390 ns | 0.0381 ns | 0.0338 ns |  1.01 |
|                                    |                     |           |          |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                  **65** | **AsciiOnly** | **5.207 ns** | **0.0135 ns** | **0.0127 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                  65 | AsciiOnly | 5.184 ns | 0.0133 ns | 0.0125 ns |  1.00 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                  65 | AsciiOnly | 4.995 ns | 0.0146 ns | 0.0137 ns |  0.96 |
|                                    |                     |           |          |           |           |       |
|  **Ascii_Local_NarrowUtf16ToAscii_v3** |                 **100** | **AsciiOnly** | **6.036 ns** | **0.0259 ns** | **0.0242 ns** |  **1.00** |
| Ascii_Local_NarrowUtf16ToAscii_v3b |                 100 | AsciiOnly | 6.055 ns | 0.0228 ns | 0.0214 ns |  1.00 |
| Ascii_Local_NarrowUtf16ToAscii_v3c |                 100 | AsciiOnly | 5.829 ns | 0.0144 ns | 0.0128 ns |  0.97 |

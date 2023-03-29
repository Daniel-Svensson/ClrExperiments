``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1413/22H2/2022Update/SunValley2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=8.0.100-preview.1.23115.2
  [Host]     : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2
  Job-NDTCCJ : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|                               Method | StringLengthInChars |  Scenario |     Mean |     Error |    StdDev |
|------------------------------------- |-------------------- |---------- |---------:|----------:|----------:|
|    **Ascii_Local_NarrowUtf16ToAscii_v2** |                  **32** | **AsciiOnly** | **5.066 ns** | **0.0099 ns** | **0.0092 ns** |
|       Ascii_Local_NarrowUtf16ToAscii |                  32 | AsciiOnly | 4.658 ns | 0.0255 ns | 0.0213 ns |
|    Ascii_Local_NarrowUtf16ToAscii_v3 |                  32 | AsciiOnly | 4.251 ns | 0.0423 ns | 0.0396 ns |
| Ascii_Local_NarrowUtf16ToAscii_v4_if |                  32 | AsciiOnly | 4.685 ns | 0.0568 ns | 0.0917 ns |
|    **Ascii_Local_NarrowUtf16ToAscii_v2** |                  **37** | **AsciiOnly** | **5.716 ns** | **0.0183 ns** | **0.0162 ns** |
|       Ascii_Local_NarrowUtf16ToAscii |                  37 | AsciiOnly | 5.087 ns | 0.0198 ns | 0.0165 ns |
|    Ascii_Local_NarrowUtf16ToAscii_v3 |                  37 | AsciiOnly | 4.237 ns | 0.0207 ns | 0.0173 ns |
| Ascii_Local_NarrowUtf16ToAscii_v4_if |                  37 | AsciiOnly | 4.858 ns | 0.0302 ns | 0.0282 ns |
|    **Ascii_Local_NarrowUtf16ToAscii_v2** |                  **52** | **AsciiOnly** | **5.472 ns** | **0.0314 ns** | **0.0294 ns** |
|       Ascii_Local_NarrowUtf16ToAscii |                  52 | AsciiOnly | 5.676 ns | 0.0270 ns | 0.0239 ns |
|    Ascii_Local_NarrowUtf16ToAscii_v3 |                  52 | AsciiOnly | 4.825 ns | 0.0594 ns | 0.1025 ns |
| Ascii_Local_NarrowUtf16ToAscii_v4_if |                  52 | AsciiOnly | 5.291 ns | 0.0454 ns | 0.0402 ns |
|    **Ascii_Local_NarrowUtf16ToAscii_v2** |                  **63** | **AsciiOnly** | **5.831 ns** | **0.0267 ns** | **0.0236 ns** |
|       Ascii_Local_NarrowUtf16ToAscii |                  63 | AsciiOnly | 5.888 ns | 0.0218 ns | 0.0204 ns |
|    Ascii_Local_NarrowUtf16ToAscii_v3 |                  63 | AsciiOnly | 5.032 ns | 0.0273 ns | 0.0256 ns |
| Ascii_Local_NarrowUtf16ToAscii_v4_if |                  63 | AsciiOnly | 5.699 ns | 0.0359 ns | 0.0318 ns |
|    **Ascii_Local_NarrowUtf16ToAscii_v2** |                  **68** | **AsciiOnly** | **6.327 ns** | **0.0319 ns** | **0.0283 ns** |
|       Ascii_Local_NarrowUtf16ToAscii |                  68 | AsciiOnly | 6.435 ns | 0.0676 ns | 0.1148 ns |
|    Ascii_Local_NarrowUtf16ToAscii_v3 |                  68 | AsciiOnly | 5.023 ns | 0.0310 ns | 0.0290 ns |
| Ascii_Local_NarrowUtf16ToAscii_v4_if |                  68 | AsciiOnly | 5.509 ns | 0.0272 ns | 0.0227 ns |

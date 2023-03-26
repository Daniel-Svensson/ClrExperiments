``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1413/22H2/2022Update/SunValley2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=8.0.100-preview.1.23115.2
  [Host]     : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2
  Job-ACRLYQ : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|            Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |
|------------------ |-------------------- |---------- |----------:|----------:|----------:|
|          **Original** |                   **6** | **AsciiOnly** |  **3.912 ns** | **0.0200 ns** | **0.0167 ns** |
|               New |                   6 | AsciiOnly |  3.718 ns | 0.0260 ns | 0.0243 ns |
| Encoding_GetBytes |                   6 | AsciiOnly |  7.191 ns | 0.0293 ns | 0.0260 ns |
|          **Original** |                   **8** | **AsciiOnly** |  **4.442 ns** | **0.0284 ns** | **0.0251 ns** |
|               New |                   8 | AsciiOnly |  4.214 ns | 0.0191 ns | 0.0178 ns |
| Encoding_GetBytes |                   8 | AsciiOnly |  7.358 ns | 0.0625 ns | 0.0554 ns |
|          **Original** |                  **12** | **AsciiOnly** |  **6.176 ns** | **0.0725 ns** | **0.0642 ns** |
|               New |                  12 | AsciiOnly |  6.104 ns | 0.0693 ns | 0.0648 ns |
| Encoding_GetBytes |                  12 | AsciiOnly |  7.562 ns | 0.0198 ns | 0.0176 ns |
|          **Original** |                  **30** | **AsciiOnly** | **10.051 ns** | **0.0471 ns** | **0.0393 ns** |
|               New |                  30 | AsciiOnly |  9.946 ns | 0.0431 ns | 0.0403 ns |
| Encoding_GetBytes |                  30 | AsciiOnly |  9.107 ns | 0.0736 ns | 0.0652 ns |
|          **Original** |                  **50** | **AsciiOnly** | **17.869 ns** | **0.1720 ns** | **0.1609 ns** |
|               New |                  50 | AsciiOnly | 10.571 ns | 0.0442 ns | 0.0392 ns |
| Encoding_GetBytes |                  50 | AsciiOnly |  9.480 ns | 0.0200 ns | 0.0167 ns |

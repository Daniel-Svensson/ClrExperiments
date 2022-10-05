``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 5 5600U with Radeon Graphics, 1 CPU, 12 logical and 6 physical cores
.NET SDK=7.0.100-preview.5.22307.18
  [Host]     : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT
  Job-CMLNSP : .NET 7.0.0 (7.0.22.30112), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|          Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |
|---------------- |-------------------- |---------- |----------:|----------:|----------:|
| **OriginalUpdated** |                   **5** | **AsciiOnly** |  **3.850 ns** | **0.0477 ns** | **0.0422 ns** |
|         SSE_v42 |                   5 | AsciiOnly |  4.346 ns | 0.0384 ns | 0.0321 ns |
|         ForLoop |                   5 | AsciiOnly |  3.991 ns | 0.0325 ns | 0.0304 ns |
|      SimdSSE_v4 |                   5 | AsciiOnly |  3.534 ns | 0.0476 ns | 0.0467 ns |
| **OriginalUpdated** |                   **8** | **AsciiOnly** |  **5.255 ns** | **0.0635 ns** | **0.0563 ns** |
|         SSE_v42 |                   8 | AsciiOnly |  1.954 ns | 0.0318 ns | 0.0326 ns |
|         ForLoop |                   8 | AsciiOnly |  5.069 ns | 0.0140 ns | 0.0124 ns |
|      SimdSSE_v4 |                   8 | AsciiOnly |  2.922 ns | 0.0411 ns | 0.0791 ns |
| **OriginalUpdated** |                  **85** | **AsciiOnly** | **24.478 ns** | **0.0570 ns** | **0.0476 ns** |
|         SSE_v42 |                  85 | AsciiOnly |  6.623 ns | 0.0078 ns | 0.0069 ns |
|         ForLoop |                  85 | AsciiOnly | 30.050 ns | 0.1530 ns | 0.1278 ns |
|      SimdSSE_v4 |                  85 | AsciiOnly |  7.796 ns | 0.0620 ns | 0.0550 ns |

``` ini

BenchmarkDotNet=v0.13.5, OS=Windows 11 (10.0.22621.1413/22H2/2022Update/SunValley2)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=8.0.100-preview.1.23115.2
  [Host]     : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2
  Job-VUQEPE : .NET 8.0.0 (8.0.23.11008), X64 RyuJIT AVX2

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|            Method | StringLengthInChars | Scenario |      Mean |     Error |    StdDev |    Median |
|------------------ |-------------------- |--------- |----------:|----------:|----------:|----------:|
|          **Original** |                   **6** |    **Mixed** | **15.348 ns** | **0.1643 ns** | **0.4607 ns** | **15.378 ns** |
|               New |                   6 |    Mixed | 12.537 ns | 0.2874 ns | 0.8430 ns | 12.659 ns |
| Encoding_GetBytes |                   6 |    Mixed |  9.055 ns | 0.0488 ns | 0.0433 ns |  9.065 ns |
|          **Original** |                   **8** |    **Mixed** | **14.641 ns** | **0.1614 ns** | **0.3989 ns** | **14.543 ns** |
|               New |                   8 |    Mixed | 12.059 ns | 0.0981 ns | 0.0869 ns | 12.049 ns |
| Encoding_GetBytes |                   8 |    Mixed | 10.249 ns | 0.1106 ns | 0.1848 ns | 10.221 ns |
|          **Original** |                  **12** |    **Mixed** | **25.895 ns** | **0.1992 ns** | **0.1556 ns** | **25.889 ns** |
|               New |                  12 |    Mixed | 14.898 ns | 0.0436 ns | 0.0387 ns | 14.902 ns |
| Encoding_GetBytes |                  12 |    Mixed | 12.902 ns | 0.0401 ns | 0.0313 ns | 12.914 ns |
|          **Original** |                  **30** |    **Mixed** | **31.711 ns** | **0.4138 ns** | **1.2202 ns** | **31.216 ns** |
|               New |                  30 |    Mixed | 16.281 ns | 0.1215 ns | 0.1077 ns | 16.275 ns |
| Encoding_GetBytes |                  30 |    Mixed | 15.422 ns | 0.0800 ns | 0.0748 ns | 15.422 ns |
|          **Original** |                  **50** |    **Mixed** | **46.075 ns** | **0.3741 ns** | **0.4003 ns** | **46.022 ns** |
|               New |                  50 |    Mixed | 20.667 ns | 0.0915 ns | 0.0856 ns | 20.649 ns |
| Encoding_GetBytes |                  50 |    Mixed | 20.046 ns | 0.1029 ns | 0.0962 ns | 20.057 ns |

```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.22631.3155/23H2/2023Update/SunValley3)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 8.0.200
  [Host] : .NET 8.0.2 (8.0.224.6711), X86 RyuJIT AVX2

Job=InProcess  Toolchain=InProcessEmitToolchain  

```
| Method | RoundedAmounts | SmallDivisor | Count | Mean     | Error     | StdDev    | Ratio |
|------- |--------------- |------------- |------ |---------:|----------:|----------:|------:|
| **New**    | **True**           | **False**        | **10000** | **2.182 ms** | **0.0088 ms** | **0.0078 ms** |  **0.94** |
| Main   | True           | False        | 10000 | 2.326 ms | 0.0041 ms | 0.0037 ms |  1.00 |
|        |                |              |       |          |           |           |       |
| **New**    | **True**           | **True**         | **10000** | **1.816 ms** | **0.0014 ms** | **0.0013 ms** |  **0.95** |
| Main   | True           | True         | 10000 | 1.902 ms | 0.0035 ms | 0.0031 ms |  1.00 |

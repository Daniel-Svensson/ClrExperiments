```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.22631.3155/23H2/2023Update/SunValley3)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 8.0.200
  [Host] : .NET 8.0.2 (8.0.224.6711), X86 RyuJIT AVX2

Job=InProcess  Toolchain=InProcessEmitToolchain  

```
| Method | RoundedAmounts | SmallDivisor | Count | Mean     | Error     | StdDev    | Ratio |
|------- |--------------- |------------- |------ |---------:|----------:|----------:|------:|
| **New**    | **True**           | **False**        | **10000** | **2.287 ms** | **0.0102 ms** | **0.0090 ms** |  **0.98** |
| Main   | True           | False        | 10000 | 2.345 ms | 0.0165 ms | 0.0146 ms |  1.00 |
|        |                |              |       |          |           |           |       |
| **New**    | **True**           | **True**         | **10000** | **1.936 ms** | **0.0171 ms** | **0.0151 ms** |  **0.99** |
| Main   | True           | True         | 10000 | 1.950 ms | 0.0118 ms | 0.0110 ms |  1.00 |

```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.22631.3155/23H2/2023Update/SunValley3)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 8.0.200
  [Host] : .NET 8.0.2 (8.0.224.6711), X64 RyuJIT AVX2

Job=InProcess  Toolchain=InProcessEmitToolchain  

```
| Method | RoundedAmounts | SmallDivisor | Count | Mean       | Error   | StdDev  | Ratio |
|------- |--------------- |------------- |------ |-----------:|--------:|--------:|------:|
| **New**    | **True**           | **False**        | **10000** | **1,096.6 μs** | **5.91 μs** | **5.53 μs** |  **0.84** |
| Main   | True           | False        | 10000 | 1,300.7 μs | 6.65 μs | 5.90 μs |  1.00 |
|        |                |              |       |            |         |         |       |
| **New**    | **True**           | **True**         | **10000** |   **992.8 μs** | **3.21 μs** | **3.00 μs** |  **0.92** |
| Main   | True           | True         | 10000 | 1,081.0 μs | 8.09 μs | 7.56 μs |  1.00 |

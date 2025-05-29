```

BenchmarkDotNet v0.14.0, Windows 11 (10.0.26100.4061)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ZLVWIY : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2

Toolchain=CoreRun  

```
| Method          | Mean     | Error   | StdDev  | Ratio |
|---------------- |---------:|--------:|--------:|------:|
| MultiplyNoFlags | 213.6 ns | 0.86 ns | 0.76 ns |  1.01 |
| BigMul          | 211.8 ns | 0.56 ns | 0.43 ns |  1.00 |
| BigMul_OLD      | 428.9 ns | 1.18 ns | 0.99 ns |  2.02 |
| BigMul2_OldPR   | 212.3 ns | 0.25 ns | 0.22 ns |  1.00 |
| Old             | 212.5 ns | 1.25 ns | 1.17 ns |  1.00 |

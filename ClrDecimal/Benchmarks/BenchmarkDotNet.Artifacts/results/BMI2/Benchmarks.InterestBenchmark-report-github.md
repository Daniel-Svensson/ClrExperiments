```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.26100.3915)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ILISAM : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-VNNWBJ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-LYYNZW : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method                 | Job        | Toolchain                                                                                  | RoundedAmounts | SmallDivisor | Count  | Mean      | Error     | StdDev    | Ratio | RatioSD |
|----------------------- |----------- |------------------------------------------------------------------------------------------- |--------------- |------------- |------- |----------:|----------:|----------:|------:|--------:|
| **&#39;System.Decimal .NET8&#39;** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **False**          | **False**        | **100000** | **12.093 ms** | **0.0581 ms** | **0.0544 ms** |  **0.94** |    **0.00** |
| &#39;System.Decimal .NET8&#39; | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | False          | False        | 100000 | 11.990 ms | 0.0583 ms | 0.0487 ms |  0.93 |    0.00 |
| &#39;System.Decimal .NET8&#39; | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | False          | False        | 100000 | 12.852 ms | 0.0382 ms | 0.0357 ms |  1.00 |    0.00 |
|                        |            |                                                                                            |                |              |        |           |           |           |       |         |
| **&#39;System.Decimal .NET8&#39;** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **False**          | **True**         | **100000** | **11.952 ms** | **0.2353 ms** | **0.3800 ms** |  **0.98** |    **0.04** |
| &#39;System.Decimal .NET8&#39; | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | False          | True         | 100000 | 11.932 ms | 0.2307 ms | 0.3158 ms |  0.98 |    0.04 |
| &#39;System.Decimal .NET8&#39; | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | False          | True         | 100000 | 12.228 ms | 0.2372 ms | 0.3000 ms |  1.00 |    0.00 |
|                        |            |                                                                                            |                |              |        |           |           |           |       |         |
| **&#39;System.Decimal .NET8&#39;** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **True**           | **False**        | **100000** | **10.860 ms** | **0.1849 ms** | **0.2130 ms** |  **1.00** |    **0.02** |
| &#39;System.Decimal .NET8&#39; | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | True           | False        | 100000 | 10.629 ms | 0.0726 ms | 0.0679 ms |  0.97 |    0.01 |
| &#39;System.Decimal .NET8&#39; | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | True           | False        | 100000 | 10.930 ms | 0.0276 ms | 0.0244 ms |  1.00 |    0.00 |
|                        |            |                                                                                            |                |              |        |           |           |           |       |         |
| **&#39;System.Decimal .NET8&#39;** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **True**           | **True**         | **100000** |  **9.606 ms** | **0.0649 ms** | **0.0607 ms** |  **0.98** |    **0.00** |
| &#39;System.Decimal .NET8&#39; | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | True           | True         | 100000 |  9.653 ms | 0.0756 ms | 0.0707 ms |  0.99 |    0.01 |
| &#39;System.Decimal .NET8&#39; | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | True           | True         | 100000 |  9.760 ms | 0.0853 ms | 0.0756 ms |  1.00 |    0.00 |

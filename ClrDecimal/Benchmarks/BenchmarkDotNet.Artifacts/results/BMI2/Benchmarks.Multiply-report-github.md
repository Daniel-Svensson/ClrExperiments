```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.26100.3915)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ILISAM : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-VNNWBJ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-LYYNZW : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method         | Job        | Toolchain                                                                                  | a                    | b                    | descr         | Mean     | Error    | StdDev   | Ratio |
|--------------- |----------- |------------------------------------------------------------------------------------------- |--------------------- |--------------------- |-------------- |---------:|---------:|---------:|------:|
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **15564.65093**          | **0.00000003**           | **32bit * 32bit** | **10.84 ns** | **0.057 ns** | **0.050 ns** |  **1.00** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 15564.65093          | 0.00000003           | 32bit * 32bit | 10.83 ns | 0.024 ns | 0.021 ns |  1.00 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 15564.65093          | 0.00000003           | 32bit * 32bit | 10.80 ns | 0.025 ns | 0.024 ns |  1.00 |
|                |            |                                                                                            |                      |                      |               |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **18446(...)19935 [21]** | **38738(...)51235 [22]** | **96bit * 96bit** | **32.96 ns** | **0.137 ns** | **0.115 ns** |  **0.99** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 18446(...)19935 [21] | 38738(...)51235 [22] | 96bit * 96bit | 32.59 ns | 0.070 ns | 0.065 ns |  0.98 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 18446(...)19935 [21] | 38738(...)51235 [22] | 96bit * 96bit | 33.38 ns | 0.129 ns | 0.120 ns |  1.00 |
|                |            |                                                                                            |                      |                      |               |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **21702051861934.75013** | **51240935.53816662**    | **64it * 64bit**  | **24.72 ns** | **0.030 ns** | **0.023 ns** |  **1.00** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 21702051861934.75013 | 51240935.53816662    | 64it * 64bit  | 24.99 ns | 0.063 ns | 0.052 ns |  1.01 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 21702051861934.75013 | 51240935.53816662    | 64it * 64bit  | 24.70 ns | 0.030 ns | 0.025 ns |  1.00 |
|                |            |                                                                                            |                      |                      |               |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **57510(...)29861 [21]** | **0.01193046**           | **96bit * 32bit** | **12.58 ns** | **0.062 ns** | **0.058 ns** |  **0.97** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 57510(...)29861 [21] | 0.01193046           | 96bit * 32bit | 12.84 ns | 0.028 ns | 0.024 ns |  0.99 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 57510(...)29861 [21] | 0.01193046           | 96bit * 32bit | 12.94 ns | 0.015 ns | 0.013 ns |  1.00 |

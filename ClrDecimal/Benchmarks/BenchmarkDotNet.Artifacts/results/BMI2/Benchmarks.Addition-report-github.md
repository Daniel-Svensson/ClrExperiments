```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.26100.3915)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ILISAM : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-VNNWBJ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-LYYNZW : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method         | Job        | Toolchain                                                                                  | a                    | b                    | descr                | Mean     | Error    | StdDev   | Ratio |
|--------------- |----------- |------------------------------------------------------------------------------------------- |--------------------- |--------------------- |--------------------- |---------:|---------:|---------:|------:|
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **0.333(...)33333 [30]** | **18446744073709551615** | **1/3 + (2^64-1)**       | **42.22 ns** | **0.255 ns** | **0.226 ns** |  **0.97** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 0.333(...)33333 [30] | 18446744073709551615 | 1/3 + (2^64-1)       | 42.92 ns | 0.417 ns | 0.325 ns |  0.99 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 0.333(...)33333 [30] | 18446744073709551615 | 1/3 + (2^64-1)       | 43.36 ns | 0.232 ns | 0.205 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                      |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **4294967.296**          | **4294967.296**          | **2^32 (...)scale [22]** | **10.90 ns** | **0.025 ns** | **0.022 ns** |  **1.00** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 4294967.296          | 4294967.296          | 2^32 (...)scale [22] | 11.16 ns | 0.075 ns | 0.067 ns |  1.02 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 4294967.296          | 4294967.296          | 2^32 (...)scale [22] | 10.89 ns | 0.042 ns | 0.037 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                      |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **79228(...)871.9 [30]** | **79228(...)033.5 [30]** | **subtract**             | **16.67 ns** | **0.018 ns** | **0.017 ns** |  **0.99** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 79228(...)871.9 [30] | 79228(...)033.5 [30] | subtract             | 16.87 ns | 0.031 ns | 0.029 ns |  1.00 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 79228(...)871.9 [30] | 79228(...)033.5 [30] | subtract             | 16.89 ns | 0.030 ns | 0.028 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                      |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **79228(...)033.5 [30]** | **79228(...)033.5 [30]** | **with carry**           | **16.94 ns** | **0.037 ns** | **0.035 ns** |  **1.01** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 79228(...)033.5 [30] | 79228(...)033.5 [30] | with carry           | 16.98 ns | 0.070 ns | 0.062 ns |  1.02 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 79228(...)033.5 [30] | 79228(...)033.5 [30] | with carry           | 16.70 ns | 0.028 ns | 0.024 ns |  1.00 |

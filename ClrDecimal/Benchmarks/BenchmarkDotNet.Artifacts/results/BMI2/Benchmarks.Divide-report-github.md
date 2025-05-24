```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.26100.3915)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ILISAM : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-VNNWBJ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-LYYNZW : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method         | Job        | Toolchain                                                                                  | a                    | b                    | descr              | Mean     | Error    | StdDev   | Ratio |
|--------------- |----------- |------------------------------------------------------------------------------------------- |--------------------- |--------------------- |------------------- |---------:|---------:|---------:|------:|
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **1**                    | **3**                    | **1/3 32bit division** | **33.61 ns** | **0.122 ns** | **0.114 ns** |  **1.00** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 1                    | 3                    | 1/3 32bit division | 33.19 ns | 0.094 ns | 0.088 ns |  0.99 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 1                    | 3                    | 1/3 32bit division | 33.49 ns | 0.129 ns | 0.108 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                    |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **107374182.39**         | **3.3**                  | **34bit / 32bit**      | **29.99 ns** | **0.126 ns** | **0.118 ns** |  **1.01** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 107374182.39         | 3.3                  | 34bit / 32bit      | 29.69 ns | 0.128 ns | 0.120 ns |  1.00 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 107374182.39         | 3.3                  | 34bit / 32bit      | 29.59 ns | 0.181 ns | 0.169 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                    |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **10145(...)50239 [21]** | **3**                    | **96bit / 32bit**      | **27.75 ns** | **0.051 ns** | **0.045 ns** |  **0.96** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 10145(...)50239 [21] | 3                    | 96bit / 32bit      | 27.61 ns | 0.087 ns | 0.081 ns |  0.95 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 10145(...)50239 [21] | 3                    | 96bit / 32bit      | 28.98 ns | 0.129 ns | 0.114 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                    |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **39291(...)21183 [22]** | **12884901920**          | **96bit / 64bit**      | **35.03 ns** | **0.151 ns** | **0.134 ns** |  **1.07** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 39291(...)21183 [22] | 12884901920          | 96bit / 64bit      | 33.19 ns | 0.107 ns | 0.100 ns |  1.01 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 39291(...)21183 [22] | 12884901920          | 96bit / 64bit      | 32.89 ns | 0.124 ns | 0.116 ns |  1.00 |
|                |            |                                                                                            |                      |                      |                    |          |          |          |       |
| **System_Decimal** | **Job-ILISAM** | **\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **39291(...)21183 [22]** | **18446744211148505120** | **96bit / 96bit**      | **50.26 ns** | **0.244 ns** | **0.216 ns** |  **1.06** |
| System_Decimal | Job-VNNWBJ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 39291(...)21183 [22] | 18446744211148505120 | 96bit / 96bit      | 46.16 ns | 0.060 ns | 0.056 ns |  0.97 |
| System_Decimal | Job-LYYNZW | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 39291(...)21183 [22] | 18446744211148505120 | 96bit / 96bit      | 47.47 ns | 0.114 ns | 0.101 ns |  1.00 |

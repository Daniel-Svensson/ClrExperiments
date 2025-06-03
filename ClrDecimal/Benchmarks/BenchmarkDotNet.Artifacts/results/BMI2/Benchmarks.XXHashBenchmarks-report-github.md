```

BenchmarkDotNet v0.14.0, Windows 11 (10.0.26100.4202)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ZAWDHL : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-RAEHQD : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method     | Job        | Toolchain                                                                         | Count   | Mean           | Error       | StdDev      | Ratio | RatioSD |
|----------- |----------- |---------------------------------------------------------------------------------- |-------- |---------------:|------------:|------------:|------:|--------:|
| **XXH32_Hash** | **Job-ZAWDHL** | **\net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe**      | **2**       |       **2.997 ns** |   **0.0131 ns** |   **0.0122 ns** |  **0.64** |    **0.00** |
| XXH64_Hash | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 2       |       5.124 ns |   0.0429 ns |   0.0401 ns |  1.10 |    0.01 |
| XXH3_Hash  | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 2       |       2.134 ns |   0.0079 ns |   0.0074 ns |  0.46 |    0.00 |
| XXH32_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 2       |       4.647 ns |   0.0159 ns |   0.0141 ns |  1.00 |    0.00 |
| XXH64_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 2       |       7.205 ns |   0.0816 ns |   0.0724 ns |  1.55 |    0.02 |
| XXH3_Hash  | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 2       |       3.546 ns |   0.0217 ns |   0.0203 ns |  0.76 |    0.00 |
|            |            |                                                                                   |         |                |             |             |       |         |
| **XXH32_Hash** | **Job-ZAWDHL** | **\net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe**      | **32**      |       **5.467 ns** |   **0.0527 ns** |   **0.0493 ns** |  **0.73** |    **0.01** |
| XXH64_Hash | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 32      |       9.398 ns |   0.0449 ns |   0.0398 ns |  1.26 |    0.01 |
| XXH3_Hash  | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 32      |       3.722 ns |   0.0132 ns |   0.0124 ns |  0.50 |    0.00 |
| XXH32_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 32      |       7.460 ns |   0.0383 ns |   0.0340 ns |  1.00 |    0.01 |
| XXH64_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 32      |      10.411 ns |   0.0371 ns |   0.0310 ns |  1.40 |    0.01 |
| XXH3_Hash  | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 32      |       4.371 ns |   0.0180 ns |   0.0141 ns |  0.59 |    0.00 |
|            |            |                                                                                   |         |                |             |             |       |         |
| **XXH32_Hash** | **Job-ZAWDHL** | **\net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe**      | **1048576** | **113,028.898 ns** | **285.5290 ns** | **238.4296 ns** |  **1.00** |    **0.00** |
| XXH64_Hash | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 1048576 |  65,586.858 ns | 362.6197 ns | 339.1947 ns |  0.58 |    0.00 |
| XXH3_Hash  | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 1048576 |  20,423.861 ns | 101.2838 ns |  84.5766 ns |  0.18 |    0.00 |
| XXH32_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 1048576 | 112,920.736 ns | 375.6223 ns | 313.6617 ns |  1.00 |    0.00 |
| XXH64_Hash | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 1048576 |  66,036.614 ns | 531.4152 ns | 471.0857 ns |  0.58 |    0.00 |
| XXH3_Hash  | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 1048576 |  20,555.481 ns |  91.6056 ns |  81.2059 ns |  0.18 |    0.00 |

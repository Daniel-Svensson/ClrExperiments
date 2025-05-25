```

BenchmarkDotNet v0.14.0, Windows 11 (10.0.26100.4061)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-UZCGFT : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-CDBMRH : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-SLUNWZ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method     | Job        | Toolchain                                                                                  | Count   | Mean           | Error         | StdDev        | Ratio | RatioSD |
|----------- |----------- |------------------------------------------------------------------------------------------- |-------- |---------------:|--------------:|--------------:|------:|--------:|
| **XXH32_Hash** | **Job-UZCGFT** | **\x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **2**       |       **4.506 ns** |     **0.0554 ns** |     **0.0518 ns** |  **0.99** |    **0.01** |
| XXH32_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 2       |       4.439 ns |     0.0038 ns |     0.0032 ns |  0.98 |    0.00 |
| XXH32_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 2       |       4.532 ns |     0.0122 ns |     0.0108 ns |  1.00 |    0.00 |
| XXH64_Hash | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 2       |       6.765 ns |     0.0264 ns |     0.0247 ns |  1.49 |    0.01 |
| XXH64_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 2       |       6.749 ns |     0.0214 ns |     0.0200 ns |  1.49 |    0.01 |
| XXH64_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 2       |       6.892 ns |     0.0189 ns |     0.0177 ns |  1.52 |    0.01 |
| XXH3_Hash  | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 2       |       3.391 ns |     0.0458 ns |     0.0428 ns |  0.75 |    0.01 |
| XXH3_Hash  | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 2       |       3.415 ns |     0.0311 ns |     0.0291 ns |  0.75 |    0.01 |
| XXH3_Hash  | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 2       |       3.450 ns |     0.0175 ns |     0.0155 ns |  0.76 |    0.00 |
|            |            |                                                                                            |         |                |               |               |       |         |
| **XXH32_Hash** | **Job-UZCGFT** | **\x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **32**      |       **7.303 ns** |     **0.0208 ns** |     **0.0185 ns** |  **1.01** |    **0.02** |
| XXH32_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 32      |       7.393 ns |     0.1118 ns |     0.0933 ns |  1.02 |    0.02 |
| XXH32_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 32      |       7.263 ns |     0.1267 ns |     0.1185 ns |  1.00 |    0.02 |
| XXH64_Hash | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 32      |      10.195 ns |     0.0276 ns |     0.0258 ns |  1.40 |    0.02 |
| XXH64_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 32      |      10.164 ns |     0.0267 ns |     0.0208 ns |  1.40 |    0.02 |
| XXH64_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 32      |      10.417 ns |     0.0439 ns |     0.0389 ns |  1.43 |    0.02 |
| XXH3_Hash  | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 32      |       3.685 ns |     0.0218 ns |     0.0182 ns |  0.51 |    0.01 |
| XXH3_Hash  | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 32      |       3.907 ns |     0.0446 ns |     0.0417 ns |  0.54 |    0.01 |
| XXH3_Hash  | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 32      |       4.367 ns |     0.0622 ns |     0.0551 ns |  0.60 |    0.01 |
|            |            |                                                                                            |         |                |               |               |       |         |
| **XXH32_Hash** | **Job-UZCGFT** | **\x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe** | **1048576** | **112,637.759 ns** | **1,998.0098 ns** | **1,868.9396 ns** |  **1.00** |    **0.02** |
| XXH32_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 1048576 | 111,333.138 ns |   623.0366 ns |   520.2637 ns |  0.98 |    0.01 |
| XXH32_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 1048576 | 113,210.760 ns | 1,452.7533 ns | 1,358.9064 ns |  1.00 |    0.02 |
| XXH64_Hash | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 1048576 |  64,450.041 ns |   438.1069 ns |   388.3704 ns |  0.57 |    0.01 |
| XXH64_Hash | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 1048576 |  63,751.124 ns |   685.1016 ns |   640.8444 ns |  0.56 |    0.01 |
| XXH64_Hash | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 1048576 |  63,781.161 ns |   806.3251 ns |   754.2370 ns |  0.56 |    0.01 |
| XXH3_Hash  | Job-UZCGFT | \x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 1048576 |  19,753.274 ns |    67.6017 ns |    59.9272 ns |  0.17 |    0.00 |
| XXH3_Hash  | Job-CDBMRH | \x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 1048576 |  20,026.098 ns |   133.5598 ns |   118.3973 ns |  0.18 |    0.00 |
| XXH3_Hash  | Job-SLUNWZ | \x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 1048576 |  20,193.862 ns |   383.1404 ns |   456.1015 ns |  0.18 |    0.00 |

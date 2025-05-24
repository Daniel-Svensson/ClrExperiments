```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.26100.3915)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-JCYSGS : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-YXAHPB : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-SJSTKO : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2

EnvironmentVariables=DOTNET_EnableBMI2=0  

```
| Method                    | Job        | Toolchain                                                                                  | TestA             | TestB                | Mean      | Error     | StdDev    | Ratio | RatioSD |
|-------------------------- |----------- |------------------------------------------------------------------------------------------- |------------------ |--------------------- |----------:|----------:|----------:|------:|--------:|
| BenchBigMulUnsigned       | Job-JCYSGS | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 |  1.283 ns | 0.0104 ns | 0.0092 ns |  0.10 |    0.00 |
| BenchBigMulUnsigned       | Job-YXAHPB | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 | 12.231 ns | 0.0136 ns | 0.0121 ns |  1.00 |    0.00 |
| BenchBigMulUnsigned       | Job-SJSTKO | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 | 12.256 ns | 0.0133 ns | 0.0118 ns |  1.00 |    0.00 |
|                           |            |                                                                                            |                   |                      |           |           |           |       |         |
| BenchBigMulSigned         | Job-JCYSGS | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 |  1.275 ns | 0.0051 ns | 0.0048 ns |  0.12 |    0.00 |
| BenchBigMulSigned         | Job-YXAHPB | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 | 10.753 ns | 0.0607 ns | 0.0538 ns |  1.00 |    0.01 |
| BenchBigMulSigned         | Job-SJSTKO | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 | 10.783 ns | 0.0743 ns | 0.0620 ns |  1.00 |    0.00 |
|                           |            |                                                                                            |                   |                      |           |           |           |       |         |
| BenchMultiplyNoFlags3Ards | Job-JCYSGS | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 |        NA |        NA |        NA |     ? |       ? |
| BenchMultiplyNoFlags3Ards | Job-YXAHPB | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 |        NA |        NA |        NA |     ? |       ? |
| BenchMultiplyNoFlags3Ards | Job-SJSTKO | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 |        NA |        NA |        NA |     ? |       ? |

Benchmarks with issues:
  BigMulTests.BenchMultiplyNoFlags3Ards: Job-JCYSGS(EnvironmentVariables=DOTNET_EnableBMI2=0, Toolchain=\net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe) [TestA=81985529216486895, TestB=16045690984833335023]
  BigMulTests.BenchMultiplyNoFlags3Ards: Job-YXAHPB(EnvironmentVariables=DOTNET_EnableBMI2=0, Toolchain=\net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe) [TestA=81985529216486895, TestB=16045690984833335023]
  BigMulTests.BenchMultiplyNoFlags3Ards: Job-SJSTKO(EnvironmentVariables=DOTNET_EnableBMI2=0, Toolchain=\net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe) [TestA=81985529216486895, TestB=16045690984833335023]

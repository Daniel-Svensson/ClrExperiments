```

BenchmarkDotNet v0.14.0, Windows 11 (10.0.26100.4061)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-SZTYJW : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-LFLJNQ : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-IIXCDT : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method                    | Job        | Toolchain                                                                                  | TestA             | TestB                | Mean     | Error     | StdDev    | Ratio |
|-------------------------- |----------- |------------------------------------------------------------------------------------------- |------------------ |--------------------- |---------:|----------:|----------:|------:|
| BenchBigMulUnsigned       | Job-SZTYJW | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 1.277 ns | 0.0168 ns | 0.0157 ns |  0.86 |
| BenchBigMulUnsigned       | Job-LFLJNQ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 | 1.397 ns | 0.0030 ns | 0.0028 ns |  0.94 |
| BenchBigMulUnsigned       | Job-IIXCDT | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 | 1.490 ns | 0.0060 ns | 0.0057 ns |  1.00 |
|                           |            |                                                                                            |                   |                      |          |           |           |       |
| BenchBigMulSigned         | Job-SZTYJW | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 1.259 ns | 0.0023 ns | 0.0020 ns |  0.43 |
| BenchBigMulSigned         | Job-LFLJNQ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 | 3.382 ns | 0.0037 ns | 0.0033 ns |  1.15 |
| BenchBigMulSigned         | Job-IIXCDT | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 | 2.943 ns | 0.0040 ns | 0.0033 ns |  1.00 |
|                           |            |                                                                                            |                   |                      |          |           |           |       |
| BenchMultiplyNoFlags3Ards | Job-SZTYJW | \net10.0-windows-Release-x64_MathInstrinct\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 3.284 ns | 0.0049 ns | 0.0045 ns |  1.00 |
| BenchMultiplyNoFlags3Ards | Job-LFLJNQ | \net10.0-windows-Release-x64_bigmul\shared\Microsoft.NETCore.App\10.0.0\corerun.exe        | 81985529216486895 | 16045690984833335023 | 3.282 ns | 0.0059 ns | 0.0055 ns |  1.00 |
| BenchMultiplyNoFlags3Ards | Job-IIXCDT | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe          | 81985529216486895 | 16045690984833335023 | 3.292 ns | 0.0141 ns | 0.0118 ns |  1.00 |

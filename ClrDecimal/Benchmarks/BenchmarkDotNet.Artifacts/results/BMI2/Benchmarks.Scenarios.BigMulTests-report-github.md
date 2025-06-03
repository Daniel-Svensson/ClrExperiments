```

BenchmarkDotNet v0.14.0, Windows 11 (10.0.26100.4202)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 10.0.100-preview.3.25201.16
  [Host]     : .NET 10.0.0 (10.0.25.17105), X64 RyuJIT AVX2
  Job-ZAWDHL : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2
  Job-RAEHQD : .NET 10.0.0 (42.42.42.42424), X64 RyuJIT AVX2


```
| Method                    | Job        | Toolchain                                                                         | TestA             | TestB                | Mean      | Error     | StdDev    | Ratio |
|-------------------------- |----------- |---------------------------------------------------------------------------------- |------------------ |--------------------- |----------:|----------:|----------:|------:|
| BenchBigMulUnsigned       | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 81985529216486895 | 16045690984833335023 | 0.6420 ns | 0.0065 ns | 0.0061 ns |  0.42 |
| BenchBigMulUnsigned       | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 1.5354 ns | 0.0141 ns | 0.0125 ns |  1.00 |
|                           |            |                                                                                   |                   |                      |           |           |           |       |
| BenchBigMulSigned         | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 81985529216486895 | 16045690984833335023 | 1.2853 ns | 0.0070 ns | 0.0065 ns |  0.43 |
| BenchBigMulSigned         | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 2.9852 ns | 0.0263 ns | 0.0246 ns |  1.00 |
|                           |            |                                                                                   |                   |                      |           |           |           |       |
| BenchMultiplyNoFlags3Ards | Job-ZAWDHL | \net10.0-windows-Release-x64\shared\Microsoft.NETCore.App\10.0.0\corerun.exe      | 81985529216486895 | 16045690984833335023 | 3.3122 ns | 0.0060 ns | 0.0056 ns |  1.00 |
| BenchMultiplyNoFlags3Ards | Job-RAEHQD | \net10.0-windows-Release-x64_main\shared\Microsoft.NETCore.App\10.0.0\corerun.exe | 81985529216486895 | 16045690984833335023 | 3.3080 ns | 0.0058 ns | 0.0054 ns |  1.00 |

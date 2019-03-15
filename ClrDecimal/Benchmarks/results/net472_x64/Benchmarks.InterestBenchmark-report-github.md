``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
  [Host] : .NET Framework 4.7.2 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.3362.0

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|       Method | RoundedAmounts | SmallDivisor |  Count |     Mean |     Error |    StdDev |   Median |
|------------- |--------------- |------------- |------- |---------:|----------:|----------:|---------:|
| **NetFramework** |          **False** |        **False** | **100000** | **66.61 ms** | **0.4049 ms** | **0.5935 ms** | **66.74 ms** |
|          New |          False |        False | 100000 | 34.54 ms | 0.4598 ms | 0.6293 ms | 35.10 ms |
|    Oleauto32 |          False |        False | 100000 | 73.60 ms | 0.3111 ms | 0.4656 ms | 73.40 ms |
| PInvokeDummy |          False |        False | 100000 | 10.53 ms | 0.2141 ms | 0.3205 ms | 10.53 ms |
| **NetFramework** |          **False** |         **True** | **100000** | **64.17 ms** | **0.0127 ms** | **0.0182 ms** | **64.18 ms** |
|          New |          False |         True | 100000 | 33.42 ms | 0.1800 ms | 0.2694 ms | 33.35 ms |
|    Oleauto32 |          False |         True | 100000 | 71.96 ms | 0.0719 ms | 0.1076 ms | 71.96 ms |
| PInvokeDummy |          False |         True | 100000 | 10.60 ms | 0.0934 ms | 0.1340 ms | 10.61 ms |
| **NetFramework** |           **True** |        **False** | **100000** | **51.28 ms** | **0.4324 ms** | **0.6339 ms** | **50.86 ms** |
|          New |           True |        False | 100000 | 30.35 ms | 0.3327 ms | 0.4772 ms | 30.01 ms |
|    Oleauto32 |           True |        False | 100000 | 58.96 ms | 0.4103 ms | 0.6140 ms | 58.59 ms |
| PInvokeDummy |           True |        False | 100000 | 10.44 ms | 0.0773 ms | 0.1158 ms | 10.36 ms |
| **NetFramework** |           **True** |         **True** | **100000** | **49.91 ms** | **0.3660 ms** | **0.5249 ms** | **50.08 ms** |
|          New |           True |         True | 100000 | 29.80 ms | 0.2570 ms | 0.3341 ms | 29.81 ms |
|    Oleauto32 |           True |         True | 100000 | 56.63 ms | 0.2252 ms | 0.3230 ms | 56.54 ms |
| PInvokeDummy |           True |         True | 100000 | 10.30 ms | 0.0046 ms | 0.0069 ms | 10.29 ms |

``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
  [Host] : .NET Framework 4.7.2 (CLR 4.0.30319.42000), 32bit LegacyJIT-v4.7.3362.0

Job=ShortRun  Toolchain=InProcessToolchain  IterationCount=3  
LaunchCount=1  WarmupCount=3  

```
|       Method | RoundedAmounts | SmallDivisor |  Count |     Mean |      Error |    StdDev |
|------------- |--------------- |------------- |------- |---------:|-----------:|----------:|
| **NetFramework** |          **False** |        **False** | **100000** | **37.88 ms** |  **4.4420 ms** | **0.2435 ms** |
|          New |          False |        False | 100000 | 39.30 ms |  0.2075 ms | 0.0114 ms |
|    Oleauto32 |          False |        False | 100000 | 36.01 ms |  0.1841 ms | 0.0101 ms |
| PInvokeDummy |          False |        False | 100000 | 11.10 ms |  0.0939 ms | 0.0051 ms |
| **NetFramework** |          **False** |         **True** | **100000** | **35.33 ms** |  **6.0594 ms** | **0.3321 ms** |
|          New |          False |         True | 100000 | 36.73 ms |  0.0554 ms | 0.0030 ms |
|    Oleauto32 |          False |         True | 100000 | 34.39 ms |  0.5789 ms | 0.0317 ms |
| PInvokeDummy |          False |         True | 100000 | 11.06 ms |  0.0855 ms | 0.0047 ms |
| **NetFramework** |           **True** |        **False** | **100000** | **34.72 ms** |  **1.3349 ms** | **0.0732 ms** |
|          New |           True |        False | 100000 | 36.37 ms |  8.9721 ms | 0.4918 ms |
|    Oleauto32 |           True |        False | 100000 | 32.88 ms |  3.9143 ms | 0.2146 ms |
| PInvokeDummy |           True |        False | 100000 | 11.26 ms |  3.5308 ms | 0.1935 ms |
| **NetFramework** |           **True** |         **True** | **100000** | **32.06 ms** |  **7.2960 ms** | **0.3999 ms** |
|          New |           True |         True | 100000 | 34.09 ms | 15.2008 ms | 0.8332 ms |
|    Oleauto32 |           True |         True | 100000 | 31.34 ms |  1.5900 ms | 0.0872 ms |
| PInvokeDummy |           True |         True | 100000 | 11.30 ms |  2.8155 ms | 0.1543 ms |

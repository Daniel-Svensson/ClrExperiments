``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27414.05, CoreFX 4.6.27414.05), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|       Method | RoundedAmounts | SmallDivisor |  Count |      Mean |     Error |    StdDev |    Median |
|------------- |--------------- |------------- |------- |----------:|----------:|----------:|----------:|
| **NetFramework** |          **False** |        **False** | **100000** | **38.041 ms** | **0.2255 ms** | **0.3305 ms** | **38.072 ms** |
|          New |          False |        False | 100000 | 34.074 ms | 0.1268 ms | 0.1858 ms | 34.040 ms |
|    Oleauto32 |          False |        False | 100000 | 30.787 ms | 0.1140 ms | 0.1635 ms | 30.746 ms |
| PInvokeDummy |          False |        False | 100000 |  7.844 ms | 0.1474 ms | 0.2206 ms |  7.876 ms |
| **NetFramework** |          **False** |         **True** | **100000** | **35.373 ms** | **0.2204 ms** | **0.3090 ms** | **35.327 ms** |
|          New |          False |         True | 100000 | 31.584 ms | 0.3056 ms | 0.4480 ms | 31.350 ms |
|    Oleauto32 |          False |         True | 100000 | 29.285 ms | 0.2590 ms | 0.3631 ms | 29.084 ms |
| PInvokeDummy |          False |         True | 100000 |  7.682 ms | 0.1436 ms | 0.2060 ms |  7.802 ms |
| **NetFramework** |           **True** |        **False** | **100000** | **34.526 ms** | **0.1504 ms** | **0.2205 ms** | **34.426 ms** |
|          New |           True |        False | 100000 | 31.182 ms | 0.1767 ms | 0.2645 ms | 31.185 ms |
|    Oleauto32 |           True |        False | 100000 | 28.217 ms | 0.4699 ms | 0.6739 ms | 27.829 ms |
| PInvokeDummy |           True |        False | 100000 |  7.450 ms | 0.0202 ms | 0.0289 ms |  7.444 ms |
| **NetFramework** |           **True** |         **True** | **100000** | **31.612 ms** | **0.2084 ms** | **0.3119 ms** | **31.511 ms** |
|          New |           True |         True | 100000 | 29.316 ms | 0.1713 ms | 0.2286 ms | 29.147 ms |
|    Oleauto32 |           True |         True | 100000 | 25.446 ms | 0.0799 ms | 0.1094 ms | 25.390 ms |
| PInvokeDummy |           True |         True | 100000 |  7.506 ms | 0.0530 ms | 0.0776 ms |  7.470 ms |

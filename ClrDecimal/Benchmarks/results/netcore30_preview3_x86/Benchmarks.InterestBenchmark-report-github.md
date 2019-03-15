``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|       Method | RoundedAmounts | SmallDivisor |  Count |       Mean |      Error |     StdDev |    Median |
|------------- |--------------- |------------- |------- |-----------:|-----------:|-----------:|----------:|
| **NetFramework** |          **False** |        **False** | **100000** |  **54.085 ms** |  **0.0435 ms** |  **0.0596 ms** | **54.069 ms** |
|          New |          False |        False | 100000 |  33.603 ms |  0.1051 ms |  0.1403 ms | 33.600 ms |
|    Oleauto32 |          False |        False | 100000 |  30.684 ms |  0.3060 ms |  0.4580 ms | 30.715 ms |
|      CoreRT2 |          False |        False | 100000 |  97.817 ms |  0.7911 ms |  1.1346 ms | 97.375 ms |
| PInvokeDummy |          False |        False | 100000 |   7.166 ms |  0.0698 ms |  0.1045 ms |  7.142 ms |
| **NetFramework** |          **False** |         **True** | **100000** |  **49.277 ms** |  **0.0239 ms** |  **0.0327 ms** | **49.280 ms** |
|          New |          False |         True | 100000 |  30.808 ms |  0.0764 ms |  0.1046 ms | 30.784 ms |
|    Oleauto32 |          False |         True | 100000 |  29.138 ms |  0.1933 ms |  0.2893 ms | 29.222 ms |
|      CoreRT2 |          False |         True | 100000 | 102.079 ms | 12.3149 ms | 18.0510 ms | 91.517 ms |
| PInvokeDummy |          False |         True | 100000 |   7.676 ms |  0.6466 ms |  0.9274 ms |  7.304 ms |
| **NetFramework** |           **True** |        **False** | **100000** |  **42.283 ms** |  **1.1856 ms** |  **1.5416 ms** | **42.096 ms** |
|          New |           True |        False | 100000 |  30.637 ms |  0.2565 ms |  0.3840 ms | 30.420 ms |
|    Oleauto32 |           True |        False | 100000 |  27.102 ms |  0.2114 ms |  0.3032 ms | 27.005 ms |
|      CoreRT2 |           True |        False | 100000 |  77.937 ms |  0.3342 ms |  0.4685 ms | 77.802 ms |
| PInvokeDummy |           True |        False | 100000 |   7.252 ms |  0.0837 ms |  0.1174 ms |  7.258 ms |
| **NetFramework** |           **True** |         **True** | **100000** |  **36.457 ms** |  **0.2679 ms** |  **0.4011 ms** | **36.165 ms** |
|          New |           True |         True | 100000 |  28.210 ms |  0.2035 ms |  0.3047 ms | 28.287 ms |
|    Oleauto32 |           True |         True | 100000 |  25.387 ms |  0.0239 ms |  0.0335 ms | 25.384 ms |
|      CoreRT2 |           True |         True | 100000 |  70.428 ms |  0.5660 ms |  0.8472 ms | 69.956 ms |
| PInvokeDummy |           True |         True | 100000 |   7.298 ms |  0.0738 ms |  0.1059 ms |  7.246 ms |

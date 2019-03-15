``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 64bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|       Method | RoundedAmounts | SmallDivisor |  Count |      Mean |     Error |    StdDev |    Median |
|------------- |--------------- |------------- |------- |----------:|----------:|----------:|----------:|
| **NetFramework** |          **False** |        **False** | **100000** | **27.492 ms** | **0.2849 ms** | **0.4265 ms** | **27.427 ms** |
|          New |          False |        False | 100000 | 30.286 ms | 0.0932 ms | 0.1395 ms | 30.292 ms |
|    Oleauto32 |          False |        False | 100000 | 70.111 ms | 0.5651 ms | 0.8104 ms | 69.829 ms |
| PInvokeDummy |          False |        False | 100000 |  8.198 ms | 0.1529 ms | 0.2041 ms |  8.386 ms |
| **NetFramework** |          **False** |         **True** | **100000** | **24.883 ms** | **0.1217 ms** | **0.1706 ms** | **24.844 ms** |
|          New |          False |         True | 100000 | 29.341 ms | 0.1253 ms | 0.1837 ms | 29.213 ms |
|    Oleauto32 |          False |         True | 100000 | 67.786 ms | 0.0743 ms | 0.1017 ms | 67.792 ms |
| PInvokeDummy |          False |         True | 100000 |  8.417 ms | 0.0781 ms | 0.1169 ms |  8.418 ms |
| **NetFramework** |           **True** |        **False** | **100000** | **24.369 ms** | **0.2518 ms** | **0.3691 ms** | **24.413 ms** |
|          New |           True |        False | 100000 | 25.831 ms | 0.0914 ms | 0.1340 ms | 25.837 ms |
|    Oleauto32 |           True |        False | 100000 | 54.780 ms | 0.0493 ms | 0.0691 ms | 54.723 ms |
| PInvokeDummy |           True |        False | 100000 |  8.128 ms | 0.0917 ms | 0.1344 ms |  8.250 ms |
| **NetFramework** |           **True** |         **True** | **100000** | **22.122 ms** | **0.1361 ms** | **0.2037 ms** | **22.094 ms** |
|          New |           True |         True | 100000 | 25.331 ms | 0.1107 ms | 0.1552 ms | 25.350 ms |
|    Oleauto32 |           True |         True | 100000 | 53.235 ms | 0.1984 ms | 0.2909 ms | 53.109 ms |
| PInvokeDummy |           True |         True | 100000 |  8.825 ms | 0.3228 ms | 0.4629 ms |  9.243 ms |

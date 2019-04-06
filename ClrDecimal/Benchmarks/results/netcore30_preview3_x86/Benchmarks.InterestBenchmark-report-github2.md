``` ini

BenchmarkDotNet=v0.11.5, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|         Method | RoundedAmounts | SmallDivisor |  Count |     Mean |     Error |    StdDev |   Median |
|--------------- |--------------- |------------- |------- |---------:|----------:|----------:|---------:|
| **System.Decimal** |          **False** |        **False** | **100000** | **54.07 ms** | **0.3373 ms** | **0.5049 ms** | **53.78 ms** |
|P/Invoke New C++|          False |        False | 100000 | 33.80 ms | 0.0457 ms | 0.0641 ms | 33.79 ms |
|P/Invoke oleauto|          False |        False | 100000 | 30.49 ms | 0.0904 ms | 0.1267 ms | 30.58 ms |
| **System.Decimal** |          **False** |         **True** | **100000** | **49.33 ms** | **0.2189 ms** | **0.3140 ms** | **49.37 ms** |
|P/Invoke New C++|          False |         True | 100000 | 31.04 ms | 0.0990 ms | 0.1420 ms | 31.04 ms |
|P/Invoke oleauto|          False |         True | 100000 | 29.06 ms | 0.0940 ms | 0.1287 ms | 29.16 ms |
| **System.Decimal** |           **True** |        **False** | **100000** | **40.23 ms** | **0.4698 ms** | **0.6585 ms** | **39.64 ms** |
|P/Invoke New C++|           True |        False | 100000 | 30.47 ms | 0.1168 ms | 0.1637 ms | 30.41 ms |
|P/Invoke oleauto|           True |        False | 100000 | 27.28 ms | 0.1704 ms | 0.2498 ms | 27.17 ms |
| **System.Decimal** |           **True** |         **True** | **100000** | **35.35 ms** | **0.1019 ms** | **0.1395 ms** | **35.42 ms** |
|P/Invoke New C++|           True |         True | 100000 | 27.64 ms | 0.0270 ms | 0.0360 ms | 27.63 ms |
|P/Invoke oleauto|           True |         True | 100000 | 25.48 ms | 0.0328 ms | 0.0449 ms | 25.50 ms |

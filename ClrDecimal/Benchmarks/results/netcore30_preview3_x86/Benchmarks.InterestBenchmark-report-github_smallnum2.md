``` ini

BenchmarkDotNet=v0.11.5, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|               Method | RoundedAmounts | SmallDivisor |  Count |     Mean |     Error |    StdDev |   Median |
|--------------------- |--------------- |------------- |------- |---------:|----------:|----------:|---------:|
|       **System.Decimal** |          **False** |        **False** | **100000** | **50.44 ms** | **0.4111 ms** | **0.5763 ms** | **50.13 ms** |
|   &#39;P/Invoke New C++&#39; |          False |        False | 100000 | 36.76 ms | 0.0232 ms | 0.0309 ms | 36.77 ms |
| &#39;P/Invoke oleauto32&#39; |          False |        False | 100000 | 30.32 ms | 0.1128 ms | 0.1653 ms | 30.24 ms |
|       **System.Decimal** |          **False** |         **True** | **100000** | **45.56 ms** | **0.1850 ms** | **0.2712 ms** | **45.74 ms** |
|   &#39;P/Invoke New C++&#39; |          False |         True | 100000 | 34.41 ms | 0.1553 ms | 0.2276 ms | 34.41 ms |
| &#39;P/Invoke oleauto32&#39; |          False |         True | 100000 | 29.67 ms | 0.5067 ms | 0.7427 ms | 30.00 ms |
|       **System.Decimal** |           **True** |        **False** | **100000** | **37.42 ms** | **0.4805 ms** | **0.6736 ms** | **36.85 ms** |
|   &#39;P/Invoke New C++&#39; |           True |        False | 100000 | 32.89 ms | 0.2697 ms | 0.4037 ms | 32.77 ms |
| &#39;P/Invoke oleauto32&#39; |           True |        False | 100000 | 26.19 ms | 0.0530 ms | 0.0743 ms | 26.20 ms |
|       **System.Decimal** |           **True** |         **True** | **100000** | **32.62 ms** | **0.2810 ms** | **0.4206 ms** | **32.37 ms** |
|   &#39;P/Invoke New C++&#39; |           True |         True | 100000 | 29.92 ms | 0.2354 ms | 0.3301 ms | 29.86 ms |
| &#39;P/Invoke oleauto32&#39; |           True |         True | 100000 | 24.48 ms | 0.1400 ms | 0.2008 ms | 24.50 ms |

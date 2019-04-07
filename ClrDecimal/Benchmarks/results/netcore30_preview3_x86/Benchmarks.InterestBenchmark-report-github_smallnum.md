``` ini

BenchmarkDotNet=v0.11.5, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=ShortRun  Toolchain=InProcessToolchain  IterationCount=3  
LaunchCount=1  WarmupCount=3  

```
|               Method | RoundedAmounts | SmallDivisor |  Count |     Mean |      Error |    StdDev |
|--------------------- |--------------- |------------- |------- |---------:|-----------:|----------:|
|       **System.Decimal** |          **False** |        **False** | **100000** | **50.42 ms** | **11.8249 ms** | **0.6482 ms** |
|   &#39;P/Invoke New C++&#39; |          False |        False | 100000 | 33.27 ms |  8.6233 ms | 0.4727 ms |
| &#39;P/Invoke oleauto32&#39; |          False |        False | 100000 | 30.25 ms |  0.9500 ms | 0.0521 ms |
|       **System.Decimal** |          **False** |         **True** | **100000** | **46.80 ms** | **13.5972 ms** | **0.7453 ms** |
|   &#39;P/Invoke New C++&#39; |          False |         True | 100000 | 30.32 ms |  0.1360 ms | 0.0075 ms |
| &#39;P/Invoke oleauto32&#39; |          False |         True | 100000 | 28.78 ms |  1.9610 ms | 0.1075 ms |
|       **System.Decimal** |           **True** |        **False** | **100000** | **37.30 ms** | **10.4275 ms** | **0.5716 ms** |
|   &#39;P/Invoke New C++&#39; |           True |        False | 100000 | 29.47 ms |  0.1789 ms | 0.0098 ms |
| &#39;P/Invoke oleauto32&#39; |           True |        False | 100000 | 26.36 ms |  6.1196 ms | 0.3354 ms |
|       **System.Decimal** |           **True** |         **True** | **100000** | **32.52 ms** |  **5.5799 ms** | **0.3059 ms** |
|   &#39;P/Invoke New C++&#39; |           True |         True | 100000 | 26.54 ms |  0.1219 ms | 0.0067 ms |
| &#39;P/Invoke oleauto32&#39; |           True |         True | 100000 | 24.07 ms |  0.0689 ms | 0.0038 ms |

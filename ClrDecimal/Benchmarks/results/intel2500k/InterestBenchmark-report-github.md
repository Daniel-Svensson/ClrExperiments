``` ini

BenchmarkDotNet=v0.10.9, OS=Windows 10 Redstone 2 (10.0.15063)
Processor=Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), ProcessorCount=4
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
  [Host]    : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0
  MediumRun : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0

Job=MediumRun  LaunchCount=2  TargetCount=15  
WarmupCount=10  TotalIssues/Op=0  LlcReference/Op=0  
LlcMisses/Op=0  

```
 |       Method | RoundedAmounts | SmallDivisor |  Count |      Mean |     Error |    StdDev |    Median |     Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | CacheMisses/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |------------- |--------------- |------------- |------- |----------:|----------:|----------:|----------:|----------:|----------:|----------------:|----------------------:|---------------:|------------------------:|----------------------------:|
 | **NetFramework** |          **False** |        **False** | **100000** | **67.416 ms** | **0.5024 ms** | **0.7043 ms** | **67.807 ms** |         **-** |       **0 B** |          **1,56 %** |               **2016438** |           **1828** |                   **31488** |                     **2016457** |
 |          New |          False |        False | 100000 | 31.021 ms | 0.1548 ms | 0.2317 ms | 30.881 ms |         - |       0 B |          0,96 % |              30332311 |          21527 |                  292398 |                    30332322 |
 |    Oleauto32 |          False |        False | 100000 | 70.724 ms | 0.3487 ms | 0.5219 ms | 70.818 ms |         - |       0 B |          1,43 % |               8597367 |           7048 |                  123084 |                     8597367 |
 |      CoreRT2 |          False |        False | 100000 | 78.494 ms | 0.2234 ms | 0.3343 ms | 78.581 ms | 1250.0000 | 4000336 B |          0,86 % |               3922505 |           1700 |                   33664 |                     3922523 |
 | PInvokeDummy |          False |        False | 100000 |  8.041 ms | 0.0560 ms | 0.0838 ms |  7.989 ms |         - |       0 B |          0,05 % |                428781 |            740 |                     228 |                      428781 |
 | **NetFramework** |          **False** |         **True** | **100000** | **65.520 ms** | **0.3141 ms** | **0.4505 ms** | **65.661 ms** |         **-** |       **0 B** |          **1,77 %** |               **2833920** |           **2638** |                   **50274** |                     **2833920** |
 |          New |          False |         True | 100000 | 30.621 ms | 0.1792 ms | 0.2683 ms | 30.498 ms |         - |       0 B |          1,03 % |               2379864 |           2097 |                   24615 |                     2379864 |
 |    Oleauto32 |          False |         True | 100000 | 68.303 ms | 0.1904 ms | 0.2606 ms | 68.299 ms |         - |       0 B |          1,68 % |                966198 |            896 |                   16201 |                      966198 |
 |      CoreRT2 |          False |         True | 100000 | 76.673 ms | 0.2460 ms | 0.3682 ms | 76.562 ms | 1250.0000 | 4000336 B |             ¤¤¤ |                     0 |              0 |                       0 |                           0 |
 | PInvokeDummy |          False |         True | 100000 |  8.357 ms | 0.2744 ms | 0.3846 ms |  8.110 ms |         - |       0 B |          0,07 % |                533344 |           1098 |                     352 |                      533344 |
 | **NetFramework** |           **True** |        **False** | **100000** | **51.639 ms** | **0.1973 ms** | **0.2633 ms** | **51.726 ms** |         **-** |       **0 B** |          **1,65 %** |                **644516** |            **493** |                   **10605** |                      **644516** |
 |          New |           True |        False | 100000 | 26.708 ms | 0.0853 ms | 0.1223 ms | 26.799 ms |         - |       0 B |          0,99 % |               2016992 |           1397 |                   19882 |                     2016992 |
 |    Oleauto32 |           True |        False | 100000 | 54.721 ms | 0.1312 ms | 0.1752 ms | 54.849 ms |         - |       0 B |          1,56 % |              21103872 |          16503 |                  328874 |                    21103889 |
 |      CoreRT2 |           True |        False | 100000 | 63.147 ms | 0.1592 ms | 0.2333 ms | 63.055 ms | 1250.0000 | 4000336 B |             ¤¤¤ |                     0 |              0 |                       0 |                           0 |
 | PInvokeDummy |           True |        False | 100000 |  8.465 ms | 0.3276 ms | 0.4593 ms |  8.771 ms |         - |       0 B |          0,05 % |                490427 |            772 |                     265 |                      490427 |
 | **NetFramework** |           **True** |         **True** | **100000** | **50.072 ms** | **0.1668 ms** | **0.2338 ms** | **49.949 ms** |         **-** |       **0 B** |          **1,85 %** |               **3934936** |           **3544** |                   **72763** |                     **3934956** |
 |          New |           True |         True | 100000 | 26.379 ms | 0.1194 ms | 0.1712 ms | 26.351 ms |         - |       0 B |          1,04 % |              13053431 |           8891 |                  136379 |                    13053440 |
 |    Oleauto32 |           True |         True | 100000 | 52.925 ms | 0.3433 ms | 0.4924 ms | 52.938 ms |         - |       0 B |          1,78 % |              28827446 |          25362 |                  513554 |                    28827465 |
 |      CoreRT2 |           True |         True | 100000 | 61.034 ms | 0.2117 ms | 0.3168 ms | 60.823 ms | 1250.0000 | 4000336 B |          1,06 % |               3950957 |           2048 |                   42020 |                     3950957 |
 | PInvokeDummy |           True |         True | 100000 |  9.081 ms | 0.2184 ms | 0.3062 ms |  8.891 ms |         - |       0 B |          0,05 % |                452845 |            708 |                     246 |                      452841 |

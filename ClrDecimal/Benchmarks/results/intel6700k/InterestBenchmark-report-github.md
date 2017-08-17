``` ini

BenchmarkDotNet=v0.10.9, OS=Windows 10 Redstone 2 (10.0.15063)
Processor=Intel Core i7-6700K CPU 4.00GHz (Skylake), ProcessorCount=8
Frequency=3914067 Hz, Resolution=255.4887 ns, Timer=TSC
  [Host]    : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0
  MediumRun : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0

Job=MediumRun  LaunchCount=2  TargetCount=15  
WarmupCount=10  TotalIssues/Op=0  CacheMisses/Op=0  
LlcReference/Op=0  LlcMisses/Op=0  

```
 |       Method | RoundedAmounts | SmallDivisor |  Count |     Mean |     Error |    StdDev |   Median |    Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |------------- |--------------- |------------- |------- |---------:|----------:|----------:|---------:|---------:|----------:|----------------:|----------------------:|------------------------:|----------------------------:|
 | **NetFramework** |          **False** |        **False** | **100000** | **60.75 ms** | **0.2337 ms** | **0.3426 ms** | **60.84 ms** |        **-** |       **0 B** |          **1,58 %** |              **35037269** |                  **555264** |                    **35037760** |
 |          New |          False |        False | 100000 | 27.05 ms | 0.0087 ms | 0.0119 ms | 27.05 ms |        - |       0 B |          0,96 % |              28666594 |                  274313 |                    28666692 |
 |    Oleauto32 |          False |        False | 100000 | 64.03 ms | 0.5310 ms | 0.7616 ms | 63.36 ms |        - |       0 B |          1,47 % |              36113685 |                  531776 |                    36114176 |
 |      CoreRT2 |          False |        False | 100000 | 67.59 ms | 0.0389 ms | 0.0558 ms | 67.57 ms | 937.5000 | 4000316 B |          0,93 % |              79038869 |                  735210 |                    79040170 |
 | **NetFramework** |          **False** |         **True** | **100000** | **60.50 ms** | **1.0556 ms** | **1.5140 ms** | **59.66 ms** |        **-** |       **0 B** |          **1,92 %** |              **26500114** |                  **508489** |                    **26500681** |
 |          New |          False |         True | 100000 | 27.31 ms | 0.1311 ms | 0.1880 ms | 27.25 ms |        - |       0 B |          1,02 % |              31098880 |                  318229 |                    31099488 |
 |    Oleauto32 |          False |         True | 100000 | 62.43 ms | 0.0597 ms | 0.0856 ms | 62.40 ms |        - |       0 B |          1,76 % |              25939899 |                  456072 |                    25940377 |
 |      CoreRT2 |          False |         True | 100000 | 66.44 ms | 0.0635 ms | 0.0848 ms | 66.41 ms | 937.5000 | 4000316 B |          1,01 % |              58329139 |                  587520 |                    58330675 |
 | **NetFramework** |           **True** |        **False** | **100000** | **46.49 ms** | **0.0909 ms** | **0.1274 ms** | **46.43 ms** |        **-** |       **0 B** |          **1,71 %** |              **29982555** |                  **513206** |                    **29983323** |
 |          New |           True |        False | 100000 | 22.90 ms | 0.1698 ms | 0.2435 ms | 22.71 ms |        - |       0 B |          1,00 % |              25436589 |                  254208 |                    25437074 |
 |    Oleauto32 |           True |        False | 100000 | 49.77 ms | 0.0300 ms | 0.0430 ms | 49.76 ms |        - |       0 B |          1,63 % |              30691858 |                  500406 |                    30692608 |
 |      CoreRT2 |           True |        False | 100000 | 54.10 ms | 0.0276 ms | 0.0405 ms | 54.08 ms | 937.5000 | 4000316 B |          1,03 % |              62849554 |                  648777 |                    62850925 |
 | **NetFramework** |           **True** |         **True** | **100000** | **45.39 ms** | **0.3404 ms** | **0.4659 ms** | **45.39 ms** |        **-** |       **0 B** |          **1,98 %** |              **28385260** |                  **563377** |                    **28385988** |
 |          New |           True |         True | 100000 | 23.11 ms | 0.1598 ms | 0.2292 ms | 23.31 ms |        - |       0 B |          1,07 % |              23710071 |                  254225 |                    23710634 |
 |    Oleauto32 |           True |         True | 100000 | 49.52 ms | 0.7477 ms | 0.9982 ms | 50.45 ms |        - |       0 B |          1,86 % |              25773585 |                  480375 |                    25774131 |
 |      CoreRT2 |           True |         True | 100000 | 52.61 ms | 0.0305 ms | 0.0437 ms | 52.60 ms | 937.5000 | 4000316 B |          1,08 % |              53829051 |                  583594 |                    53829614 |

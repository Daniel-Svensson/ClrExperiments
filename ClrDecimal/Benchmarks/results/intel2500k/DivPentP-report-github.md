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
 |          Method |       Mean |     Error |    StdDev | Scaled |     Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | CacheMisses/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |-----------:|----------:|----------:|-------:|----------:|----------:|----------------:|----------------------:|---------------:|------------------------:|----------------------------:|
 |    NetFramework | 134.276 ms | 0.6122 ms | 0.8380 ms |   1.26 | 1500.0000 | 4771004 B |          1,44 % |              11844693 |           5312 |                  170154 |                    11844714 |
 |      PInvokeNew |  94.432 ms | 0.5844 ms | 0.8381 ms |   0.89 |  875.0000 | 2830968 B |          1,19 % |               7922907 |           2523 |                   93915 |                     7922907 |
 |    PInvokePalRT | 105.207 ms | 0.1540 ms | 0.2055 ms |   0.99 |  875.0000 | 2830968 B |             ¤¤¤ |                     0 |              0 |                       0 |                           0 |
 |    PInvokeOle32 | 106.411 ms | 0.3877 ms | 0.5436 ms |   1.00 |  875.0000 | 2830968 B |          1,39 % |               7892004 |           2505 |                  109842 |                     7892004 |
 | CoreCRTManaged2 | 115.345 ms | 0.9821 ms | 1.3768 ms |   1.08 |  875.0000 | 2830968 B |          1,32 % |              64325956 |          18773 |                  848725 |                    64325973 |
 |    PInvokeDummy |   4.404 ms | 0.0012 ms | 0.0016 ms |   0.04 |         - |       0 B |          0,05 % |               1506329 |            224 |                     710 |                     1506329 |

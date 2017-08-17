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
 |          Method |       Mean |     Error |    StdDev | Scaled | ScaledSD |     Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | CacheMisses/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |-----------:|----------:|----------:|-------:|---------:|----------:|----------:|----------------:|----------------------:|---------------:|------------------------:|----------------------------:|
 |    NetFramework | 135.181 ms | 0.4360 ms | 0.5669 ms |   1.34 |     0.02 | 1750.0000 | 5694690 B |          1,04 % |              47856913 |          22067 |                  497425 |                    47856947 |
 |      PInvokeNew |  99.043 ms | 0.5673 ms | 0.8135 ms |   0.98 |     0.01 | 1062.5000 | 3375282 B |          0,85 % |               8407040 |           2944 |                   71661 |                     8407040 |
 |    PInvokePalRT | 101.335 ms | 0.4869 ms | 0.7137 ms |   1.00 |     0.01 | 1062.5000 | 3375282 B |          0,95 % |             164659626 |          61440 |                 1566776 |                   164659712 |
 |    PInvokeOle32 | 101.150 ms | 0.8227 ms | 1.2314 ms |   1.00 |     0.00 | 1062.5000 | 3375282 B |          0,92 % |               8417517 |           2852 |                   77385 |                     8417517 |
 | CoreCRTManaged2 | 105.002 ms | 0.2617 ms | 0.3669 ms |   1.04 |     0.01 | 1062.5000 | 3375282 B |          0,93 % |              34699997 |          11537 |                  323054 |                    34700014 |
 |    PInvokeDummy |   4.322 ms | 0.0191 ms | 0.0261 ms |   0.04 |     0.00 |         - |       0 B |          0,05 % |               2278557 |            388 |                    1224 |                     2278560 |

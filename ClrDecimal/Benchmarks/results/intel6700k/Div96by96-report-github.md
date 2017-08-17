``` ini

BenchmarkDotNet=v0.10.9, OS=Windows 10 Redstone 2 (10.0.15063)
Processor=Intel Core i7-6700K CPU 4.00GHz (Skylake), ProcessorCount=8
Frequency=3914067 Hz, Resolution=255.4887 ns, Timer=TSC
  [Host]   : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0
  ShortRun : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0

Job=ShortRun  LaunchCount=1  TargetCount=3  
WarmupCount=3  TotalIssues/Op=0  CacheMisses/Op=0  
BranchMispredictions/Op=0  LlcReference/Op=0  LlcMisses/Op=0  

```
 |          Method |      Mean |     Error |    StdDev | Scaled |  Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|-------:|-------:|----------:|----------------:|----------------------:|----------------------------:|
 |    NetFramework | 181.47 ns | 0.3781 ns | 0.0214 ns |   0.95 |      - |       0 B |          0,11 % |                   121 |                         121 |
 |          Native |  87.05 ns | 1.4461 ns | 0.0817 ns |   0.45 |      - |       0 B |          0,07 % |                    94 |                          94 |
 |           PalRT | 196.17 ns | 7.5681 ns | 0.4276 ns |   1.02 |      - |       0 B |          0,10 % |                   113 |                         113 |
 |           Ole32 | 191.97 ns | 2.0178 ns | 0.1140 ns |   1.00 |      - |       0 B |          0,09 % |                   112 |                         112 |
 |  CoreCRTManaged | 188.13 ns | 3.3974 ns | 0.1920 ns |   0.98 | 0.0379 |     160 B |          0,06 % |                   277 |                         277 |
 | CoreCRTManaged2 | 175.47 ns | 0.1174 ns | 0.0066 ns |   0.91 |      - |       0 B |          0,05 % |                   230 |                         230 |

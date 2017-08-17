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
 |          Method |      Mean |     Error |    StdDev |    Median | Scaled |     Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|----------:|-------:|----------:|----------:|----------------:|----------------------:|------------------------:|----------------------------:|
 |    NetFramework | 91.196 ms | 0.2843 ms | 0.4255 ms | 91.222 ms |   1.23 | 1125.0000 | 4770950 B |          0,94 % |             137261243 |                 1292202 |                   137262677 |
 |      PInvokeNew | 63.550 ms | 0.4617 ms | 0.6622 ms | 63.068 ms |   0.86 |  625.0000 | 2830948 B |          0,79 % |             123542823 |                  973056 |                   123544024 |
 |    PInvokePalRT | 74.090 ms | 0.2558 ms | 0.3750 ms | 73.925 ms |   1.00 |  625.0000 | 2830948 B |          1,00 % |             114282404 |                 1144795 |                   114283392 |
 |    PInvokeOle32 | 74.316 ms | 0.2580 ms | 0.3861 ms | 74.306 ms |   1.00 |  625.0000 | 2830948 B |          1,03 % |             106934016 |                 1096857 |                   106934988 |
 | CoreCRTManaged2 | 80.115 ms | 0.0689 ms | 0.1010 ms | 80.122 ms |   1.08 |  625.0000 | 2830948 B |          0,95 % |             134825764 |                 1276032 |                   134827026 |
 |    PInvokeDummy |  3.822 ms | 0.0034 ms | 0.0051 ms |  3.819 ms |   0.05 |         - |       0 B |          0,11 % |               3490573 |                    3714 |                     3490583 |

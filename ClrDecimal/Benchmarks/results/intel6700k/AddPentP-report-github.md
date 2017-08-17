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
 |          Method |      Mean |     Error |    StdDev |    Median | Scaled |     Gen 0 |  Allocated | Mispredict rate | BranchInstructions/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|----------:|-------:|----------:|-----------:|----------------:|----------------------:|------------------------:|----------------------------:|
 |    NetFramework |  6.443 ms | 0.0048 ms | 0.0072 ms |  6.441 ms |   0.74 |         - |        0 B |          5,21 % |               7506557 |                  390981 |                     7506685 |
 |    PInvokeDummy |  3.917 ms | 0.0200 ms | 0.0299 ms |  3.921 ms |   0.45 |         - |        0 B |          0,13 % |               3861517 |                    5012 |                     3861542 |
 |      PInvokeNew |  6.158 ms | 0.0114 ms | 0.0167 ms |  6.164 ms |   0.71 |         - |        0 B |          2,48 % |               7436045 |                  184400 |                     7436226 |
 |    PInvokePalRT |  8.811 ms | 0.0526 ms | 0.0737 ms |  8.872 ms |   1.01 |         - |        0 B |          3,47 % |               8569942 |                  297316 |                     8570034 |
 |    PInvokeOle32 |  8.706 ms | 0.0042 ms | 0.0060 ms |  8.705 ms |   1.00 |         - |        0 B |          3,49 % |               8134446 |                  283554 |                     8134660 |
 |  CoreCRTManaged | 11.868 ms | 0.0542 ms | 0.0795 ms | 11.843 ms |   1.36 | 3312.5000 | 13897830 B |          2,05 % |              15208546 |                  311692 |                    15208819 |
 | CoreCRTManaged2 | 13.579 ms | 0.0058 ms | 0.0082 ms | 13.576 ms |   1.56 |         - |        0 B |          1,56 % |              21509769 |                  335515 |                    21510176 |

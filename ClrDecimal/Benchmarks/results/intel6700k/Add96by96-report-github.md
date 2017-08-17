``` ini

BenchmarkDotNet=v0.10.9, OS=Windows 10 Redstone 2 (10.0.15063)
Processor=Intel Core i7-6700K CPU 4.00GHz (Skylake), ProcessorCount=8
Frequency=3914067 Hz, Resolution=255.4887 ns, Timer=TSC
  [Host]    : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0
  MediumRun : .NET Framework 4.7 (CLR 4.0.30319.42000), 64bit RyuJIT-v4.7.2102.0

Job=MediumRun  LaunchCount=2  TargetCount=15  
WarmupCount=10  TotalIssues/Op=0  CacheMisses/Op=0  
BranchMispredictions/Op=0  LlcReference/Op=0  LlcMisses/Op=0  

```
 |          Method |     Mean |     Error |    StdDev |   Median | Scaled |  Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchInstructionRetired/Op |
 |---------------- |---------:|----------:|----------:|---------:|-------:|-------:|----------:|----------------:|----------------------:|----------------------------:|
 |          Native | 23.99 ns | 0.0975 ns | 0.1460 ns | 23.98 ns |   0.94 |      - |       0 B |          0,09 % |                    31 |                          31 |
 |           PalRT | 26.57 ns | 0.0859 ns | 0.1259 ns | 26.48 ns |   1.04 |      - |       0 B |          0,11 % |                    31 |                          31 |
 |           Ole32 | 25.50 ns | 0.0095 ns | 0.0142 ns | 25.50 ns |   1.00 |      - |       0 B |          0,13 % |                    32 |                          32 |
 |  CoreCRTManaged | 50.07 ns | 0.0667 ns | 0.0999 ns | 50.05 ns |   1.96 | 0.0209 |      88 B |          0,03 % |                    83 |                          83 |
 | CoreCRTManaged2 | 53.85 ns | 0.0122 ns | 0.0182 ns | 53.85 ns |   2.11 |      - |       0 B |          0,08 % |                    89 |                          89 |

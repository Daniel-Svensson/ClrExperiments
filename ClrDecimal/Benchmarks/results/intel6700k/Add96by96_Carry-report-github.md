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
 |          Method |     Mean |     Error |    StdDev |   Median | Scaled | Allocated | Mispredict rate | BranchInstructions/Op | BranchInstructionRetired/Op |
 |---------------- |---------:|----------:|----------:|---------:|-------:|----------:|----------------:|----------------------:|----------------------------:|
 |    NetFramework | 36.48 ns | 0.0201 ns | 0.0301 ns | 36.50 ns |   0.75 |       0 B |          0,11 % |                    18 |                          18 |
 |          Native | 33.62 ns | 0.4267 ns | 0.6387 ns | 33.64 ns |   0.69 |       0 B |          0,10 % |                    37 |                          37 |
 |           PalRT | 48.74 ns | 0.0842 ns | 0.1180 ns | 48.85 ns |   1.00 |       0 B |          0,14 % |                    31 |                          31 |
 |           Ole32 | 48.62 ns | 0.0014 ns | 0.0019 ns | 48.62 ns |   1.00 |       0 B |          0,10 % |                    31 |                          31 |
 | CoreCRTManaged2 | 80.48 ns | 0.0337 ns | 0.0472 ns | 80.46 ns |   1.66 |       0 B |          0,09 % |                   107 |                         107 |

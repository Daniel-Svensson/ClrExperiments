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
 |          Method |      Mean |     Error |    StdDev | Scaled |  Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|-------:|-------:|----------:|----------------:|----------------------:|----------------------------:|
 |    NetFramework | 159.69 ns | 0.0144 ns | 0.0211 ns |   0.95 |      - |       0 B |          0,07 % |                    60 |                          60 |
 |          Native |  69.99 ns | 0.0348 ns | 0.0499 ns |   0.42 |      - |       0 B |          0,13 % |                    50 |                          50 |
 |           PalRT | 167.30 ns | 0.0773 ns | 0.1133 ns |   1.00 |      - |       0 B |          0,08 % |                    69 |                          69 |
 |           Ole32 | 167.27 ns | 0.0519 ns | 0.0710 ns |   1.00 |      - |       0 B |          0,07 % |                    68 |                          68 |
 |  CoreCRTManaged | 141.01 ns | 0.0663 ns | 0.0992 ns |   0.84 | 0.0112 |      48 B |          0,06 % |                   111 |                         111 |
 | CoreCRTManaged2 | 166.59 ns | 0.0928 ns | 0.1389 ns |   1.00 |      - |       0 B |          0,03 % |                   176 |                         176 |

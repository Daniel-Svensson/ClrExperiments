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
 |          Method |      Mean |     Error |    StdDev |    Median | Scaled | ScaledSD |     Gen 0 | Allocated | Mispredict rate | BranchInstructions/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|----------:|-------:|---------:|----------:|----------:|----------------:|----------------------:|------------------------:|----------------------------:|
 |    NetFramework | 85.096 ms | 0.2116 ms | 0.3035 ms | 85.067 ms |   1.31 |     0.01 | 1312.5000 | 5694628 B |          0,48 % |             148543829 |                  717175 |                   148545314 |
 |    PInvokeDummy |  3.935 ms | 0.0024 ms | 0.0035 ms |  3.934 ms |   0.06 |     0.00 |         - |       0 B |          0,07 % |               4165417 |                    2924 |                     4165432 |
 |      PInvokeNew | 62.970 ms | 0.1347 ms | 0.2017 ms | 63.012 ms |   0.97 |     0.01 |  750.0000 | 3375211 B |          0,38 % |             113250286 |                  434346 |                   113251140 |
 |    PInvokePalRT | 65.281 ms | 0.1295 ms | 0.1938 ms | 65.305 ms |   1.00 |     0.01 |  750.0000 | 3375211 B |          0,46 % |             113774165 |                  526950 |                   113775240 |
 |    PInvokeOle32 | 65.021 ms | 0.2240 ms | 0.3283 ms | 64.840 ms |   1.00 |     0.00 |  750.0000 | 3375211 B |          0,48 % |             113915648 |                  544716 |                   113916859 |
 | CoreCRTManaged2 | 70.021 ms | 0.6929 ms | 1.0371 ms | 70.123 ms |   1.08 |     0.02 |  750.0000 | 3375211 B |          0,49 % |             125412352 |                  611003 |                   125413444 |

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
 |          Method |      Mean |     Error |    StdDev |    Median | Scaled | ScaledSD |     Gen 0 |  Allocated | Mispredict rate | BranchInstructions/Op | CacheMisses/Op | BranchMispredictions/Op | BranchInstructionRetired/Op |
 |---------------- |----------:|----------:|----------:|----------:|-------:|---------:|----------:|-----------:|----------------:|----------------------:|---------------:|------------------------:|----------------------------:|
 |    NetFramework |  7.094 ms | 0.0040 ms | 0.0055 ms |  7.093 ms |   0.72 |     0.00 |         - |        0 B |          5,08 % |               1422389 |            530 |                   72282 |                     1422389 |
 |      PInvokeNew |  7.355 ms | 0.2430 ms | 0.3485 ms |  7.039 ms |   0.75 |     0.04 |         - |        0 B |          2,51 % |                505995 |            178 |                   12717 |                      505995 |
 |    PInvokePalRT |  9.830 ms | 0.0364 ms | 0.0521 ms |  9.795 ms |   1.00 |     0.01 |         - |        0 B |          3,49 % |                561056 |            219 |                   19579 |                      561056 |
 |    PInvokeOle32 |  9.799 ms | 0.0379 ms | 0.0543 ms |  9.788 ms |   1.00 |     0.00 |         - |        0 B |          3,56 % |                242816 |            290 |                    8649 |                      242820 |
 |  CoreCRTManaged | 15.648 ms | 0.1325 ms | 0.1983 ms | 15.601 ms |   1.60 |     0.02 | 4406.2500 | 13898032 B |          2,13 % |                870116 |           1005 |                   18505 |                      870116 |
 | CoreCRTManaged2 | 15.831 ms | 0.1046 ms | 0.1565 ms | 15.784 ms |   1.62 |     0.02 |         - |        0 B |          1,62 % |               3179697 |           1250 |                   51436 |                     3179697 |
 |    PInvokeDummy |  4.122 ms | 0.0065 ms | 0.0098 ms |  4.117 ms |   0.42 |     0.00 |         - |        0 B |          0,05 % |               1222105 |            174 |                     573 |                     1222105 |

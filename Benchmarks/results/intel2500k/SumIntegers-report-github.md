``` ini

BenchmarkDotNet=v0.10.1, OS=Microsoft Windows NT 6.2.9200.0
Processor=Intel(R) Core(TM) i5-2500K CPU 3.30GHz, ProcessorCount=4
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
  [Host]     : Clr 4.0.30319.42000, 64bit RyuJIT-v4.6.1586.0
  Job-QTIBDN : Clr 4.0.30319.42000, 64bit LegacyJIT/clrjit-v4.6.1586.0;compatjit-v4.6.1586.0
  Job-IDHBCW : Clr 4.0.30319.42000, 32bit LegacyJIT-v4.6.1586.0
  Job-FPOZGJ : Clr 4.0.30319.42000, 64bit RyuJIT-v4.6.1586.0

Runtime=Clr  IterationTime=1.0000 s  LaunchCount=1  
TargetCount=3  WarmupCount=3  

```
             Method |       Jit | Platform |        Mean |    StdDev | Scaled | Scaled-StdDev |    Gen 0 | Allocated |
------------------- |---------- |--------- |------------ |---------- |------- |-------------- |--------- |---------- |
     ForLoopSumImpl | LegacyJit |      X64 |   2.4823 us | 0.0049 us |   1.00 |          0.00 |        - |       0 B |
    GenericILMethod | LegacyJit |      X64 |   2.3826 us | 0.0056 us |   0.96 |          0.00 |        - |       0 B |
 GenericHelperClass | LegacyJit |      X64 |  19.0988 us | 0.0627 us |   7.69 |          0.02 |        - |       0 B |
     DynamicHelperC | LegacyJit |      X64 | 191.5216 us | 0.2228 us |  77.16 |          0.14 | 144.8171 |    480 kB |
        VectorBased | LegacyJit |      X64 |  66.4641 us | 0.0063 us |  26.78 |          0.04 |        - |      40 B |
     ForLoopSumImpl | LegacyJit |      X86 |   4.2910 us | 0.0042 us |   1.00 |          0.00 |        - |       0 B |
    GenericILMethod | LegacyJit |      X86 |   4.3001 us | 0.0013 us |   1.00 |          0.00 |        - |       0 B |
 GenericHelperClass | LegacyJit |      X86 |  14.3652 us | 0.0021 us |   3.35 |          0.00 |        - |       0 B |
     DynamicHelperC | LegacyJit |      X86 | 130.4330 us | 0.2160 us |  30.40 |          0.05 |  70.8506 |    240 kB |
        VectorBased | LegacyJit |      X86 | 105.3966 us | 0.0674 us |  24.56 |          0.02 |        - |      28 B |
     ForLoopSumImpl |    RyuJit |      X64 |   4.2240 us | 0.0042 us |   1.00 |          0.00 |        - |       0 B |
    GenericILMethod |    RyuJit |      X64 |   4.1970 us | 0.0001 us |   0.99 |          0.00 |        - |       0 B |
 GenericHelperClass |    RyuJit |      X64 |  19.0561 us | 0.0416 us |   4.51 |          0.01 |        - |       0 B |
     DynamicHelperC |    RyuJit |      X64 | 142.4387 us | 0.0921 us |  33.72 |          0.03 | 145.3101 |    480 kB |
        VectorBased |    RyuJit |      X64 |   4.4415 us | 0.0105 us |   1.05 |          0.00 |        - |      40 B |

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
                             Method |       Jit | Platform |          Mean |    StdDev | Scaled | Scaled-StdDev |     Gen 0 | Allocated |
----------------------------------- |---------- |--------- |-------------- |---------- |------- |-------------- |---------- |---------- |
                          NormalSum | LegacyJit |      X64 |    71.4205 us | 0.0015 us |   1.00 |          0.00 |         - |       0 B |
                          MethodSum | LegacyJit |      X64 |   214.2726 us | 0.1296 us |   3.00 |          0.00 |         - |       0 B |
                       MethodRefSum | LegacyJit |      X64 |   358.0521 us | 0.4716 us |   5.01 |          0.01 |         - |       0 B |
                         GenericSum | LegacyJit |      X64 |   238.4602 us | 0.4328 us |   3.34 |          0.00 |         - |       0 B |
                       MethodIncSum | LegacyJit |      X64 |   214.2432 us | 0.0899 us |   3.00 |          0.00 |         - |       0 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X64 |   238.0012 us | 0.0356 us |   3.33 |          0.00 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X64 |   519.4720 us | 1.7594 us |   7.27 |          0.02 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X64 |   519.5500 us | 1.1951 us |   7.27 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X64 |   238.1423 us | 0.1637 us |   3.33 |          0.00 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X64 |   214.8104 us | 0.3503 us |   3.01 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X64 | 1,958.2458 us | 4.1969 us |  27.42 |          0.05 | 1455.0781 |    4.8 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X64 |   357.7350 us | 0.4167 us |   5.01 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X64 |   238.1366 us | 0.1461 us |   3.33 |          0.00 |         - |       0 B |
                      VectorSummary | LegacyJit |      X64 |   580.6061 us | 0.3580 us |   8.13 |          0.00 |         - |       0 B |
                          NormalSum | LegacyJit |      X86 |    71.4857 us | 0.0513 us |   1.00 |          0.00 |         - |       0 B |
                          MethodSum | LegacyJit |      X86 |   238.6342 us | 0.5297 us |   3.34 |          0.01 |         - |       0 B |
                       MethodRefSum | LegacyJit |      X86 |   238.2972 us | 0.5498 us |   3.33 |          0.01 |         - |       0 B |
                         GenericSum | LegacyJit |      X86 |   238.5595 us | 0.1522 us |   3.34 |          0.00 |         - |       0 B |
                       MethodIncSum | LegacyJit |      X86 |   238.2641 us | 0.5284 us |   3.33 |          0.01 |         - |       0 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X86 |   238.3722 us | 0.2793 us |   3.33 |          0.00 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X86 |   550.3136 us | 0.0235 us |   7.70 |          0.00 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X86 |   565.2561 us | 1.5446 us |   7.91 |          0.02 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X86 |   238.4277 us | 0.0924 us |   3.34 |          0.00 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X86 |   238.1918 us | 0.0100 us |   3.33 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X86 | 1,361.7458 us | 4.4867 us |  19.05 |          0.05 |  712.5000 |    2.4 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X86 |   238.3112 us | 0.0234 us |   3.33 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X86 |   238.5530 us | 0.1514 us |   3.34 |          0.00 |         - |       0 B |
                      VectorSummary | LegacyJit |      X86 |   696.2857 us | 0.1231 us |   9.74 |          0.01 |         - |       0 B |
                          NormalSum |    RyuJit |      X64 |    71.4501 us | 0.0261 us |   1.00 |          0.00 |         - |       0 B |
                          MethodSum |    RyuJit |      X64 |    71.5223 us | 0.1429 us |   1.00 |          0.00 |         - |       0 B |
                       MethodRefSum |    RyuJit |      X64 |    71.5344 us | 0.0139 us |   1.00 |          0.00 |         - |       0 B |
                         GenericSum |    RyuJit |      X64 |   214.4548 us | 0.4746 us |   3.00 |          0.01 |         - |       0 B |
                       MethodIncSum |    RyuJit |      X64 |    71.5472 us | 0.0187 us |   1.00 |          0.00 |         - |       0 B |
        Generic_Sum_Scalar_Proposal |    RyuJit |      X64 |   214.9129 us | 0.5377 us |   3.01 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast |    RyuJit |      X64 |   459.0193 us | 1.3961 us |   6.42 |          0.02 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast |    RyuJit |      X64 |   461.2875 us | 0.0154 us |   6.46 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Member |    RyuJit |      X64 |   238.5641 us | 0.5706 us |   3.34 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method |    RyuJit |      X64 |    71.8590 us | 0.2066 us |   1.01 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic |    RyuJit |      X64 | 1,465.4534 us | 7.8208 us |  20.51 |          0.09 | 1472.5379 |    4.8 MB |
           Generic_Sum_ScalarHelper |    RyuJit |      X64 |    71.5958 us | 0.2881 us |   1.00 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static |    RyuJit |      X64 |   214.2056 us | 0.0400 us |   3.00 |          0.00 |         - |       0 B |
                      VectorSummary |    RyuJit |      X64 |    18.9587 us | 0.0040 us |   0.27 |          0.00 |         - |       0 B |

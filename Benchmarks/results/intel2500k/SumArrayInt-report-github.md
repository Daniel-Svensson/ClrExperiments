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
                             Method |       Jit | Platform |          Mean |     StdDev | Scaled | Scaled-StdDev |     Gen 0 | Allocated |
----------------------------------- |---------- |--------- |-------------- |----------- |------- |-------------- |---------- |---------- |
                     ForLoopSumImpl | LegacyJit |      X64 |    47.9822 us |  0.2565 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X64 |   500.9599 us |  1.0165 us |  10.44 |          0.05 |         - |      32 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X64 |   237.9465 us |  0.0120 us |   4.96 |          0.02 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X64 |   301.5669 us |  0.0601 us |   6.29 |          0.03 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X64 |   315.3790 us |  0.0030 us |   6.57 |          0.03 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X64 |   237.9499 us |  0.0092 us |   4.96 |          0.02 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X64 |    47.6515 us |  0.0054 us |   0.99 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X64 | 1,991.0906 us | 19.6368 us |  41.50 |          0.38 | 1449.8698 |    4.8 MB |
     Generic_Sum_ScalarClass_Static | LegacyJit |      X64 |    54.0455 us |  0.0055 us |   1.13 |          0.00 |         - |       0 B |
   Generic_Sum_ScalarClass_Instance | LegacyJit |      X64 |    54.9773 us |  0.3658 us |   1.15 |          0.01 |         - |       0 B |
           Generic_Sum_ScalarHelper | LegacyJit |      X64 |    47.8449 us |  0.3145 us |   1.00 |          0.01 |         - |       0 B |
          Generic_Sum_Scalar_Helper | LegacyJit |      X64 |   215.6252 us |  1.3006 us |   4.49 |          0.03 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X64 |   238.7468 us |  0.7900 us |   4.98 |          0.03 |         - |       0 B |
                      VectorSummary | LegacyJit |      X64 |   581.4810 us |  1.3310 us |  12.12 |          0.06 |         - |       0 B |
                     ForLoopSumImpl | LegacyJit |      X86 |    47.8161 us |  0.1069 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X86 |   452.2991 us |  1.7632 us |   9.46 |          0.03 |         - |      22 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X86 |   166.5818 us |  0.0109 us |   3.48 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X86 |   166.7781 us |  0.3627 us |   3.49 |          0.01 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X86 |   167.2282 us |  0.0367 us |   3.50 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X86 |   168.4009 us |  0.3508 us |   3.52 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X86 |    47.8609 us |  0.0299 us |   1.00 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X86 | 1,396.9615 us |  3.3170 us |  29.22 |          0.08 |  709.6920 |    2.4 MB |
     Generic_Sum_ScalarClass_Static | LegacyJit |      X86 |    64.0510 us |  0.1194 us |   1.34 |          0.00 |         - |       0 B |
   Generic_Sum_ScalarClass_Instance | LegacyJit |      X86 |    75.8813 us |  0.1209 us |   1.59 |          0.00 |         - |       0 B |
           Generic_Sum_ScalarHelper | LegacyJit |      X86 |    47.6966 us |  0.0048 us |   1.00 |          0.00 |         - |       0 B |
          Generic_Sum_Scalar_Helper | LegacyJit |      X86 |   166.5843 us |  0.0171 us |   3.48 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X86 |   166.9871 us |  0.6960 us |   3.49 |          0.01 |         - |       0 B |
                      VectorSummary | LegacyJit |      X86 |   698.0484 us |  0.1858 us |  14.60 |          0.03 |         - |       0 B |
                     ForLoopSumImpl |    RyuJit |      X64 |    52.0464 us |  0.1321 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum |    RyuJit |      X64 |   501.3648 us |  1.6000 us |   9.63 |          0.03 |         - |      33 B |
        Generic_Sum_Scalar_Proposal |    RyuJit |      X64 |   214.7496 us |  0.5338 us |   4.13 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast |    RyuJit |      X64 |   215.3213 us |  0.5840 us |   4.14 |          0.01 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast |    RyuJit |      X64 |   215.9063 us |  0.1815 us |   4.15 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member |    RyuJit |      X64 |   214.1407 us |  0.0061 us |   4.11 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method |    RyuJit |      X64 |    51.9628 us |  0.0096 us |   1.00 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic |    RyuJit |      X64 | 1,494.0717 us |  9.1161 us |  28.71 |          0.15 | 1469.7421 |    4.8 MB |
     Generic_Sum_ScalarClass_Static |    RyuJit |      X64 |    71.8482 us |  0.1602 us |   1.38 |          0.00 |         - |       0 B |
   Generic_Sum_ScalarClass_Instance |    RyuJit |      X64 |    71.7464 us |  0.0031 us |   1.38 |          0.00 |         - |       0 B |
           Generic_Sum_ScalarHelper |    RyuJit |      X64 |    52.1151 us |  0.1212 us |   1.00 |          0.00 |         - |       0 B |
          Generic_Sum_Scalar_Helper |    RyuJit |      X64 |   214.7399 us |  0.5013 us |   4.13 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Static |    RyuJit |      X64 |   214.7182 us |  0.5259 us |   4.13 |          0.01 |         - |       0 B |
                      VectorSummary |    RyuJit |      X64 |    18.6934 us |  0.0088 us |   0.36 |          0.00 |         - |       0 B |

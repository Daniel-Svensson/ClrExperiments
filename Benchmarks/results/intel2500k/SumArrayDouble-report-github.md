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
                     ForLoopSumImpl | LegacyJit |      X64 |    71.6351 us | 0.1791 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X64 |   502.4377 us | 0.0430 us |   7.01 |          0.01 |         - |      32 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X64 |   238.6133 us | 0.4476 us |   3.33 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X64 |   358.5901 us | 1.2485 us |   5.01 |          0.02 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X64 |   386.7098 us | 0.7336 us |   5.40 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X64 |   239.1244 us | 0.2226 us |   3.34 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X64 |    95.2413 us | 0.0077 us |   1.33 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X64 | 1,815.0655 us | 5.9507 us |  25.34 |          0.09 | 1458.3333 |    4.8 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X64 |    95.4806 us | 0.3700 us |   1.33 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X64 |   238.0531 us | 0.0149 us |   3.32 |          0.01 |         - |       0 B |
                      VectorSummary | LegacyJit |      X64 |   805.8134 us | 0.1696 us |  11.25 |          0.02 |         - |       0 B |
                     ForLoopSumImpl | LegacyJit |      X86 |    71.5934 us | 0.1431 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X86 |   460.7058 us | 0.7320 us |   6.44 |          0.01 |         - |      22 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X86 |   238.6205 us | 0.5538 us |   3.33 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X86 |   238.1941 us | 0.0368 us |   3.33 |          0.01 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X86 |   238.1855 us | 0.1127 us |   3.33 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X86 |   238.8232 us | 0.4737 us |   3.34 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X86 |    71.8042 us | 0.0672 us |   1.00 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X86 | 1,629.8892 us | 3.8233 us |  22.77 |          0.06 |  955.0439 |    3.2 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X86 |    71.4405 us | 0.0122 us |   1.00 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X86 |   238.2840 us | 0.0985 us |   3.33 |          0.01 |         - |       0 B |
                      VectorSummary | LegacyJit |      X86 | 1,179.8006 us | 0.2608 us |  16.48 |          0.03 |         - |       0 B |
                     ForLoopSumImpl |    RyuJit |      X64 |    71.5141 us | 0.0333 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum |    RyuJit |      X64 |   500.3988 us | 0.1912 us |   7.00 |          0.00 |         - |      32 B |
        Generic_Sum_Scalar_Proposal |    RyuJit |      X64 |   214.6427 us | 0.3543 us |   3.00 |          0.00 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast |    RyuJit |      X64 |   457.4098 us | 0.0370 us |   6.40 |          0.00 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast |    RyuJit |      X64 |   461.7314 us | 0.3836 us |   6.46 |          0.01 |         - |       0 B |
            Generic_Sum_Base_Member |    RyuJit |      X64 |   238.6889 us | 0.3592 us |   3.34 |          0.00 |         - |       0 B |
              Generic_Sum_IL_Method |    RyuJit |      X64 |    71.5971 us | 0.1630 us |   1.00 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic |    RyuJit |      X64 | 1,459.7833 us | 3.9505 us |  20.41 |          0.05 | 1472.3837 |    4.8 MB |
           Generic_Sum_ScalarHelper |    RyuJit |      X64 |    71.4854 us | 0.0101 us |   1.00 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static |    RyuJit |      X64 |   214.2118 us | 0.0178 us |   3.00 |          0.00 |         - |       0 B |
                      VectorSummary |    RyuJit |      X64 |    37.6499 us | 0.0067 us |   0.53 |          0.00 |         - |       0 B |

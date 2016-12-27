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
                     ForLoopSumImpl | LegacyJit |      X64 |    71.7634 us |  0.2936 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X64 |   550.9525 us |  0.9106 us |   7.68 |          0.03 |         - |      35 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X64 |   239.2990 us |  0.8058 us |   3.33 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X64 |   520.8064 us |  1.8871 us |   7.26 |          0.03 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X64 |   521.7338 us |  0.0339 us |   7.27 |          0.02 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X64 |   238.9872 us |  0.9118 us |   3.33 |          0.02 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X64 |   214.4979 us |  0.5165 us |   2.99 |          0.01 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X64 | 1,963.4358 us | 12.4123 us |  27.36 |          0.17 | 1457.6823 |    4.8 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X64 |   357.6871 us |  0.6883 us |   4.98 |          0.02 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X64 |   238.7112 us |  0.5706 us |   3.33 |          0.01 |         - |       0 B |
                      VectorSummary | LegacyJit |      X64 |   590.2545 us |  0.2218 us |   8.23 |          0.03 |         - |       0 B |
                     ForLoopSumImpl | LegacyJit |      X86 |    71.4417 us |  0.0104 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum | LegacyJit |      X86 |   509.8255 us |  1.3443 us |   7.14 |          0.02 |         - |      20 B |
        Generic_Sum_Scalar_Proposal | LegacyJit |      X86 |   238.3234 us |  0.0161 us |   3.34 |          0.00 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast | LegacyJit |      X86 |   551.5812 us |  1.8653 us |   7.72 |          0.02 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast | LegacyJit |      X86 |   566.5922 us |  1.4431 us |   7.93 |          0.02 |         - |       0 B |
            Generic_Sum_Base_Member | LegacyJit |      X86 |   238.4431 us |  0.0286 us |   3.34 |          0.00 |         - |       0 B |
              Generic_Sum_IL_Method | LegacyJit |      X86 |   239.1767 us |  0.9408 us |   3.35 |          0.01 |         - |       0 B |
                Generic_Sum_Dynamic | LegacyJit |      X86 | 1,382.1485 us |  9.3484 us |  19.35 |          0.11 |  709.6920 |    2.4 MB |
           Generic_Sum_ScalarHelper | LegacyJit |      X86 |   239.7217 us |  0.2217 us |   3.36 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static | LegacyJit |      X86 |   239.2939 us |  0.6320 us |   3.35 |          0.01 |         - |       0 B |
                      VectorSummary | LegacyJit |      X86 |   700.2521 us |  0.2264 us |   9.80 |          0.00 |         - |       0 B |
                     ForLoopSumImpl |    RyuJit |      X64 |    71.8400 us |  0.2610 us |   1.00 |          0.00 |         - |       0 B |
                            LinqSum |    RyuJit |      X64 |   551.2392 us |  0.1656 us |   7.67 |          0.02 |         - |      35 B |
        Generic_Sum_Scalar_Proposal |    RyuJit |      X64 |   239.6227 us |  0.5736 us |   3.34 |          0.01 |         - |       0 B |
       Generic_Sum_ScalarOfT_NoCast |    RyuJit |      X64 |   457.9203 us |  0.9302 us |   6.37 |          0.02 |         - |       0 B |
 Generic_Sum_ScalarOfT_ExplicitCast |    RyuJit |      X64 |   463.1772 us |  0.0940 us |   6.45 |          0.02 |         - |       0 B |
            Generic_Sum_Base_Member |    RyuJit |      X64 |   238.7901 us |  0.3433 us |   3.32 |          0.01 |         - |       0 B |
              Generic_Sum_IL_Method |    RyuJit |      X64 |    71.7482 us |  0.0197 us |   1.00 |          0.00 |         - |       0 B |
                Generic_Sum_Dynamic |    RyuJit |      X64 | 1,484.7912 us |  5.1288 us |  20.67 |          0.08 | 1474.3217 |    4.8 MB |
           Generic_Sum_ScalarHelper |    RyuJit |      X64 |    71.7414 us |  0.0220 us |   1.00 |          0.00 |         - |       0 B |
            Generic_Sum_Base_Static |    RyuJit |      X64 |   215.6586 us |  0.0665 us |   3.00 |          0.01 |         - |       0 B |
                      VectorSummary |    RyuJit |      X64 |    18.9711 us |  0.0022 us |   0.26 |          0.00 |         - |       0 B |

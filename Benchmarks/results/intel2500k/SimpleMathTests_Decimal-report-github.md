``` ini

BenchmarkDotNet=v0.10.1, OS=Microsoft Windows NT 6.2.9200.0
Processor=Intel(R) Core(TM) i5-2500K CPU 3.30GHz, ProcessorCount=4
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
  [Host]     : Clr 4.0.30319.42000, 64bit RyuJIT-v4.6.1586.0
  Job-QTIBDN : Clr 4.0.30319.42000, 64bit LegacyJIT/clrjit-v4.6.1586.0;compatjit-v4.6.1586.0
  Job-IDHBCW : Clr 4.0.30319.42000, 32bit LegacyJIT-v4.6.1586.0
  Job-FPOZGJ : Clr 4.0.30319.42000, 64bit RyuJIT-v4.6.1586.0

Runtime=Clr  IterationTime=1.0000 s  LaunchCount=1  
TargetCount=3  WarmupCount=3  Allocated=0 B  

```
       Method |       Jit | Platform |      Mean |    StdDev | Scaled | Scaled-StdDev |
------------- |---------- |--------- |---------- |---------- |------- |-------------- |
    NormalSum | LegacyJit |      X64 | 2.6913 ms | 0.0054 ms |   1.00 |          0.00 |
    MethodSum | LegacyJit |      X64 | 2.8115 ms | 0.0060 ms |   1.04 |          0.00 |
 MethodRefSum | LegacyJit |      X64 | 2.8455 ms | 0.0117 ms |   1.06 |          0.00 |
 MethodIncSum | LegacyJit |      X64 | 3.1797 ms | 0.0002 ms |   1.18 |          0.00 |
   GenericSum | LegacyJit |      X64 | 3.0389 ms | 0.0094 ms |   1.13 |          0.00 |
    NormalSum | LegacyJit |      X86 | 2.2301 ms | 0.0061 ms |   1.00 |          0.00 |
    MethodSum | LegacyJit |      X86 | 2.5651 ms | 0.0069 ms |   1.15 |          0.00 |
 MethodRefSum | LegacyJit |      X86 | 2.2953 ms | 0.0327 ms |   1.03 |          0.01 |
 MethodIncSum | LegacyJit |      X86 | 2.4805 ms | 0.0081 ms |   1.11 |          0.00 |
   GenericSum | LegacyJit |      X86 | 2.7370 ms | 0.0064 ms |   1.23 |          0.00 |
    NormalSum |    RyuJit |      X64 | 2.2210 ms | 0.0052 ms |   1.00 |          0.00 |
    MethodSum |    RyuJit |      X64 | 2.4181 ms | 0.0100 ms |   1.09 |          0.00 |
 MethodRefSum |    RyuJit |      X64 | 2.2684 ms | 0.0093 ms |   1.02 |          0.00 |
 MethodIncSum |    RyuJit |      X64 | 2.3030 ms | 0.0061 ms |   1.04 |          0.00 |
   GenericSum |    RyuJit |      X64 | 2.4125 ms | 0.0015 ms |   1.09 |          0.00 |

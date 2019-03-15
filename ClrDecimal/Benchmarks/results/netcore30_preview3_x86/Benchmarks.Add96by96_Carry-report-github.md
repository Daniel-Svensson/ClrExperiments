``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|          Method |     Mean |     Error |    StdDev |   Median | Ratio | RatioSD |
|---------------- |---------:|----------:|----------:|---------:|------:|--------:|
|    NetFramework | 58.19 ns | 0.4515 ns | 0.6476 ns | 57.98 ns |  1.67 |    0.01 |
|          Native | 36.60 ns | 0.2207 ns | 0.3304 ns | 36.58 ns |  1.05 |    0.00 |
|           PalRT | 57.37 ns | 0.2811 ns | 0.4031 ns | 57.68 ns |  1.65 |    0.01 |
|           Ole32 | 34.82 ns | 0.1778 ns | 0.2550 ns | 34.81 ns |  1.00 |    0.00 |
| CoreCRTManaged2 | 96.88 ns | 0.7411 ns | 1.0862 ns | 96.04 ns |  2.78 |    0.05 |

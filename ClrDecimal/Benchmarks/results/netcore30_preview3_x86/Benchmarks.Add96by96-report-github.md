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
|          Native | 27.14 ns | 0.7222 ns | 1.0357 ns | 27.00 ns |  1.24 |    0.07 |
|           PalRT | 26.24 ns | 0.0250 ns | 0.0366 ns | 26.21 ns |  1.20 |    0.02 |
|           Ole32 | 21.82 ns | 0.2606 ns | 0.3653 ns | 22.16 ns |  1.00 |    0.00 |
|  CoreCRTManaged | 71.19 ns | 0.4465 ns | 0.6404 ns | 71.23 ns |  3.26 |    0.08 |
| CoreCRTManaged2 | 47.89 ns | 0.3890 ns | 0.5701 ns | 47.77 ns |  2.20 |    0.02 |

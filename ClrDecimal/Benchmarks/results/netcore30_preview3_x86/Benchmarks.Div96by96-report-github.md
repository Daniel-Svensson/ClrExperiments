``` ini

BenchmarkDotNet=v0.11.4, OS=Windows 10.0.17763.379 (1809/October2018Update/Redstone5)
Intel Core i5-2500K CPU 3.30GHz (Sandy Bridge), 1 CPU, 4 logical and 4 physical cores
Frequency=14318180 Hz, Resolution=69.8413 ns, Timer=HPET
.NET Core SDK=3.0.100-preview3-010431
  [Host] : .NET Core ? (CoreCLR 4.6.27422.72, CoreFX 4.7.19.12807), 32bit RyuJIT

Job=MediumRun  Toolchain=InProcessToolchain  IterationCount=15  
LaunchCount=2  WarmupCount=10  

```
|          Method |      Mean |     Error |    StdDev |    Median | Ratio | RatioSD |
|---------------- |----------:|----------:|----------:|----------:|------:|--------:|
|    NetFramework | 186.43 ns | 0.0103 ns | 0.0144 ns | 186.43 ns |  1.87 |    0.00 |
|          Native | 126.15 ns | 0.9066 ns | 1.3570 ns | 125.38 ns |  1.27 |    0.01 |
|           PalRT | 195.98 ns | 0.0200 ns | 0.0286 ns | 195.98 ns |  1.97 |    0.00 |
|           Ole32 |  99.48 ns | 0.1253 ns | 0.1875 ns |  99.48 ns |  1.00 |    0.00 |
|  CoreCRTManaged | 386.26 ns | 1.8292 ns | 2.5643 ns | 387.96 ns |  3.88 |    0.02 |
| CoreCRTManaged2 | 329.98 ns | 0.0217 ns | 0.0325 ns | 329.98 ns |  3.32 |    0.01 |

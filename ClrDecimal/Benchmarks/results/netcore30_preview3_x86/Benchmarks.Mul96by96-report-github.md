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
|    NetFramework |  92.68 ns | 0.4555 ns | 0.6533 ns |  92.10 ns |  1.32 |    0.01 |
|          Native |  71.32 ns | 0.3195 ns | 0.4782 ns |  70.93 ns |  1.02 |    0.01 |
|           PalRT | 164.00 ns | 0.0171 ns | 0.0250 ns | 164.00 ns |  2.34 |    0.02 |
|           Ole32 |  70.18 ns | 0.3966 ns | 0.5814 ns |  70.70 ns |  1.00 |    0.00 |
|  CoreCRTManaged | 267.76 ns | 1.4344 ns | 2.0572 ns | 269.06 ns |  3.81 |    0.00 |
| CoreCRTManaged2 | 222.97 ns | 0.0118 ns | 0.0169 ns | 222.97 ns |  3.18 |    0.03 |

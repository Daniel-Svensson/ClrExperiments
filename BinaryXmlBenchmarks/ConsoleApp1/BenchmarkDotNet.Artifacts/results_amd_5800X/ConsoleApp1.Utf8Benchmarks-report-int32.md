``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-QIEUWM : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|     Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev | Ratio | RatioSD |
|----------- |-------------------- |---------- |----------:|----------:|----------:|------:|--------:|
|   **Encoding** |                  **34** | **AsciiOnly** |  **9.236 ns** | **0.0306 ns** | **0.0286 ns** |  **2.59** |    **0.01** |
|  Int32Loop |                  34 | AsciiOnly |  9.528 ns | 0.0330 ns | 0.0292 ns |  2.68 |    0.01 |
| SimdSSE_v4 |                  34 | AsciiOnly |  3.772 ns | 0.0119 ns | 0.0099 ns |  1.06 |    0.00 |
|    SimdAVX |                  34 | AsciiOnly |  3.559 ns | 0.0057 ns | 0.0048 ns |  1.00 |    0.00 |
|  SimdAVX_2 |                  34 | AsciiOnly |  3.568 ns | 0.0082 ns | 0.0073 ns |  1.00 |    0.00 |
|  SimdAVX_3 |                  34 | AsciiOnly |  3.763 ns | 0.0082 ns | 0.0073 ns |  1.06 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Encoding** |                  **84** | **AsciiOnly** | **10.707 ns** | **0.0151 ns** | **0.0118 ns** |  **2.21** |    **0.01** |
|  Int32Loop |                  84 | AsciiOnly | 19.117 ns | 0.0317 ns | 0.0281 ns |  3.95 |    0.02 |
| SimdSSE_v4 |                  84 | AsciiOnly |  5.816 ns | 0.0187 ns | 0.0156 ns |  1.20 |    0.01 |
|    SimdAVX |                  84 | AsciiOnly |  4.837 ns | 0.0203 ns | 0.0190 ns |  1.00 |    0.00 |
|  SimdAVX_2 |                  84 | AsciiOnly |  4.813 ns | 0.0069 ns | 0.0054 ns |  1.00 |    0.00 |
|  SimdAVX_3 |                  84 | AsciiOnly |  5.238 ns | 0.0091 ns | 0.0076 ns |  1.08 |    0.00 |
|            |                     |           |           |           |           |       |         |
|   **Encoding** |                 **170** | **AsciiOnly** | **12.713 ns** | **0.0174 ns** | **0.0154 ns** |  **1.75** |    **0.02** |
|  Int32Loop |                 170 | AsciiOnly | 37.562 ns | 0.0913 ns | 0.0854 ns |  5.18 |    0.04 |
| SimdSSE_v4 |                 170 | AsciiOnly | 10.732 ns | 0.0196 ns | 0.0173 ns |  1.48 |    0.01 |
|    SimdAVX |                 170 | AsciiOnly |  7.260 ns | 0.0724 ns | 0.0642 ns |  1.00 |    0.00 |
|  SimdAVX_2 |                 170 | AsciiOnly |  7.175 ns | 0.0259 ns | 0.0229 ns |  0.99 |    0.01 |
|  SimdAVX_3 |                 170 | AsciiOnly |  7.578 ns | 0.0164 ns | 0.0153 ns |  1.04 |    0.01 |

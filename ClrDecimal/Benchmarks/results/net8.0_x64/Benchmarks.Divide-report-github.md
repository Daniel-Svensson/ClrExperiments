```

BenchmarkDotNet v0.13.12, Windows 11 (10.0.22631.3155/23H2/2023Update/SunValley3)
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK 8.0.101
  [Host] : .NET 8.0.1 (8.0.123.58001), X64 RyuJIT AVX2

Job=InProcess  Toolchain=InProcessEmitToolchain  

```
| Method | a                    | b                    | descr         | Mean     | Error    | StdDev   | Ratio |
|------- |--------------------- |--------------------- |-------------- |---------:|---------:|---------:|------:|
| **New**    | **92233(...)63.19 [21]** | **3.3**                  | **34bit / 32bit** | **23.09 ns** | **0.145 ns** | **0.121 ns** |  **0.54** |
| Main   | 92233(...)63.19 [21] | 3.3                  | 34bit / 32bit | 42.75 ns | 0.195 ns | 0.183 ns |  1.00 |
|        |                      |                      |               |          |          |          |       |
| **New**    | **10145(...)02.39 [22]** | **0.3**                  | **96bit / 32bit** | **14.21 ns** | **0.086 ns** | **0.072 ns** |  **0.48** |
| Main   | 10145(...)02.39 [22] | 0.3                  | 96bit / 32bit | 29.83 ns | 0.074 ns | 0.066 ns |  1.00 |
|        |                      |                      |               |          |          |          |       |
| **New**    | **39291(...)11.83 [23]** | **12884901.920**         | **96bit / 64bit** | **30.57 ns** | **0.045 ns** | **0.040 ns** |  **0.86** |
| Main   | 39291(...)11.83 [23] | 12884901.920         | 96bit / 64bit | 35.45 ns | 0.190 ns | 0.159 ns |  1.00 |
|        |                      |                      |               |          |          |          |       |
| **New**    | **39291(...)21183 [22]** | **18446744211148505120** | **96bit / 96bit** | **47.87 ns** | **0.129 ns** | **0.114 ns** |  **0.82** |
| Main   | 39291(...)21183 [22] | 18446744211148505120 | 96bit / 96bit | 58.72 ns | 0.106 ns | 0.094 ns |  1.00 |
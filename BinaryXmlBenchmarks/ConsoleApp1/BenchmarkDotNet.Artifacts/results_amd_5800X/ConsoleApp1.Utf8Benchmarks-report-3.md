``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-GOOLTT : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|        Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |    Median |
|-------------- |-------------------- |---------- |----------:|----------:|----------:|----------:|
|      **Original** |                   **5** | **AsciiOnly** |  **3.667 ns** | **0.0079 ns** | **0.0066 ns** |  **3.669 ns** |
|      Encoding |                   5 | AsciiOnly |  7.491 ns | 0.0127 ns | 0.0119 ns |  7.490 ns |
|     Int32Loop |                   5 | AsciiOnly |  3.430 ns | 0.0079 ns | 0.0066 ns |  3.430 ns |
|    SimdSSE_v4 |                   5 | AsciiOnly |  3.199 ns | 0.0053 ns | 0.0050 ns |  3.199 ns |
|     SimdAVX_2 |                   5 | AsciiOnly |  3.495 ns | 0.0064 ns | 0.0057 ns |  3.493 ns |
| SimdVector256 |                   5 | AsciiOnly |  3.487 ns | 0.0095 ns | 0.0153 ns |  3.483 ns |
|      **Original** |                   **8** | **AsciiOnly** |  **4.524 ns** | **0.0129 ns** | **0.0115 ns** |  **4.526 ns** |
|      Encoding |                   8 | AsciiOnly |  7.694 ns | 0.0056 ns | 0.0044 ns |  7.696 ns |
|     Int32Loop |                   8 | AsciiOnly |  3.871 ns | 0.0093 ns | 0.0082 ns |  3.875 ns |
|    SimdSSE_v4 |                   8 | AsciiOnly |  2.768 ns | 0.0051 ns | 0.0048 ns |  2.767 ns |
|     SimdAVX_2 |                   8 | AsciiOnly |  2.767 ns | 0.0070 ns | 0.0065 ns |  2.768 ns |
| SimdVector256 |                   8 | AsciiOnly |  2.777 ns | 0.0037 ns | 0.0034 ns |  2.777 ns |
|      **Original** |                  **10** | **AsciiOnly** |  **5.115 ns** | **0.0189 ns** | **0.0177 ns** |  **5.108 ns** |
|      Encoding |                  10 | AsciiOnly |  7.916 ns | 0.0122 ns | 0.0102 ns |  7.914 ns |
|     Int32Loop |                  10 | AsciiOnly |  4.276 ns | 0.0076 ns | 0.0068 ns |  4.277 ns |
|    SimdSSE_v4 |                  10 | AsciiOnly |  2.973 ns | 0.0093 ns | 0.0087 ns |  2.970 ns |
|     SimdAVX_2 |                  10 | AsciiOnly |  2.772 ns | 0.0053 ns | 0.0047 ns |  2.773 ns |
| SimdVector256 |                  10 | AsciiOnly |  3.067 ns | 0.0167 ns | 0.0148 ns |  3.065 ns |
|      **Original** |                  **16** | **AsciiOnly** |  **7.357 ns** | **0.0669 ns** | **0.0626 ns** |  **7.361 ns** |
|      Encoding |                  16 | AsciiOnly |  8.554 ns | 0.0155 ns | 0.0137 ns |  8.554 ns |
|     Int32Loop |                  16 | AsciiOnly |  5.217 ns | 0.0143 ns | 0.0134 ns |  5.217 ns |
|    SimdSSE_v4 |                  16 | AsciiOnly |  2.987 ns | 0.0098 ns | 0.0087 ns |  2.985 ns |
|     SimdAVX_2 |                  16 | AsciiOnly |  2.770 ns | 0.0056 ns | 0.0050 ns |  2.770 ns |
| SimdVector256 |                  16 | AsciiOnly |  2.802 ns | 0.0077 ns | 0.0068 ns |  2.801 ns |
|      **Original** |                  **20** | **AsciiOnly** |  **8.159 ns** | **0.0708 ns** | **0.0695 ns** |  **8.145 ns** |
|      Encoding |                  20 | AsciiOnly |  9.612 ns | 0.1040 ns | 0.2629 ns |  9.582 ns |
|     Int32Loop |                  20 | AsciiOnly |  6.189 ns | 0.0699 ns | 0.1753 ns |  6.111 ns |
|    SimdSSE_v4 |                  20 | AsciiOnly |  3.229 ns | 0.0165 ns | 0.0147 ns |  3.225 ns |
|     SimdAVX_2 |                  20 | AsciiOnly |  3.438 ns | 0.0081 ns | 0.0076 ns |  3.436 ns |
| SimdVector256 |                  20 | AsciiOnly |  3.227 ns | 0.0071 ns | 0.0063 ns |  3.228 ns |
|      **Original** |                  **30** | **AsciiOnly** | **10.717 ns** | **0.0926 ns** | **0.0723 ns** | **10.707 ns** |
|      Encoding |                  30 | AsciiOnly | 10.146 ns | 0.0625 ns | 0.0554 ns | 10.127 ns |
|     Int32Loop |                  30 | AsciiOnly |  9.043 ns | 0.0987 ns | 0.0970 ns |  9.039 ns |
|    SimdSSE_v4 |                  30 | AsciiOnly |  3.661 ns | 0.0057 ns | 0.0051 ns |  3.660 ns |
|     SimdAVX_2 |                  30 | AsciiOnly |  3.411 ns | 0.0051 ns | 0.0048 ns |  3.410 ns |
| SimdVector256 |                  30 | AsciiOnly |  3.288 ns | 0.0134 ns | 0.0125 ns |  3.287 ns |
|      **Original** |                  **32** | **AsciiOnly** | **11.116 ns** | **0.0687 ns** | **0.0609 ns** | **11.116 ns** |
|      Encoding |                  32 | AsciiOnly |  9.438 ns | 0.0139 ns | 0.0116 ns |  9.440 ns |
|     Int32Loop |                  32 | AsciiOnly |  9.501 ns | 0.0971 ns | 0.0811 ns |  9.471 ns |
|    SimdSSE_v4 |                  32 | AsciiOnly |  3.652 ns | 0.0176 ns | 0.0156 ns |  3.644 ns |
|     SimdAVX_2 |                  32 | AsciiOnly |  3.425 ns | 0.0078 ns | 0.0073 ns |  3.424 ns |
| SimdVector256 |                  32 | AsciiOnly |  3.203 ns | 0.0053 ns | 0.0047 ns |  3.204 ns |
|      **Original** |                  **34** | **AsciiOnly** | **11.562 ns** | **0.0515 ns** | **0.0430 ns** | **11.567 ns** |
|      Encoding |                  34 | AsciiOnly |  9.460 ns | 0.0143 ns | 0.0127 ns |  9.464 ns |
|     Int32Loop |                  34 | AsciiOnly |  9.683 ns | 0.0526 ns | 0.0492 ns |  9.698 ns |
|    SimdSSE_v4 |                  34 | AsciiOnly |  3.901 ns | 0.0157 ns | 0.0122 ns |  3.898 ns |
|     SimdAVX_2 |                  34 | AsciiOnly |  3.848 ns | 0.0085 ns | 0.0071 ns |  3.850 ns |
| SimdVector256 |                  34 | AsciiOnly |  3.651 ns | 0.0075 ns | 0.0066 ns |  3.652 ns |
|      **Original** |                  **84** | **AsciiOnly** | **22.269 ns** | **0.0708 ns** | **0.0627 ns** | **22.277 ns** |
|      Encoding |                  84 | AsciiOnly | 11.141 ns | 0.0113 ns | 0.0094 ns | 11.141 ns |
|     Int32Loop |                  84 | AsciiOnly | 19.601 ns | 0.0301 ns | 0.0267 ns | 19.597 ns |
|    SimdSSE_v4 |                  84 | AsciiOnly |  5.958 ns | 0.0163 ns | 0.0145 ns |  5.955 ns |
|     SimdAVX_2 |                  84 | AsciiOnly |  5.148 ns | 0.0097 ns | 0.0091 ns |  5.150 ns |
| SimdVector256 |                  84 | AsciiOnly |  5.155 ns | 0.0087 ns | 0.0081 ns |  5.154 ns |
|      **Original** |                 **170** | **AsciiOnly** | **44.331 ns** | **0.0684 ns** | **0.0607 ns** | **44.326 ns** |
|      Encoding |                 170 | AsciiOnly | 12.310 ns | 0.0168 ns | 0.0149 ns | 12.312 ns |
|     Int32Loop |                 170 | AsciiOnly | 38.353 ns | 0.0795 ns | 0.0744 ns | 38.323 ns |
|    SimdSSE_v4 |                 170 | AsciiOnly | 10.980 ns | 0.0215 ns | 0.0179 ns | 10.985 ns |
|     SimdAVX_2 |                 170 | AsciiOnly |  7.546 ns | 0.0112 ns | 0.0105 ns |  7.549 ns |
| SimdVector256 |                 170 | AsciiOnly |  9.863 ns | 0.0161 ns | 0.0143 ns |  9.863 ns |

``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.22000
AMD Ryzen 7 5800X, 1 CPU, 16 logical and 8 physical cores
.NET SDK=7.0.100-preview.6.22352.1
  [Host]     : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT
  Job-QIEUWM : .NET 7.0.0 (7.0.22.32404), X64 RyuJIT

MaxRelativeError=0.01  IterationTime=250.0000 ms  

```
|               Method | StringLengthInChars |  Scenario |      Mean |     Error |    StdDev |
|--------------------- |-------------------- |---------- |----------:|----------:|----------:|
|             **Encoding** |                  **42** | **AsciiOnly** |  **5.047 ns** | **0.0167 ns** | **0.0148 ns** |
|         VectorLength |                  42 | AsciiOnly |  2.087 ns | 0.0069 ns | 0.0058 ns |
| VectorLength_Aligned |                  42 | AsciiOnly |  2.077 ns | 0.0086 ns | 0.0076 ns |
|             **Encoding** |                  **85** | **AsciiOnly** |  **6.113 ns** | **0.0233 ns** | **0.0218 ns** |
|         VectorLength |                  85 | AsciiOnly |  2.755 ns | 0.0149 ns | 0.0139 ns |
| VectorLength_Aligned |                  85 | AsciiOnly |  2.761 ns | 0.0144 ns | 0.0134 ns |
|             **Encoding** |                 **256** | **AsciiOnly** |  **9.585 ns** | **0.0339 ns** | **0.0317 ns** |
|         VectorLength |                 256 | AsciiOnly |  6.839 ns | 0.0227 ns | 0.0201 ns |
| VectorLength_Aligned |                 256 | AsciiOnly |  6.944 ns | 0.0211 ns | 0.0197 ns |
|             **Encoding** |                 **512** | **AsciiOnly** | **15.959 ns** | **0.0308 ns** | **0.0288 ns** |
|         VectorLength |                 512 | AsciiOnly | 11.452 ns | 0.0327 ns | 0.0290 ns |
| VectorLength_Aligned |                 512 | AsciiOnly | 11.632 ns | 0.0165 ns | 0.0147 ns |
|             **Encoding** |                **2048** | **AsciiOnly** | **59.927 ns** | **0.1031 ns** | **0.0914 ns** |
|         VectorLength |                2048 | AsciiOnly | 36.509 ns | 0.0851 ns | 0.0754 ns |
| VectorLength_Aligned |                2048 | AsciiOnly | 44.433 ns | 0.4511 ns | 0.7154 ns |

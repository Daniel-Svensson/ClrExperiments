using System;
using System.Collections.Generic;
using System.IO.Hashing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;

namespace Benchmarks;

public class XXHashBenchmarks
{
    [Params(2,/* 4, 8, 16,*/ 32, /*64,*/ /*128,*/ 1024 * 1024)]
    public int Count { get; set; }
    
    private byte[] _output = new byte[100];
    private byte[] _input;

    [GlobalSetup]
    public void Setup() => _input = RandomNumberGenerator.GetBytes(Count);

    [Benchmark(Baseline = true)] public int XXH32_Hash() => XxHash32.Hash(_input, _output);
    [Benchmark] public int XXH64_Hash() => XxHash64.Hash(_input, _output);
    [Benchmark] public int XXH3_Hash() => XxHash3.Hash(_input, _output);
    
}

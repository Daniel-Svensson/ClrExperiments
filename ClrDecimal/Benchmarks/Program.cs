#define TEST_32bit_with_0_scale
#define TEST_32bit_with_scale
#define TEST_64bit_with_scale_64bit_result
#define TEST_64bit_with_0_scale_128bit_result
#define TEST_64bit_with_scale_128bit_result
//#define TEST_96bit_with_scale_96bit_result_and_overflow
//#define TEST_96bit_with_scale_96bit_result_no_overflow
//#define TEST_Bitpatterns_with_all_scales

#define TEST_MULTIPLY
//#define TEST_ADD
//#define TEST_SUB
#define TEST_DIV

using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Diagnosers;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Running;
using Benchmarks.Distributions;
using CommandLine;
using Perfolizer.Horology;

namespace Benchmarks
{
    class Program
    {
        static readonly bool Is64Bit = IntPtr.Size == 8;
        const int DEC_SCALE_MAX = 28;


        unsafe static delegate*<decimal, decimal, decimal> CastPointer<T>(delegate*<T, T, T> ptr)
            => (delegate*<decimal, decimal, decimal>)ptr;

        unsafe static void Main(string[] args)
        {
            Console.WriteLine($"Bitness is {IntPtr.Size * 8}");
#if TARGET_64BIT
            Console.WriteLine($"TARGET_64BIT is defined");
#endif
#if TARGET_32BIT
            Console.WriteLine($"TARGET_32BIT is defined");
#endif
            //Managed.New.Decimal a = 1m / 365, b = 10_000m, res = 42m;
            //Managed.Main.Decimal a2 = 1m / 365, b2 = 10_000m, res2 = 42m;

            //for (int i = 0; i < 5_000_000; ++i)
            //{
            //    res = res + (a + b);
            //    //  res = /*res * */a * b;
            //    res = res / (a * b);

            //    res2 = res2 + (a2 + b2);
            //    //res2 = /*res2 * */a2 * b2;
            //    res2 = res2 / (a2 * b2);
            //}
            //return;

            //Managed.New.Decimal a = new decimal(1023, 412, 213, false, 2), b = new decimal(32, 0, 0, false, 3);
            //Managed.New.Decimal res;
            //Managed.Main.Decimal a2 = new decimal(1023, 412, 213, false, 2), b2 = new decimal(32, 0, 0, false, 3);
            //Managed.Main.Decimal res2;

            //for (int i = 0; i < 100_000; ++i)
            //{
            //    res = a / b;
            //    res = a % b;
            //    res2 = a2 / b2;
            //}

            //Console.WriteLine("\n\n----------------- SLEEP ------------------------");
            //Thread.Sleep(7_000);

            //for (int i = 0; i < 200_000_000; ++i)
            //{
            //    res = a / b;
            //    res2 = a2 / b2;
            //}

            //return;

            var mul = new Multiply();
            foreach(var testCase in mul.TestCases())
            {
                var expected = mul.System_Decimal((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);
                var actual = mul.New((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);

                Console.Write($"Verifying: {testCase[2]} ");
                if (actual != expected)
                    Console.WriteLine($"- FAILED: {actual} but expected {expected}");
                else
                    Console.WriteLine($"- PASS");

                Debug.Assert(actual == expected);
            }

            var div = new Divide();
            foreach (var testCase in div.TestCases())
            {
                var expected = div.System_Decimal((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);
                var actual = div.New((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);
                var _ = div.Main((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);

                Console.Write($"Verifying: {testCase[2]} ");
                if (actual != expected)
                    Console.WriteLine($"- FAILED: {actual} but expected {expected}");
                else
                    Console.WriteLine($"- PASS");

                Debug.Assert(actual == expected);
            }

            var add = new Addition();
            foreach (var testCase in add.TestCases())
            {
                var expected = add.System_Decimal((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);
                var actual = add.New((decimal)testCase[0], (decimal)testCase[1], (string)testCase[2]);

                Console.Write($"Verifying: {testCase[2]} ");
                if (actual != expected)
                    Console.WriteLine($"- FAILED: {actual} but expected {expected}");
                else
                    Console.WriteLine($"- PASS");

                Debug.Assert(actual == expected);
            }


            var config = BenchmarkDotNet.Configs.DefaultConfig.Instance
    .AddJob(Job.InProcess/*.WithIterationCount(5)*/
    //.AddHardwareCounters(
    //HardwareCounter.TotalCycles,
    //HardwareCounter.TotalIssues,
    //HardwareCounter.InstructionRetired
    //HardwareCounter.UnhaltedCoreCycles
    //HardwareCounter.Timer,
    //HardwareCounter.BranchInstructionRetired,
    //HardwareCounter.BranchMispredictsRetired,
    )
    ;

            BenchmarkRunner.Run<Addition>(config);
            BenchmarkRunner.Run<InterestBenchmark>(config);
            BenchmarkRunner.Run<Multiply>(config);
            BenchmarkRunner.Run<Divide>(config);
            ////BenchmarkRunner.Run<MulPentP>();
            ////BenchmarkRunner.Run<DivPentP>();

                
            return;

            //PrintStats(Distributions.AddPentP.LoadFile());


            var program = new Program();

            Console.WriteLine("----------------- WARMUP ------------------------");

#if TEST_DIV
            program.run_benchmarks(iterations: 10, elements: 500, bytes: 4,
            baseline_name: "Managed.Main.Divide", func_name: "Managed.New.Divide",
            baseline: CastPointer<Managed.Main.Decimal>(&Managed.Main.Decimal.Divide),
            func: CastPointer<Managed.New.Decimal>(&Managed.New.Decimal.Divide));

            program.run_benchmarks(iterations: 10, elements: 500, bytes: 4,
baseline_name: "Ole32 Div", func_name: "System. Div",
baseline: &ClrClassLibrary.Methods.DivOle32,
func: &decimal.Divide);
#endif

#if TEST_MULTIPLY
            program.run_benchmarks(iterations: 10, elements: 500, bytes: 4,
baseline_name: "Managed.Main Multiply", func_name: "Managed.New Multiply",
baseline: CastPointer<Managed.Main.Decimal>(&Managed.Main.Decimal.Multiply),
func: CastPointer<Managed.New.Decimal>(&Managed.New.Decimal.Multiply));

            program.run_benchmarks(iterations: 10, elements: 500, bytes: 4,
baseline_name: "MulOle32", func_name: "System. Multiply",
baseline: &ClrClassLibrary.Methods.MulOle32,
func: &decimal.Multiply);
#endif
            Console.WriteLine("\n\n----------------- SLEEP ------------------------");
            Thread.Sleep(10_000);

            Console.WriteLine("----------------- BENCH ------------------------");

            //            program.run_benchmarks(iterations: 1, elements: 8_000, bytes: 4,
            //baseline_name: "System.Decimal", func_name: "Managed.New",
            //baseline: &Decimal.Divide,
            //func: CastPointer<Managed.New.Decimal>(&Managed.New.Decimal.Divide));

#if TEST_DIV
            program.run_benchmarks(iterations: 1, elements: 4_000, bytes: 4,
            baseline_name: "Managed.Main.Div", func_name: "Managed.New.Div",
            baseline: CastPointer<Managed.Main.Decimal>(&Managed.Main.Decimal.Divide),
            func: CastPointer<Managed.New.Decimal>(&Managed.New.Decimal.Divide));

            program.run_benchmarks(iterations: 1, elements: 4_000, bytes: 4,
baseline_name: "Ole32 Div", func_name: "System. Div",
baseline: &ClrClassLibrary.Methods.DivOle32,
func: &decimal.Divide);
#endif

#if TEST_MULTIPLY
            program.run_benchmarks(iterations: 1, elements: 6_000, bytes: 4,
            baseline_name: "Managed.Main Multiply", func_name: "Managed.New Multiply",
            baseline: CastPointer<Managed.Main.Decimal>(&Managed.Main.Decimal.Multiply),
            func: CastPointer<Managed.New.Decimal>(&Managed.New.Decimal.Multiply));

            program.run_benchmarks(iterations: 1, elements: 6_000, bytes: 4,
baseline_name: "MulOle32", func_name: "System. Multiply",
baseline: &ClrClassLibrary.Methods.MulOle32,
func: &decimal.Multiply);
#endif
            return;



            //BenchmarkRunner.Run<Add96by96_Carry>(config);
            //BenchmarkRunner.Run<Add96by96>(config);
            //BenchmarkRunner.Run<Mul96by96>(config);
            //BenchmarkRunner.Run<Div96by96>(config);

            BenchmarkRunner.Run<InterestBenchmark>(config);

            //BenchmarkRunner.Run<Distributions.AddPentP>(config);
            //BenchmarkRunner.Run<Distributions.MulPentP>(config);
            //BenchmarkRunner.Run<Distributions.DivPentP>(config);

            //BenchmarkRunner.Run<AddDummy>(config);


            if (Debugger.IsAttached)
            {
                Console.WriteLine("Press ENTER to exit");
                Console.ReadLine();
            }
        }




        // Runs a single benchmark pass and records the timing when calling func
        // with for all combination ov values from lhs and rhs.
        unsafe long run_benchmark(
    ReadOnlySpan<decimal> lhs,
    ReadOnlySpan<decimal> rhs,
    Span<decimal> target, Span<Exception> result,
    delegate*<decimal, decimal, decimal> binary_op)
        {
            var sw = Stopwatch.StartNew();

            int dest = 0;

            for (int i = 0; i < lhs.Length; ++i)
                for (int j = 0; j < rhs.Length; ++j)
                {
                    try
                    {
                        target[dest] = binary_op(lhs[i], rhs[j]);
                        result[dest] = null;
                    }
                    catch (Exception ex)
                    {
                        target[dest] = decimal.Zero;
                        result[dest] = ex;
                    }
                }

            return sw.ElapsedMilliseconds;
        }


        void CompareResult(
        ReadOnlySpan<decimal> lhs,
        ReadOnlySpan<decimal> rhs,
        ReadOnlySpan<Managed.New.Decimal> expected, ReadOnlySpan<Exception> expected_result,
        ReadOnlySpan<Managed.New.Decimal> actual, ReadOnlySpan<Exception> actual_result)
        {
            int errors = 0;

            int result_idx = (int)0 - 1;
            for (int i = 0; i < lhs.Length; ++i)
                for (int j = 0; j < rhs.Length; ++j)
                {
                    ++result_idx;

                    if (!object.ReferenceEquals(expected_result[result_idx], actual_result[result_idx]))
                    {
                        if (expected_result[result_idx].GetType() == actual_result[result_idx].GetType())
                            continue;
                        else
                        {
                            if (++errors < 10)
                            {
                                Console.WriteLine($"Wrong exception type actual {actual_result[result_idx].ToString()} expected {expected_result[result_idx].ToString()}");
                            }
                        }
                    }
                    else if (expected[result_idx] != actual[result_idx])
                    {
                        //var cmp = actual_result[result_idx] is null ?
                        //    VarDecCmp(const_cast<DECIMAL*>(&expected[result_idx]), const_cast<DECIMAL*>(&actual[result_idx]))
                        //    : (expected_result[result_idx] == actual_result[result_idx] ? VARCMP_EQ : ~VARCMP_EQ);
                        //if (cmp == VARCMP_EQ)
                        //{
                        //    if (expected_result[result_idx] != actual_result[result_idx])
                        //        printf("[%zi] ONLY RESULT DIFFERENT?\n", result_idx);
                        //    else
                        //    {
                        //        ++scale_diff;
                        //        //printf("[%i] FALSE POSITIVE\n", result_idx);
                        //    }
                        //}
                        if (++errors < 10)
                        {

                            //printf("[%i]x[%i]  -- (%i) %lu %I64u / 10^%i  * (%i) %lu %I64u / 10^%i: \n",
                            //    i, j,
                            //    lhs[i].sign, lhs[i].High, lhs[i].Low64, lhs[i].scale,
                            //    rhs[j].sign, rhs[j].High, rhs[j].Low64, rhs[j].scale);

                            //if (expected_result[result_idx] != actual_result[result_idx])
                            //    printf(" RESULT expected %li actual %li\n", expected_result[result_idx], actual_result[result_idx]);
                            //else
                            Console.WriteLine(" PRODUCT expected {0} {1} {2} / 10^{3} but got {4} {5} {6} / 10^{7}",
                                    Decimal.IsNegative(expected[result_idx]) ? "-" : " ", expected[result_idx].High, expected[result_idx].Low64, expected[result_idx].Scale,
                                    Decimal.IsNegative(actual[result_idx]) ? "-" : " ", actual[result_idx].High, actual[result_idx].Low64, actual[result_idx].Scale
                                );
                        }
                    }
                }


            if (errors > 0)
                Console.WriteLine($"ERRORS FOUND: {errors} with different results ({100.0 * (double)errors / (double)(expected.Length)}%)");
        }

        unsafe void compare_benchmark(
        string scenario,
        string first,
        string second,

        int iterations,
        ReadOnlySpan<decimal> lhs,
        ReadOnlySpan<decimal> rhs,
        Span<decimal> first_target, Span<Exception> first_result,
        Span<decimal> second_target, Span<Exception> second_result,
        delegate*<decimal, decimal, decimal> first_func,
    delegate*<decimal, decimal, decimal> second_func)
        {
            long[] timings1 = new long[iterations];
            long[] timings2 = new long[iterations];

            for (int i = 0; i < iterations; ++i)
            {
#if NO_COMPARE
		var time2 = run_benchmark(lhs, rhs, second_target, second_result, second_func);
		var time1 = time2;
#else
                var time1 = run_benchmark(lhs, rhs, first_target, first_result, first_func);
                var time2 = run_benchmark(lhs, rhs, second_target, second_result, second_func);
#endif
                timings1[i] = time1;
                timings2[i] = time2;
            }

            for (int i = 0; i < iterations; ++i)
            {
                var time1 = timings1[i];
                var time2 = timings2[i];
                double seconds1 = (double)time1 / (double)Stopwatch.Frequency;
                double seconds2 = (double)time2 / (double)Stopwatch.Frequency;

                double percent_of_original = (seconds2 / seconds1);
                double speedup = (seconds1 - seconds2) / seconds2;

                Console.WriteLine($"{scenario}; {second}; {first}; {time1}; {seconds1}; {time2} {seconds2} {percent_of_original * 100.0} {speedup * 100.0}");
            }

#if !(NO_VALIDATE || NO_COMPARE || NO_COMPARE_ONLY_BASELINE)
            int first_hres0 = (int)first_result.ToArray().Count(x => x is null);
            int second_hres0 = (int)second_result.ToArray().Count(x => x is null);
            //cout << second_hres0 << " out of " << first_target.Length << " resuts was successfull, expected " << first_hres0 << " => ratio " << 100.0 * (double)second_hres0 / (double)(first_target.Length) << "%" << endl;


#if NO_VERBOSE_OUTPUT
            if (first_hres0 != second_hres0)
#endif
            //    printf("%d out of %zi results was successfull, expected %d, ratio is %f%%\n", second_hres0, first_target.Length, first_hres0, 100.0 * (double)second_hres0 / (double)(first_target.Length));

            CompareResult(lhs, rhs, MemoryMarshal.Cast<decimal, Managed.New.Decimal>(first_target), first_result, MemoryMarshal.Cast<decimal, Managed.New.Decimal>(second_target), second_result);
#endif // !NO_COMPARE
        }

        unsafe void run_benchmarks(int iterations, int elements, int bytes,
            string baseline_name, string func_name,
            delegate*<decimal, decimal, decimal> baseline,
            delegate*<decimal, decimal, decimal> func
        )
        {
            decimal[] numbers = new decimal[Math.Max(elements, 435)];
            decimal[] targetA = new decimal[elements * Math.Max(elements, 435)];
            decimal[] targetC = new decimal[elements * Math.Max(elements, 435)];
            Exception[] hresultA = new Exception[elements * Math.Max(elements, 435)];
            Exception[] hresultC = new Exception[elements * Math.Max(elements, 435)];
            Span<Managed.New.Decimal.DecCalc> numbersAsCalc = MemoryMarshal.Cast<decimal, Managed.New.Decimal.DecCalc>(numbers.AsSpan());


            InitializeTestData(numbers, bytes);

            // Change scale
            const int minScale = 10;
            const int maxScale = DEC_SCALE_MAX - 2;

            Console.WriteLine("{0};;{1};;;%% time;speedup;elements={2}\n", baseline_name, func_name, elements);
            Console.WriteLine("scenario;ticks;sec;ticks;sec;;;");

#if TEST_32bit_with_0_scale
            compare_benchmark("32bit x 32bit with scale 0", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif


#if TEST_32bit_with_scale
            for (int i = 0; i < numbers.Length; ++i)
                numbersAsCalc[i].scale = (byte)Random.Shared.Next(minScale, maxScale + 1);

            compare_benchmark("32bit x 32bit with scale in range [10,26]", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

            //#ifdef TEST_64bit_with_scale_64bit_result
#if TEST_64bit_with_scale_64bit_result
            for (int i = 0; i < numbers.Length; ++i)
            {
                numbersAsCalc[i].scale = random_scale(minScale, maxScale);
                numbersAsCalc[i].Mid = numbersAsCalc[i].Low >> 4;
            }

            // use 4 bits with scales [0,28]
            byte smallNumScaleLimit = DEC_SCALE_MAX;
            int smallNumBitLimit = (1 << 4) - 1;
            int numSmallNumbers = smallNumBitLimit * (smallNumScaleLimit + 1);
            decimal[] smallNumbers = new decimal[numSmallNumbers];
            int smallNumbersInit = 0;
            Span<Managed.New.Decimal.DecCalc> smallNumbersCalc = MemoryMarshal.Cast<decimal, Managed.New.Decimal.DecCalc>(smallNumbers.AsSpan());
            for (int bits = 1; bits <= smallNumBitLimit; bits++)
            {
                for (byte scale = 0; scale <= smallNumScaleLimit; scale++)
                {
                    smallNumbers[smallNumbersInit] = bits;
                    smallNumbersCalc[smallNumbersInit].scale = scale;
                    ++smallNumbersInit;
                }
            }
            compare_benchmark("64bit values -> 64bit results with varying scale", baseline_name, func_name, iterations, numbers, smallNumbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#if TEST_64bit_with_0_scale_128bit_result
            for (int i = 0; i < numbers.Length; ++i)
            {
                numbersAsCalc[i].Mid = UInt32.RotateLeft(numbersAsCalc[i].Low, 14);
                numbersAsCalc[i].scale = 0;
            }

            compare_benchmark("64bit values -> 65-128 bit results and no scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#if TEST_64bit_with_scale_128bit_result
            for (int i = 0; i < numbers.Length; ++i)
            {
                numbersAsCalc[i].Mid = UInt32.RotateLeft(numbersAsCalc[i].Low, 14);
                numbersAsCalc[i].scale = random_scale(7, 15);
            }

            compare_benchmark("64bit values -> 65-128 bit results and scale", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#if TEST_96bit_with_scale_96bit_result_and_overflow
            for (int i = 0; i < numbers.Length; ++i)
            {
                numbersAsCalc[i].scale = (byte)(Random.Shared.Next() % 5);
                numbersAsCalc[i].High = numbersAsCalc[i].Mid = numbersAsCalc[i].Low;
            }

            compare_benchmark("96bit values with high overflow probablility", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif

#if TEST_96bit_with_scale_96bit_result_no_overflow
            for (int i = 0; i < numbers.Length; ++i)
            {
                numbersAsCalc[i].scale = random_scale(minScale + 5, maxScale);
                numbersAsCalc[i].High = numbersAsCalc[i].Mid = numbersAsCalc[i].Low;
            }

            compare_benchmark("96bit values with no overflow", baseline_name, func_name, iterations, numbers, numbers, targetA, hresultA, targetC, hresultC, baseline, func);
#endif
        }

        private byte random_scale(int minScale, int maxScale)
        {
            return (byte)Random.Shared.Next(minScale, maxScale + 1);
        }

        void InitializeTestData(Span<decimal> numbers, int bytes)
        {
            // Start with some specific patters
            int bits = bytes * 8;

            ulong allBits = (0xffffffffffffffffUL) >> (64 - bits);
            const ulong noBits = (0x0000000000000000UL);

            Debug.Assert(numbers.Length >= 2 + 3 * bits + 2 * bytes);

            // Genereate 0x0000000000000000, 0x000000000000000f, 0x00000000000000ff, to 0xffffffffffffffffu
            int dest = 0;
            numbers[dest++] = new decimal(allBits);
            numbers[dest++] = new decimal(noBits);

            // Some bit patterns
            for (int i = 1; i < bits; ++i)
            {
                // i bits to left set
                numbers[dest++] = (allBits >> i) & allBits;
                // i bits to right set
                numbers[dest++] = (allBits << i) & allBits;
                // only bit i set
                numbers[dest++] = (1UL << i) & allBits;
            }

            // Genereate 0x000000000000000f, 0x0000000000000f0f, ...	
            for (int i = 1; i < bytes; ++i)
            {
                numbers[dest++] = (allBits >> i * 8) & allBits;
                numbers[dest++] = (allBits << i * 8) & allBits;
            }

            // Generate deterministic random sequence to fill the rest of the numbers
            Random rand = new Random(42);
            for (; dest < numbers.Length; ++dest)
            {
                ulong number = 0;
                for (int i = 0; i < bytes; ++i)
                    number = (ulong)(number << 8) | (ulong)((uint)rand.Next() & 0x00ffu);

                Debug.Assert(number == (number & allBits));
                //random.NextBytes((byte*)&number, bytes);
                if (number == 0)
                    number = 42;
                numbers[dest] = number;
            }
        }

        void test_round_to_nearest()
        {
            TestMultiply(new decimal(32, 0, 0, false, 10), new decimal(3, 0, 0, false, 19)); // 96

            //a = 95;
            //b = 1;
            //a.scale = 10;
            //b.scale = 19;
            //TestMultiply(a, b); // 95 => 100 (even)
            TestMultiply(new decimal(95, 0, 0, false, 10), new decimal(1, 0, 0, false, 19)); // 95 => 100 (even)

            TestMultiply(new decimal(85, 0, 0, false, 10), new decimal(1, 0, 0, false, 19)); // 85 => 100 (even)

            //a = 94;
            //b = 1;
            //a.scale = 10;
            //b.scale = 19;
            //TestMultiply(a, b); // 96

            //a = 86;
            //b = 1;
            //a.scale = 10;
            //b.scale = 19;
            //TestMultiply(a, b); // 85

            //a = 85;
            //b = 1;
            //a.scale = 10;
            //b.scale = 19;
            //TestMultiply(a, b); // 85 => 80 (even)

            //a = 84;
            //b = 1;
            //a.scale = 10;
            //b.scale = 19;
            //TestMultiply(a, b); // 85

        }

        private void TestMultiply(decimal a, decimal b)
        {
            throw new NotImplementedException();
        }


//        void AdditionalTests(const int &iterations)
//{
//    // Additional tests
//    List<decimal> numbers = new List(2 * (DEC_SCALE_MAX + 1) * 96);
//    BYTE signs[] = { 0, DECIMAL_NEG };
//        const int bitsteps = 3;
//        const uint32_t bitmask = (1 << (bitsteps + 1)) - 1;

//    for (BYTE sign : signs)
//	{
//		DECIMAL current;
//        current.sign = sign;
//for (int scale = 0; scale <= DEC_SCALE_MAX; scale += 3)
//{
//    current.scale = (BYTE) scale;
//        current.Low64 = MAXDWORD64;
//    current.High = 0;

//    //for (int bit = 0; bit < 96; ++bit)
//    for (int bit = 0; bit< 32; bit += bitsteps)
//    {
//        if (current.Low64 == MAXDWORD64)
//            current.High = (current.High << bitsteps) | bitmask;
//        current.Low64 = (current.Low64 << bitsteps) | bitmask;
//        numbers.push_back(current);
//    }
//}
//	}
//	vector<DECIMAL> expected(numbers.Length* numbers.Length), actual(numbers.Length * numbers.Length);
//vector<HRESULT> expected_res(numbers.Length* numbers.Length), actual_res(numbers.Length * numbers.Length);

//#if WRITE_TEST_CODE
//	FILE* mul = fopen("compare_mul.cpp", "w");
//	FILE* add = fopen("compare_add.cpp", "w");
//	FILE* div = fopen("compare_div.cpp", "w");
//	for (int i = 0; i < numbers.Length; i++)
//	{
//		for (int j =i; j < numbers.Length; j += 3)
//		{
//			WriteOp(mul, numbers[i], numbers[j], "DecimalMul", DecimalMul);
//			WriteOp(add, numbers[i], numbers[j], "DecimalAdd", DecimalAdd);
//			WriteOp(div, numbers[i], numbers[j], "DecimalDiv", DecimalDiv);
//		}
//	}
//	fclose(mul);
//	fclose(add);
//	fclose(div);
//#endif

//#if TEST_MULTIPLY
//#if COMPARE_OLEAUT
//compare_benchmark("VarDecMul all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecMul,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalMul
//	);
//#endif // COMPARE_OLEAUT
//#if COMPARE_CORECLR
//compare_benchmark("VarDecMul all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecMul_PALRT,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalMul
//	);
//#endif // COMPARE_CORECLR
//#endif

//#if TEST_ADD
//#if COMPARE_OLEAUT
//compare_benchmark("VarDecAdd all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecAdd,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalAdd
//	);
//#endif // COMPARE_OLEAUT
//#if COMPARE_CORECLR
//compare_benchmark("VarDecAdd all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecAdd_PALRT,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalAdd
//	);
//#endif // COMPARE_CORECLR
//#endif

//#if TEST_SUB
//#if COMPARE_OLEAUT
//compare_benchmark("VarDecSub all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecSub,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalSub
//	);
//#endif // COMPARE_OLEAUT
//#if COMPARE_CORECLR
//compare_benchmark("VarDecSub all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecSub_PALRT,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalSub
//	);
//#endif // COMPARE_CORECLR
//#endif

//#if TEST_DIV
//#if COMPARE_OLEAUT
//compare_benchmark("VarDecDiv all 0..111 patterns for all signs and scales", "oleaut", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecDiv,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalDiv
//	);
//#endif // COMPARE_OLEAUT
//#if COMPARE_CORECLR
//compare_benchmark("VarDecDiv all 0..111 patterns for all signs and scales", "palrt", platform, iterations, numbers,
//    numbers, expected, expected_res, actual, actual_res,
//    (HRESULT(STDAPICALLTYPE *)(const DECIMAL*, const DECIMAL*, DECIMAL*))VarDecDiv_PALRT,
//		(HRESULT(STDAPICALLTYPE *)(const DECIMAL *, const DECIMAL *, DECIMAL *))DecimalDiv
//	);
//#endif // COMPARE_CORECLR
//#endif

//}

    }
}

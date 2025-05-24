using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using Benchmarks.ClrClassLibrary;

namespace Benchmarks
{
    public struct Accounts_Decimal
    {
        public decimal NewInterest;
        public decimal CurrentInterest;
        public decimal CurrentInterestRate;
        public decimal Amount;
        public decimal DaysInYear;
    }

    //[InProcess]
    public class InterestBenchmark
    {
        public Accounts_Decimal[] _decAccounts;

        //[Params(100, 100000)]
        [Params(100000)]
        public int Count { get; set; }
        [Params(true, false)]
        public bool RoundedAmounts = true;
        [Params(true, false)]
        public bool SmallDivisor = true;
        public decimal days = 1m;

        [GlobalSetup]
        public void Setup()
        {
            var rand = new Random(321);
            _decAccounts = new Accounts_Decimal[Count];
            var days = new decimal[] { 360, 360, 360, 360, 365, 365, 365, 365, 366 };

            var decimals = new int[] { 2, 2, 2, 2, 3, 3, 5, 10, 0 };

            for (int i = 0; i < Count; ++i)
            {
                var rounding = decimals[rand.Next(0, decimals.Length)];

                _decAccounts[i] = new Accounts_Decimal()
                {
                    // interest from -
                    CurrentInterest = Random(rand, -200_000m, 200_000m),
                    CurrentInterestRate = Random(rand, -2m, 10m),
                    Amount = Random(rand, -200_000_000m, 200_000_000m),
                    DaysInYear = days[rand.Next(0, days.Length - 1)]
                };

                if (RoundedAmounts)
                {
                    // interest from -
                    _decAccounts[i].CurrentInterest = Math.Round(_decAccounts[i].CurrentInterest, rounding);
                    _decAccounts[i].CurrentInterestRate = Math.Round(_decAccounts[i].CurrentInterestRate, 5);
                    _decAccounts[i].Amount = Math.Round(_decAccounts[i].Amount, rounding);
                }

                if (!SmallDivisor)
                {
                    _decAccounts[i].DaysInYear /= 3m;
                }
            }
        }

        private static decimal Random(Random rand, decimal min, decimal max)
        {
            var range = (max - min);
            var mid = (max + min) / 2m;

            return new decimal(rand.NextDouble()) * range - mid;
        }

        [Benchmark(Description = "System.Decimal .NET8")]
        public void NetFramework()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Methods.DivManaged(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Methods.AddManaged(array[i].CurrentInterest,
                        Methods.MulManaged(array[i].Amount, Methods.MulManaged(array[i].CurrentInterestRate, dayFactor)));
            }
        }

     //   [Benchmark(Description = "P/Invoke oleauto32")]
        public void Oleauto32()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Methods.DivOle32(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Methods.AddOle32(array[i].CurrentInterest,
                        Methods.MulOle32(array[i].Amount, Methods.MulOle32(array[i].CurrentInterestRate, dayFactor)));
            }
        }

     //   [Benchmark]
        public void New()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Managed.New.Decimal.Divide(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Managed.New.Decimal.Add(array[i].CurrentInterest,
                        Managed.New.Decimal.Multiply(array[i].Amount, Managed.New.Decimal.Multiply(array[i].CurrentInterestRate, dayFactor)));
            }
        }

     //   [Benchmark(Baseline = true)]
        public void Main()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Managed.Main.Decimal.Divide(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Managed.Main.Decimal.Add(array[i].CurrentInterest,
                        Managed.Main.Decimal.Multiply(array[i].Amount, Managed.Main.Decimal.Multiply(array[i].CurrentInterestRate, dayFactor)));
            }
        }

    }
}

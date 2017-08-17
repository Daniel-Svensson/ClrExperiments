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

    public struct Accounts_CoreRT2
    {
        public CoreRT.Decimal2 NewInterest;
        public CoreRT.Decimal2 CurrentInterest;
        public CoreRT.Decimal2 CurrentInterestRate;
        public CoreRT.Decimal2 Amount;
        public CoreRT.Decimal2 DaysInYear;
    }

    public class InterestBenchmark
    {
        public Accounts_Decimal[] _decAccounts;
        public Accounts_CoreRT2[] _crt2Accounts;

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
            _crt2Accounts = new Accounts_CoreRT2[Count];
            var days = new decimal[] { 360, 360, 360, 360, 365, 365, 365, 365, 366 };

            var decimals = new int[] { 2, 2, 2, 2, 3, 3, 5, 10, 0 };

            for (int i = 0; i < Count; ++i)
            {
                var rounding = decimals[rand.Next(0, decimals.Length - 1)];

                _decAccounts[i] = new Accounts_Decimal()
                {
                    // interest from -
                    CurrentInterest = Random(rand, -100_000_000_000m, 100_000_000_000m),
                    CurrentInterestRate = Random(rand, -2m, 10m),
                    Amount = Random(rand, -100_000_000_000_000m, 100_000_000_000_000m),
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

                _crt2Accounts[i] = new Accounts_CoreRT2()
                {
                    CurrentInterest = _decAccounts[i].CurrentInterest,
                    CurrentInterestRate = _decAccounts[i].CurrentInterestRate,
                    Amount = _decAccounts[i].Amount,
                    DaysInYear = _decAccounts[i].DaysInYear,
                };
            }
        }

        private static decimal Random(Random rand, decimal min, decimal max)
        {
            var range = (max - min);
            var mid = (max + min) / 2;

            return new decimal(rand.NextDouble()) * range - mid;
        }

        [Benchmark]
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

        [Benchmark]
        public void New()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Methods.DivNative(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Methods.AddNative(array[i].CurrentInterest,
                        Methods.MulNative(array[i].Amount, Methods.MulNative(array[i].CurrentInterestRate, dayFactor)));
            }
        }

        [Benchmark]
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

        [Benchmark]
        public void CoreRT2()
        {
            var array = _crt2Accounts;

            for (int i = 0; i < _crt2Accounts.Length; ++i)
            {
                var dayFactor = Methods.DivCoreRTManaged(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Methods.AddCoreRTManaged(array[i].CurrentInterest,
                        Methods.MulCoreRTManaged(array[i].Amount, Methods.MulCoreRTManaged(array[i].CurrentInterestRate, dayFactor)));
            }
        }


        [Benchmark]
        public void PInvokeDummy()
        {
            var array = _decAccounts;

            for (int i = 0; i < array.Length; ++i)
            {
                var dayFactor = Methods.DivNoop(days, array[i].DaysInYear);

                array[i].NewInterest =
                    Methods.AddNoop(array[i].CurrentInterest,
                        Methods.MulNoop(array[i].Amount, Methods.MulNoop(array[i].CurrentInterestRate, dayFactor)));
            }
        }
    }
}

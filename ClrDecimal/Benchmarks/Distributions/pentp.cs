using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Benchmarks.Distributions
{
    public class AddPentP : ArrayAdd
    {
        public AddPentP()
            : this(LoadFile())
        {

        }
        private AddPentP(decimal[] numbers)
            : base(numbers, numbers)
        {

        }

        public static decimal[] LoadFile()
        {
            var numbers = File.ReadAllLines("Distributions\\decimals.txt")
                .Select(l => decimal.Parse(l, CultureInfo.InvariantCulture))
                .ToList();

            // Randomize array
            var result = new decimal[numbers.Count];
            var rnd = new Random(42);
            for (int i =0; i < result.Length; ++i)
            {
                var from = rnd.Next(0, numbers.Count - 1);
                result[i] = numbers[from];
                numbers.RemoveAt(from);
            }

            return result;
        }
    }

    public class MulPentP : ArrayMul
    {
        public MulPentP()
            : this(AddPentP.LoadFile())
        {

        }
        private MulPentP(decimal[] numbers)
            : base(numbers, numbers)
        {

        }
    }

    public class DivPentP : ArrayDiv
    {
        public DivPentP()
            : this(AddPentP.LoadFile().Where(d => d != decimal.Zero).ToArray())
        {

        }

        private DivPentP(decimal[] numbers)
            : base(numbers, numbers)
        {

        }
    }
}

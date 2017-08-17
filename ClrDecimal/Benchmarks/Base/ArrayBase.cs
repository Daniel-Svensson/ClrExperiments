using System.Linq;

namespace Benchmarks
{
    public class ArrayBase
    {
        protected readonly decimal[] lhs_builtin;
        protected readonly decimal[] rhs_builtin;
        protected readonly decimal[] res_builtin;
        protected readonly CoreRT.Decimal[] lhs_corert;
        protected readonly CoreRT.Decimal[] rhs_corert;
        protected readonly CoreRT.Decimal[] res_corert;
        protected readonly CoreRT.Decimal2[] lhs_corert2;
        protected readonly CoreRT.Decimal2[] rhs_corert2;
        protected readonly CoreRT.Decimal2[] res_corert2;

        public ArrayBase(decimal[] lhs, decimal[] rhs)
        {
            lhs_builtin = lhs;
            rhs_builtin = rhs;

            if (object.ReferenceEquals(lhs, rhs))
                rhs_builtin = rhs.Select(d => d).ToArray();

            res_builtin = new decimal[lhs.Length * rhs.Length];

            lhs_corert = lhs.Select(x => (CoreRT.Decimal)x).ToArray();
            rhs_corert = rhs.Select(x => (CoreRT.Decimal)x).ToArray();
            res_corert = new CoreRT.Decimal[lhs.Length * rhs.Length];

            lhs_corert2 = lhs.Select(x => (CoreRT.Decimal2)x).ToArray();
            rhs_corert2 = rhs.Select(x => (CoreRT.Decimal2)x).ToArray();
            res_corert2 = new CoreRT.Decimal2[lhs.Length * rhs.Length];
        }

    }
}

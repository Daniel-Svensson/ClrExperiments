using System.Linq;

namespace Benchmarks
{
    public class ArrayBase
    {
        protected readonly decimal[] lhs_builtin;
        protected readonly decimal[] rhs_builtin;
        protected readonly decimal[] res_builtin;
        protected readonly Managed.New.Decimal[] lhs_corert;
        protected readonly Managed.New.Decimal[] rhs_corert;
        protected readonly Managed.New.Decimal[] res_corert;
        //protected readonly Managed.New.Decimal[] lhs_corert2;
        //protected readonly Managed.New.Decimal] rhs_corert2;
        //protected readonly Managed.New.Decimal2[] res_corert2;

        public ArrayBase(decimal[] lhs, decimal[] rhs)
        {
            lhs_builtin = lhs;
            rhs_builtin = rhs;

            if (object.ReferenceEquals(lhs, rhs))
                rhs_builtin = rhs.Select(d => d).ToArray();

            res_builtin = new decimal[lhs.Length * rhs.Length];

            lhs_corert = lhs.Select(x => (Managed.New.Decimal)x).ToArray();
            rhs_corert = rhs.Select(x => (Managed.New.Decimal)x).ToArray();
            res_corert = new Managed.New.Decimal[lhs.Length * rhs.Length];

            //lhs_corert2 = lhs.Select(x => (Managed.New.Decimal2)x).ToArray();
            //rhs_corert2 = rhs.Select(x => (Managed.New.Decimal2)x).ToArray();
            //res_corert2 = new Managed.New.Decimal2[lhs.Length * rhs.Length];
        }

    }
}

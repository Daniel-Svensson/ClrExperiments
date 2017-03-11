using System;
using System.Linq;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Horology;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Environments;

namespace Benchmarks
{
    public class BenchmarkConfig : ManualConfig
    {
        public BenchmarkConfig()
        {


#if CORE
            AddJobs(new[] { Job.RyuJitX64});
            Console.WriteLine("Changed Runtime to Core");
#else
            AddJobs(new[] { Job.RyuJitX64, Job.LegacyJitX86, Job.LegacyJitX64 });
#endif
        }

        private void AddJobs(Job[] jobs)
        {
            foreach (var job in jobs)
            {
                Add(job.WithLaunchCount(1)
                    .WithIterationTime(TimeInterval.FromMilliseconds(IterationTime))
                    .WithWarmupCount(3)
                    .WithTargetCount(Iterations)
#if CORE
                    .With(Runtime.Core)
#endif
                    );
                
            }
        }

        public const int IterationTime = 1000;
        public const int Iterations = 3;
    }
}

using System;
using System.Linq;
using BenchmarkDotNet.Columns;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Horology;
using BenchmarkDotNet.Jobs;

namespace Benchmarks
{
    public class BenchmarkConfig : ManualConfig
    {
        public BenchmarkConfig()
        {
            AddJobs(new[] { Job.RyuJitX64, Job.LegacyJitX86, Job.LegacyJitX64 });
            //Add(StatisticColumn.Max);
        }

        private void AddJobs(Job[] jobs)
        {
            foreach (var job in jobs)
            {
                Add(job.WithLaunchCount(1)
                    .WithIterationTime(TimeInterval.FromMilliseconds(IterationTime))
                    .WithWarmupCount(3)
                    .WithTargetCount(Iterations)
                    //.With(JobMode.Throughput)
                    );
            }
        }

        public const int IterationTime = 1000;
        public const int Iterations = 3;
        public const NBench.RunMode RunMode = NBench.RunMode.Throughput;
        public const NBench.TestMode TestMode = NBench.TestMode.Measurement;
    }
}

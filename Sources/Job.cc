#include <Yolo/Job.h>
#include <Yolo/Array.h>

namespace JobOps
{
    Array<Job> jobs;

    void Start(Job job)
    {
        ArrayOps::Push(&jobs, job);
    }

    void Start(Function<void(void)> job)
    {
        Start(Job{ job });
    }

    void UpdateJobs(void)
    {
        int n = jobs.count;
        for (int i = 0; i < n; i++)
        {
            Job job = jobs.elements[i];
            job.executor();
        }

        if (n == jobs.count)
        {
            ArrayOps::Clear(&jobs);
        }
        else
        {
            ArrayOps::Erase(&jobs, 0, n);
        }
    }
}

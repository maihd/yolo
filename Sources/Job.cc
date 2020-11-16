#include <Yolo/Job.h>
#include <Yolo/Array.h>

namespace JobOps
{
    Array<Job> jobs;

    void Start(Job job)
    {
        ArrayPush(&jobs, job);
    }

    void Start(void* data, void (*execute)(void* data))
    {
        Start(Job{ data, execute });
    }

    void UpdateJobs(void)
    {
        int n = jobs.count;
        for (int i = 0; i < n; i++)
        {
            Job job = jobs.elements[i];
            job.Execute(job.Data);
        }

        if (n == jobs.count)
        {
            ArrayClear(&jobs);
        }
        else
        {
            ArrayErase(&jobs, 0, n);
        }
    }
}

#pragma once

#include <Yolo/Types.h>

namespace JobOps
{
    void Start(Job                    job);
    void Start(Function<void(void)>   job);

    void UpdateJobs(void);
}


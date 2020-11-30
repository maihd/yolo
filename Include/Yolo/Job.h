#pragma once

#include <Yolo/Core.h>

namespace JobOps
{
    void Start(Job job);
    void Start(void* data, void (*execute)(void* data));

    void UpdateJobs(void);
}


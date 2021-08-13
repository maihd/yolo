#pragma once

#include <System/Core.h>

void StartJob(Job job);
void StartJob(void* data, void (*execute)(void* data));

void UpdateJobs(void);

#pragma once

#include "stdafx.h"

class TaskSchedulerTask
{
public:
	TaskSchedulerTask();
	~TaskSchedulerTask();

  bool Create(LPCWSTR taskName, LPCWSTR program, TASK_TRIGGER_TYPE2 flags);
};


#ifndef LOS2FREERTOS_TASK_H
#define LOS2FREERTOS_TASK_H

#include <stdint.h>
#include "LOS2FreeRTOS/types.h"

typedef void* (*TSK_ENTRY_FUNC)(uint32_t arg);

typedef struct tagTskInitParam {
	TSK_ENTRY_FUNC pfnTaskEntry;
	uint16_t usTaskPrio;
	uint32_t uwArg;
	UINTPTR  stackAddr;
	uint32_t uwStackSize;
	char* pcName;
	uint32_t uwResved;
} TSK_INIT_PARAM_S;

// extern UINT32 LOS_TaskCreate(UINT32 *taskID, TSK_INIT_PARAM_S *taskInitParam);
uint32_t LOS_TaskCreate(UINT32 *task_id, TSK_INIT_PARAM_S *task_init_param);

// extern UINT32 LOS_TaskDelay(UINT32 tick);
uint32_t LOS_TaskDelay(uint32_t tick);

#endif // LOS2FREERTOS_TASK_H
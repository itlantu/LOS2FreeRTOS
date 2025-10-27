#include "LOS2FreeRTOS/task.h"
#include <malloc.h>

#include "FreeRTOS.h"
#include "task.h"


struct TaskArgs{
	uint32_t args;
	TSK_ENTRY_FUNC func;
};

configSTACK_DEPTH_TYPE to_stack_depth(const uint32_t bytes) {
	// 字节转字
	configSTACK_DEPTH_TYPE result = bytes / 4;
	if (result < configMINIMAL_STACK_SIZE)
		result = configMINIMAL_STACK_SIZE;
	return result;
}

static void v_task(void* task_args_ptr) {
	const struct TaskArgs* task_args = (const struct TaskArgs*)task_args_ptr;
	const TSK_ENTRY_FUNC func = task_args->func;
	const uint32_t args = task_args->args;
	// 回收内存
	free(task_args_ptr);
	// 执行任务函数
	func(args);
}

uint32_t LOS_TaskCreate(UINT32 *task_id, TSK_INIT_PARAM_S *task_init_param) {
	TaskHandle_t handle;
	const configSTACK_DEPTH_TYPE stack_depth = to_stack_depth(task_init_param->stackAddr);
	// 从内存中分配空间存储TaskArgs
	struct TaskArgs *task_args = (struct TaskArgs*)malloc(sizeof(struct TaskArgs));
	task_args->args = task_init_param->uwArg;
	task_args->func = task_init_param->pfnTaskEntry;
	// 使用FreeRTOS创建任务
	xTaskCreate(
		v_task,
		task_init_param->pcName,
		stack_depth,
		task_args,
		task_init_param->usTaskPrio,
		&handle);
	// 返回任务是否创建成功
	if (handle == NULL)
		// 任务创建失败
		return LOS_NOK;
	return LOS_OK;
}

uint32_t LOS_TaskDelay(const uint32_t tick) {
	vTaskDelay(tick);
	return LOS_OK;
}
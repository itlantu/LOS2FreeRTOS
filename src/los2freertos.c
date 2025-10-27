#include "LOS2FreeRTOS/los2freertos.h"
#include "task.h"

uint32_t LOS_MS2Tick(const uint32_t milli_second) {
	return pdMS_TO_TICKS(milli_second);
}

uint32_t LOS_Start() {
	vTaskStartScheduler();
	return LOS_OK;
}

uint32_t LOS_KernelInit() {
	return LOS_OK;
}
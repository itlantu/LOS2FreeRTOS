#ifndef LOS2FREERTOS_H
#define LOS2FREERTOS_H

#include "FreeRTOS.h"
#include "LOS2FreeRTOS/types.h"

// extern UINT32 LOS_MS2Tick(UINT32 millisec);
uint32_t LOS_MS2Tick(const uint32_t milli_second);

// extern UINT32 LOS_Start(VOID)
uint32_t LOS_Start();

// extern UINT32 LOS_KernelInit(VOID)
uint32_t LOS_KernelInit();

#endif // LOS2FREERTOS_H
#ifndef LOS2FREERTOS_QUEUE_H
#define LOS2FREERTOS_QUEUE_H

#include "LOS2FreeRTOS/types.h"

#define LOS2FREERTOS_CONFIG_QUEUE_MAX_NUM 8

/* extern UINT32 LOS_QueueCreate(const CHAR *queueName,
							  UINT16 len,
							  UINT32 *queueID,
							  UINT32 flags,
							  UINT16 maxMsgSize);*/
uint32_t LOS_QueueCreate(const char* name, uint16_t length, uint32_t* queue_id, uint32_t flags, uint16_t max_msg_size);

/* extern UINT32 LOS_QueueWriteCopy(UINT32 queueID,
								 VOID *bufferAddr,
								 UINT32 bufferSize,
								 UINT32 timeOut);*/
uint32_t LOS_QueueWriteCopy(uint32_t queue_id, void* buffer,uint32_t buffer_size, uint32_t timeout);

/* extern UINT32 LOS_QueueReadCopy(UINT32 queueID,
                                VOID *bufferAddr,
                                UINT32 *bufferSize,
                                UINT32 timeOut); */
uint32_t LOS_QueueReadCopy(uint32_t queue_id, void* buffer, uint32_t* buffer_size, uint32_t timeout);

#endif // LOS2FREERTOS_QUEUE_H

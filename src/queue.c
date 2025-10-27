#include "LOS2FreeRTOS/queue.h"
#include <malloc.h>

#include "FreeRTOS.h"
#include "queue.h"

char queue_handle_table_init_flag = 0;

/**
 * QueueHandleTable结构体
 *	use_flag: 标志位, 每一位表示table中对应的元素是否在使用
 *	table: 存储FreeRTOS的Queue句柄
 */
struct {
	uint32_t use_flag;
	QueueHandle_t table[LOS2FREERTOS_CONFIG_QUEUE_MAX_NUM];
}QueueHandleTable;

typedef struct {
	uint32_t size;
	void* data;
}DynamicMessage;

/**
 * 尝试初始化QueueHandleTable，若已初始化(标志位为1)则返回
 */
inline void queue_handle_table_check_init() {
	if (queue_handle_table_init_flag)
		return;
	queue_handle_table_init_flag = 1;
	QueueHandleTable.use_flag = 0;
}

/**
 * 根据ID来获取FreeRTOS中消息队列对应的的句柄
 * @param queue_id 消息队列ID
 * @return 如果消息队列存在, 则返回句柄对应的指针, 若消息队列不存在则返回空指针
 */
QueueHandle_t* queue_handle_table_get(const uint32_t queue_id) {
	queue_handle_table_check_init();
	if (queue_id >= LOS2FREERTOS_CONFIG_QUEUE_MAX_NUM)
		return NULL;
	return &QueueHandleTable.table[queue_id];
}

/**
 * 在QueueHandleTable中分配一个可用的Queue句柄
 * @param queue_id 消息队列ID
 * @return 返回0表示分配成功; 为1表示QueueHandleTable中没有空闲的可分配句柄, 即在使用的队列已经超过设置的最大上限
 */
int queue_handle_table_create(uint32_t* queue_id) {
	queue_handle_table_check_init();
	uint32_t use_flag = QueueHandleTable.use_flag;

	// 根据标志位查找是否有空余分配位置
	for (int i = 0; i < LOS2FREERTOS_CONFIG_QUEUE_MAX_NUM; ++i, use_flag >>= 1) {
		if (use_flag & 1) {
			// 初始化值
			QueueHandleTable.table[i] = NULL;
			// 更新queue_id
			*queue_id = i;
			// 设置标志位对应的位数为1
			use_flag |= (1 << i);
			return 0;
		}
	}
	return 1;
}

/**
 * 将src拷贝到dst
 * @param dst 目标内存地址
 * @param src 源内存地址
 * @param bytes 拷贝的字节数
 */
void copy_src2dst(void* dst, void* src, uint32_t bytes) {
	uint32_t i = 0;
	// 先循环拷贝4字节数据
	int32_t* dst_32bit = (int32_t*)dst, *src_32bit = (int32_t*)src;
	for (const uint32_t size = bytes >> 2; i < size; ++i) {
		dst_32bit[i] = src_32bit[i];
	}

	// 循环拷贝剩余的字节
	int8_t* dst_8bit = (int8_t*)dst, *src_8bit = (int8_t*)src;
	for (uint32_t j = i << 2; j < bytes; ++j) {
		dst_8bit[j] = src_8bit[i];
	}
}

uint32_t LOS_QueueCreate(const char* name,
						 const uint16_t length,
						 uint32_t* queue_id,
						 uint32_t flags,
						 uint16_t max_msg_size) {
	// todo 实现max_msg_size
	if (queue_handle_table_create(queue_id))
		// 在使用的队列已经超过设置的最大上限
		return LOS_NOK;
	QueueHandle_t* handle = queue_handle_table_get(*queue_id);
	*handle = xQueueCreate(length, sizeof(DynamicMessage));
	return LOS_OK;
}

uint32_t LOS_QueueWriteCopy(uint32_t queue_id, void* buffer, const uint32_t* buffer_size, uint32_t timeout) {
	const QueueHandle_t * handle = queue_handle_table_get(queue_id);
	if (handle == NULL)
		return LOS_NOK;

	// 创建发送的消息
	const DynamicMessage message = {.data = malloc(*buffer_size), .size = *buffer_size};
	// 值拷贝
	copy_src2dst(message.data, buffer, *buffer_size);
	// 发送消息
	const BaseType_t status = xQueueSend(*handle, &message, timeout);
	if (status == pdPASS)
		return LOS_OK;
	// 发送失败, 释放内存
	free(message.data);
	return LOS_NOK;
}

uint32_t LOS_QueueReadCopy(const uint32_t queue_id, void* buffer, uint32_t* buffer_size, const uint32_t timeout) {
	const QueueHandle_t * handle = queue_handle_table_get(queue_id);
	if (handle == NULL)
		return LOS_NOK;

	// 接收消息
	DynamicMessage message;
	if (xQueueReceive(*handle, &message, timeout) != pdPASS)
		return LOS_NOK;

	// 拷贝到buffer
	copy_src2dst(buffer, message.data, message.size);
	// 释放内存
	free(message.data);

	return LOS_OK;
}

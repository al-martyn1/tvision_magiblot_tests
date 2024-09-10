#include "frameworkconfig.h"

#include "memsections.h"
#include "sdram.h"

#include "safe_malloc.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "memoryheap.h"
#include <atomic>


static uint32_t heapData[FW_HEAP_SIZE_IN_DWORDS] = { 0 }; 
CCMRAM static uint32_t heapCCMData[FW_CCM_HEAP_SIZE_IN_DWORDS] = { 0 };

extern const uint32_t heapStart = (uint32_t)&heapData;
extern const uint32_t heapEnd = (uint32_t)&heapData[FW_HEAP_SIZE_IN_DWORDS];

extern const uint32_t heapCCMStart = (uint32_t)&heapCCMData;
extern const uint32_t heapCCMEnd = (uint32_t)&heapCCMData[FW_CCM_HEAP_SIZE_IN_DWORDS];

static MemoryHeapSimple_t<uint16_t, 4, heapStart, heapEnd, FW_HEAP_FAST_STATISTICS_ENABLED> heap;
static MemoryHeapSimple_t<uint16_t, 4, heapCCMStart, heapCCMEnd, FW_HEAP_FAST_STATISTICS_ENABLED> heapCCM;

static FastMemoryHeap_t<FW_HEAP_FAST_STATISTICS_ENABLED> heapEXRAM;

// ==============================================================
const MemoryHeapStatistics_t & Heap::RamStatistics()
{
	return heap.mStatistics;
}

const MemoryHeapStatistics_t & Heap::CcmStatistics()
{
	return heapCCM.mStatistics;
}

const MemoryHeapStatistics_t & Heap::ExramStatistics()
{
	return heapEXRAM.mStatistics;
}

void * safe_malloc_ram(size_t __size)
{
	vTaskSuspendAll();
	std::atomic_thread_fence(std::memory_order_acquire);
	void * ptr = heap.Allocate(__size);
	//FWTHROW_IF(ptr==NULL, Result_OutOfMemory);
	std::atomic_thread_fence(std::memory_order_release);
	(void)xTaskResumeAll();
	return ptr;
}

void * safe_malloc_ccm(size_t __size)
{
	vTaskSuspendAll();
	std::atomic_thread_fence(std::memory_order_acquire);
	void * ptr = heapCCM.Allocate(__size);
	if (ptr== nullptr) ptr = heap.Allocate(__size); //try to allocate from common memory if CCM ram is not available
	//FWTHROW_IF(ptr==NULL, Result_OutOfMemory);
	std::atomic_thread_fence(std::memory_order_release);
	(void)xTaskResumeAll();
	return ptr;
}

void * safe_malloc_exram(size_t __size)
{
	vTaskSuspendAll();
	void * result = nullptr;

	if (!heapEXRAM.IsInitialized())
	{
		if (Sdram::IsInitialized())
		{
			heapEXRAM.Init(Sdram::StartAddress(), Sdram::Size());
		}
	}

	if (heapEXRAM.IsInitialized())
	{
		result = heapEXRAM.Allocate(__size);
	}

	(void)xTaskResumeAll();
	//if (result!=NULL) return result; 
	//return safe_malloc_ram(__size);
	return result;
}

void safe_free(void * __ptr)
{
	vTaskSuspendAll();

	std::atomic_thread_fence(std::memory_order_acquire);

	if ((uint32_t)__ptr >= heapStart && (uint32_t)__ptr < heapEnd)
	{
		heap.Deallocate(__ptr);
	}
	else if ((uint32_t)__ptr >= heapCCMStart && (uint32_t)__ptr<heapCCMEnd)
	{
		heapCCM.Deallocate(__ptr);
	}
	else if (heapEXRAM.ContainsPointer(__ptr))
	{
		heapEXRAM.Deallocate(__ptr);
	}
	else FWTHROW(Result_InvalidPointer);

	std::atomic_thread_fence(std::memory_order_release);

	(void)xTaskResumeAll();
}

/* Переопределение _sbrk, чтобы контролировать выход за доступный диапазон памяти (библиотека по умолчанию не определяет этого) */

static bool _sbrk_is_valid_ptr(char * value) {
	const intptr_t x = reinterpret_cast<intptr_t>(value);
	if (x < 0x20000000) return false;
	if (x > 0x20030000) return false;
	return true;
}

extern "C"
char * _sbrk(int incr) {
	extern char end;
	char * volatile sbrk_heap_start = reinterpret_cast<char *>(&end);
	char * volatile sbrk_heap_end = reinterpret_cast<char *>(__get_MSP());// (char *)(&_estack);
	volatile int byteCount = incr;
	static char * sbrk_heap_ptr = nullptr;

	if (!_sbrk_is_valid_ptr(sbrk_heap_start) || !_sbrk_is_valid_ptr(sbrk_heap_end)) {
		asm("bkpt");
	}

	if (sbrk_heap_ptr == nullptr) {
		sbrk_heap_ptr = sbrk_heap_start;
	}
	char * prev_heap_ptr = sbrk_heap_ptr;
	char * volatile next_sbrk_heap_ptr = sbrk_heap_ptr + byteCount;
	if ((next_sbrk_heap_ptr <= sbrk_heap_end) && (prev_heap_ptr != nullptr)) {
		sbrk_heap_ptr = next_sbrk_heap_ptr;
		return prev_heap_ptr;
	}
	else {
		FWTHROW(Result_OutOfMemory);
		return reinterpret_cast<char *>(static_cast<intptr_t>(-1));
	}
}

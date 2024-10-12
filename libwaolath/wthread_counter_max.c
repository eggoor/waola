#include "pch.h"
#include "wthread_counter_max.h"

#define DEF_MAX_THREAD_COUNT 0x01000000

uint32_t calc_max_thread_count(void)
{
	uint32_t max_threads = DEF_MAX_THREAD_COUNT;
	
	switch (sizeof(void*))
	{
		case 4:
		{
			max_threads = 0x5DF;
			break;
		}
		default:
		{
			// Do nothing
		}
	}
	
	return max_threads;
}

/** @file warper_cb_data.h
* Data for ARP request callback.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef struct warper_cb_data {
	void* context;
	wthread_cbi_t* parentThreadCbi;
	warper_receive_cb cb;
	uint32_t ipAddr;
	const byte* macAddr;
	wcont4r_t* hostDiscovererList;
} warper_cb_data_t;

#if __cplusplus
}   // Extern C
#endif

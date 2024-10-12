/** @file warper_cb_data_t.h
* Forward declaration of Waola ARP request callback data.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

struct warper_cb_data;
typedef void (*warper_receive_cb)(struct warper_cb_data*);
typedef struct warper_cb_data warper_cb_data_t;

#if __cplusplus
}   // Extern C
#endif

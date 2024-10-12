/** @file warper_u.h
* Waola ARP client Unices-specific internal declarations.
*/

#pragma once

#include "include/wnet_iface.h"

#include "warper.h"

#define ARPPCKTSIZ (sizeof(struct ether_header) + sizeof(struct ether_arp))

#define ARP_REQ_COUNT 3
#define ARP_TIMEOUT_S 0
#define ARP_TIMEOUT_MS 650

int warper_get_file_descriptor(const warper_t* self);
void warper_set_file_descriptor(warper_t* self, int fd);
void warper_set_errno(warper_t* self, int err_no);
void process_arp_response(const warper_t* self, const struct ether_arp* const restrict ether_arp);
int warper_receive(const warper_t* const restrict felf);
void warper_prepare_req_buffer(const warper_t* self, const wnet_iface_t* const restrict nif);
int warper_do_send(const warper_t* self, uint32_t target_ip);
int warper_setup_communication_device(warper_t* self, const wnet_iface_t* const restrict nif);

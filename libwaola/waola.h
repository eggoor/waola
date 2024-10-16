/** @file waola.h
* Wake on LAN library.
*
* Broadcasts magic packet for each specified MAC
* address through all active network interfaces.
*/

#pragma once

#include "pch.h"

#include "include/libwaola.h"

static int waolam(const wcont4r_t* const restrict mac_addr_list,
	const wcont4r_t* const restrict iface_list);
static int waolah(const wcont4r_t* iface_list,
	whost_view_t* const restrict hosts[], size_t count);
static wcont4r_t* make_mac_addr_list_from_strings(const wcont4r_factory cont4r_factory,
	const char* const restrict hosts[], size_t size);
static struct sockaddr* get_dest_sockaddr(sa_family_t sa_family);
static int get_sockaddr_len(sa_family_t sa_family);
static int wake_all_macs(SOCKET s, const struct sockaddr* const restrict dest_sa,
	int dest_sa_len, const wcont4r_t* const restrict mac_addr_list,
	char* const restrict payload);
static int wake_all_hosts(SOCKET s, struct sockaddr* dest_sa, int dest_sa_len,
	whost_view_t* const restrict hosts[], size_t count, char* payload);
static void init_dest_addr(void);
static char* make_magic_packet(void);
static SOCKET make_socket(wnet_iface_t* const ni, sa_family_t sa_family);

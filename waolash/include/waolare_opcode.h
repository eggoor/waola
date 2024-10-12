/** @file waolare_opcode.h
* Waola resolver operation code.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * \enum Waola resolver operation code.
 */
typedef enum waolare_opcode {
	wre_undefined,
	wre_getaddrinfo			= 0x0001, /**< Gonna resolve hostname into IP address; provided data is a hostname */
	wre_gotaddrinfo			= 0x0002, /**< Successfully resolved hostname into IP address; provided data is a pointer to uint32_t representing IPv4 address in network byte order */
	wre_getaddrinfo_failed	= 0x0004, /**< IP address resolution failed; provided data is a hostname */
	wre_getnameinfo			= 0x0008, /**< Gonna resolve IP address into hostname; provided data is a pointer to uint32_t representing IPv4 address in network byte order */
	wre_gotnameinfo			= 0x0010, /**< Successfully resolved IP address into hostname; provided data is a hostname */
	wre_getnameinfo_failed	= 0x0020, /**< Hostname resolution failed; provided data is a pointer to uint32_t representing IPv4 address in network byte order */
	wre_sending_arp			= 0x0040, /**< Gonna resolve IP address into MAC address; provided data is a pointer to uint32_t representing IPv4 address in network byte order*/
	wre_got_arp				= 0x0080, /**< Successfully resolved IP address into MAC address; provided data is a pointer to uint32_t representing IPv4 address in network byte order */
	wre_arp_failed			= 0x0100, /**< IP address into MAC address resolution failed; provided data is a pointer to uint32_t representing IPv4 address in network byte order */
} waolare_opcode_t;

#if __cplusplus
}   // Extern C
#endif

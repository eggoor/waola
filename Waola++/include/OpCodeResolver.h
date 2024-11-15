/** @file OpCodeResolver.h
* Waola resolver operation codes.
*/

#pragma once

namespace Waola {
	/**
 	* \enum Waola resolver operation code.
	* Keep in sync with waolare_opcode_t
	*/
	enum OpCodeResolver {
		wre_undefined,
		wre_getaddrinfo =			0x0001,	/**< Provides hostname in UTF-8 encoding */
		wre_gotaddrinfo =			0x0002,	/**< Provides pointer to struct sockaddr */
		wre_getaddrinfo_failed =	0x0004,	/**< Provides hostname in UTF-8 encoding */
		wre_getnameinfo =			0x0008,	/**< Provides pointer to struct sockaddr */
		wre_gotnameinfo =			0x0010,	/**< Provides hostname in UTF-8 encoding */
		wre_getnameinfo_failed =	0x0020,	/**< Provides pointer to struct sockaddr */
		wre_sending_arp =			0x0040,	/**< Provides pointer to struct sockaddr */
		wre_got_arp =				0x0080,	/**< Provides pointer to struct sockaddr */
		wre_arp_failed =			0x0100	/**< Provides pointer to struct sockaddr */
	};
}

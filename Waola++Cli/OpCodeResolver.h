/** @file OpCodeResolver.h
* Waola resolver operation code.
*/

#pragma once

namespace WaolaCli {
	/**
 	* \enum waola resolver operation code.
	* Keep in sync with waolare_opcode_t
	*/
	public enum class OpCodeResolver {
		Undefined,
		GetAddrInfo = 0x0001,			/**< Provides hostname in UTF-8 encoding */
		GotAddrInfo = 0x0002,			/**< Provides pointer to struct sockaddr */
		GetAddrInfoFailed = 0x0004,		/**< Provides hostname in UTF-8 encoding */
		GetNameInfo = 0x0008,			/**< Provides pointer to struct sockaddr */
		GotNameInfo = 0x0010,			/**< Provides hostname in UTF-8 encoding */
		GetNameInfoFailed = 0x0020,		/**< Provides pointer to struct sockaddr */
		SendingArp = 0x0040,			/**< Provides pointer to struct sockaddr */
		GotArp = 0x0080,				/**< Provides pointer to struct sockaddr */
		ArpFailed = 0x0100				/**< Provides pointer to struct sockaddr */
	} waolare_opcode_t;
}
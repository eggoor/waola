#include "pch.h"

#include "include/arp_cache_entry.h"

#include "wnet_iface.h"
#include "waolane_fact4s.h"

#include "wnet_iface_prov.h"

static void log_item(struct sockaddr* sa);

void wnet_iface_prov_collect_ifaces(wnet_iface_prov_t* self)
{
	static const uint32_t DefaultOutBufferSize = 0x1000;

	PIP_ADAPTER_ADDRESSES adapterAddresses = NULL;

	if (!self->fact4s->make_cont4r) {
		wlog_if_level(wll_warning, "Network interface provider has no container factory \n");
		return;
	}

	if (!self->fact4s->make_net_iface) {
		wlog_if_level(wll_warning, "Network interface provider has no network interface factory \n");
		return;
	}

	self->ifList = (* self->fact4s->make_cont4r)(wct_list);

	if (!self->ifList) {
		wlog_if_level(wll_warning, "Unable to create network interface list\n");
		return;
	}

	self->collectionCancelled = FALSE;

	int rc = 0;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_UNSPEC;

	ULONG outBufferSize = DefaultOutBufferSize;
	int tries = 0;

	do {
		adapterAddresses = (IP_ADAPTER_ADDRESSES*)walloc_s(outBufferSize);
		if (!adapterAddresses) {
			wlog_if_level(wll_warning, "Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			return;
		}

		rc = GetAdaptersAddresses(family, flags, NULL, adapterAddresses, &outBufferSize);

		if (ERROR_BUFFER_OVERFLOW == rc) {
			wfree_s(adapterAddresses);
			adapterAddresses = NULL;
		}
		else {
			break;
		}
	} while (rc == ERROR_BUFFER_OVERFLOW
		&& tries++ < WDEF_TRY_COUNT
		&& !is_collection_net_ifaces_cancelled(self));

	if (NO_ERROR == rc) {
		PIP_ADAPTER_ADDRESSES currentAddresses = adapterAddresses;
		while (currentAddresses && !is_collection_net_ifaces_cancelled(self)) {
			char* adapterName = wstr_16_to_8_s(currentAddresses->FriendlyName);
			wlog_if_level(wll_debug, "Found network adapter '%s' ('%s')\n",
				adapterName, currentAddresses->AdapterName);

			if (currentAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK
				&& currentAddresses->OperStatus == IfOperStatusUp
				&& (currentAddresses->Ipv4Enabled || currentAddresses->Ipv6Enabled)) {

				PIP_ADAPTER_UNICAST_ADDRESS unicastAddress = currentAddresses->FirstUnicastAddress;

				if (unicastAddress) {
					while (unicastAddress) {
						ULONG mask = 0;

						if (AF_INET == unicastAddress->Address.lpSockaddr->sa_family
							&& ConvertLengthToIpv4Mask(unicastAddress->OnLinkPrefixLength, &mask) != NO_ERROR) {
							wlog_if_level(wll_warning, "ConvertLengthToIpv4Mask failed\n");
						};

						wnet_iface_t* netIface = (* self->fact4s->make_net_iface)(self->fact4s,
							currentAddresses->IfIndex, adapterName ? adapterName : currentAddresses->AdapterName,
							unicastAddress->Address.lpSockaddr,	mask, NULL, NULL);

						if (netIface) {
							BOOL added = wcont4r_add_s(self->ifList, netIface);

							if (added)
							{
								if (wlog_get_level() >= wll_debug) {
									wlog_if_level(wll_debug, "\tAdded network adapter '%s' ('%s')\n",
										adapterName, currentAddresses->AdapterName);
									SOCKADDR* sockAddr = unicastAddress->Address.lpSockaddr;
									log_item(sockAddr);
								}
							}
							else {
								wlog_if_level(wll_warning, "Unable to make network interface	'%s' ('%s') list item\n",
									adapterName, currentAddresses->AdapterName);
								wnet_iface_free(netIface);
							}
						}
						else {
							wlog_if_level(wll_error, "\tNot enough memory for network interface '%s' ('%s')\n",
								adapterName, currentAddresses->AdapterName);
						}

						unicastAddress = unicastAddress->Next;
					}
				}
				else {
					wlog_if_level(wll_verbose, "\tNo unicast addresses found on adapter '%s' ('%s')\n",
						adapterName, currentAddresses->AdapterName);
				}
			}
			else {
				wlog_if_level(wll_verbose, "\tIgnoring network adapter '%s' ('%s')\n",
					adapterName, currentAddresses->AdapterName);
			}

			wfree_s(adapterName);
			currentAddresses = currentAddresses->Next;
		}
	}
	else {
		wlog_if_level(wll_error, "Call to GetAdaptersAddresses failed with error: %d\n", rc);
		if (ERROR_NO_DATA == rc) {
			wlog_if_level(wll_warning, "No addresses were found for the %d family and 0x%04x flags\n",
				family, flags);
		}
		else {
			LPWSTR messageBuffer = NULL;
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, rc, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL)) {
				wlog_if_level(wll_error, "\tError: %s\n", messageBuffer);
				LocalFree(messageBuffer);
			}
		}
	}

	wfree_s(adapterAddresses);
}

static void log_item(struct sockaddr* sa)
{
	switch (sa->sa_family) {
	case AF_INET:
	case AF_INET6: {
		char buffer[INET6_ADDRSTRLEN];
		wsockaddr_to_string_s(sa, buffer, INET6_ADDRSTRLEN);
		wlog_if_level(wll_verbose, "\t\t, IP address %s\n", buffer);
		break;
	}
	default: {
		wlog_if_level(wll_verbose, "\t\t, its address family %d is neither AF_INET nor AF_INET6\n",
			sa->sa_family);
	}
	}
}

BOOL wnet_iface_prov_process_cache(wcont4r_t* arp_entries)
{
	BOOL res = FALSE;

	PMIB_IPNETTABLE table;
	ULONG size = 0;
	ULONG retVal = GetIpNetTable(NULL, &size, FALSE);

	if (NO_ERROR != retVal && ERROR_INSUFFICIENT_BUFFER != retVal) {
		return res;
	}

	table = walloc_s(size);

	if (!table) {
		wlog_if_level(wll_warning, "No %zuB of free memory for MIB_IPNETTABLE\n",
			size);
		return res;
	}

	retVal = GetIpNetTable(table, &size, FALSE);

	if (NO_ERROR != retVal && ERROR_NO_DATA != retVal) {
		goto cleanup;
	}

	for (int i = table->dwNumEntries; i --> 0;) {
		PMIB_IPNETROW_LH rt = &table->table[i];
		if (ETHER_ADDR_LEN == rt->dwPhysAddrLen)
		{
			arp_cache_entry_t* ce = walloc_s(sizeof(struct arp_cache_entry));
			if (!ce) {
				wlog_if_level(wll_warning, "No %zuB of free memory for cache entrie\n",
					sizeof(struct arp_cache_entry));
				continue;
			}
			ce->ipAddr = rt->dwAddr;
			memcpy(ce->macAddr, rt->bPhysAddr, ETHER_ADDR_LEN);
			res |= wcont4r_add_s(arp_entries, ce);
		}
	}

cleanup:
	wfree_s(table);

	return res;
}
//
//  HostViewModel.swift
//  WaolaS
//
//  Created by e on 16.09.2024.
//

import Foundation

class HostViewModel: Identifiable, Equatable {
	let hostView: OpaquePointer!
	
	var displayName: String {
		whost_view_set_field_change_processed(hostView, cf_display_name)
		let cVal = whost_view_get_display_name(hostView)
		let val = (cVal != nil) ? String(cString: cVal!) : "";
		return val;
	}
	
	var hostname: String? {
		whost_view_set_field_change_processed(hostView, cf_host_name)
		let cVal = whost_view_get_hostname(hostView)
		return (cVal != nil) ? String(cString: cVal!) : ""
	}
	
	var ipAddress: String? {
		whost_view_set_field_change_processed(hostView, cf_ip_address)
		let cVal = whost_view_get_ip_addr(hostView)
		return (cVal != nil) ? String(cString: cVal!) : ""
	}
	
	var macAddress: String? {
		whost_view_set_field_change_processed(hostView, cf_mac_address)
		let cVal = whost_view_get_mac_addr(hostView)
		return (cVal != nil) ? String(cString: cVal!) : ""
	}
	
	var lastSeenOnline: Date {
		whost_view_set_field_change_processed(hostView, cf_last_seen_online)
		let host = whost_view_get_host(hostView)
		return Date(timeIntervalSince1970: Double(whost_get_last_seen_online(host)))
	}
	
	var wakeupResult: String? {
		whost_view_set_field_change_processed(hostView, cf_op_result)
		let cVal = whost_view_get_op_result(hostView)
		return (cVal != nil) ? String(cString: cVal!) : ""
	}
	
	init (hostView: OpaquePointer!) {
		self.hostView = hostView
	}
	
	func getData() -> HostData {
		let exactDisplayName = whost_view_get_exact_display_name(hostView) ?? nil
		let displayName = (exactDisplayName != nil) ? String(cString: exactDisplayName!) : nil
		return HostData(displayName: displayName, hostname: hostname, ipAddress: ipAddress, macAddress: macAddress)
	}
	
	func toHostModel() -> HostModel {
		let exactDisplayName = whost_view_get_exact_display_name(hostView) ?? nil
		let displayName = (exactDisplayName != nil) ? String(cString: exactDisplayName!) : nil
		return HostModel(displayName: displayName, hostname: hostname, ipAddress: ipAddress, macAddress: macAddress, lastSeenOnline: lastSeenOnline)
	}
	
	func toString() -> String {
		return "\(macAddress.unwrapOrEmpty)\t\(ipAddress.unwrapOrEmpty)\t\(hostname.unwrapOrEmpty)";
	}
	
	static func == (lhs: HostViewModel, rhs: HostViewModel) -> Bool {
		return whost_view_compare(lhs.hostView, rhs.hostView) == 0
	}
}

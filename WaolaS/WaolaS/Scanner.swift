//
//  Scanner.swift
//  WaolaS
//
//  Created by e on 13.09.2024.
//

import Foundation

public class Scanner {
	var scanner = wmake_waolasc_def(nil, nil)
	var stateSubscribeData: UnsafeMutablePointer<waolasc_subscribe_data_t>?
	var vaultSubscribeData: UnsafeMutablePointer<waolava_subscribe_data_t>?
	var viewModel: ContentView.ContentViewModel?
	
	public init() {
		viewModel = nil
		subscribe()
	}
	
	deinit {
		unsubscribe()
		waolasc_free(scanner)
	}
	
	func setViewModel(viewModel: ContentView.ContentViewModel) {
		self.viewModel = viewModel
	}
	
	func scanNetwork() {
		waolasc_discover_async(scanner)
	}
	
	func refresh() {
		waolasc_refresh_async(scanner)
	}
	
	func onStateEvent(tasks: wtask_t) {
		viewModel?.onStateEvent(tasks: tasks)
	}
	
	func onVaultEvent(opCode: waolava_op_t, hostView: OpaquePointer!) {
		viewModel?.onVaultEvent(opCode: opCode, hostView: hostView)
	}
	
	func batchAdd(Hosts: [HostModel]) {
		var hosts: [OpaquePointer?] = []
		hosts.reserveCapacity(Hosts.count)
		
		Hosts.forEach { hostData in
			let host_data = whost_data_make(hostData.displayName, hostData.hostname, hostData.ipAddress, hostData.macAddress, Int(hostData.lastSeenOnline.timeIntervalSince1970))
			if (host_data != nil) {
				hosts.append(host_data)
			}
		}
		
		waolasc_batch_add(scanner, &hosts, hosts.count)
	}
	
	func manuallyAddHost(hostData: HostData) {
		let host_data = whost_data_make(hostData.displayName, hostData.hostname, hostData.ipAddress, hostData.macAddress, Int(Date.distantPast.timeIntervalSince1970))
		if (host_data != nil) {
			waolasc_add(scanner, host_data)
			whost_data_free(host_data)
		}
	}
	
	
	func editHost(host: HostViewModel, hostData: HostData) {
		var updated = whost_view_set_display_name(host.hostView, hostData.displayName)
		updated |= whost_view_set_hostname(host.hostView, hostData.hostname)
		updated |= whost_view_set_ip_addr(host.hostView, hostData.ipAddress)
		updated |= whost_view_set_mac_addr(host.hostView, hostData.macAddress)
		
		if (updated != 0) {
			waolasc_refresh_host(scanner, host.hostView)
		}
	}
	
	func deleteHost(hostView: OpaquePointer!) {
		waolasc_delete_host(scanner, hostView)
	}
	
	func refreshHost(hostView: OpaquePointer!) {
		waolasc_refresh_host(scanner, hostView)
	}
	
	func getStatus(module: UnsafeMutablePointer<wmodule_t>, opCode: UnsafeMutablePointer<UInt32>) -> UnsafeRawPointer? {
		return waolasc_get_status(scanner, module, opCode)
	}
	
	private func subscribe() {
		let this = toUnsafeRawPointer(self)
		stateSubscribeData = UnsafeMutablePointer<waolasc_subscribe_data_t>.allocate(capacity: 1)
		stateSubscribeData!.pointee.caller = this
		stateSubscribeData!.pointee.cb = stateChangedCb
		
		vaultSubscribeData = UnsafeMutablePointer<waolava_subscribe_data_t>.allocate(capacity: 1)
		vaultSubscribeData!.pointee.caller = this
		vaultSubscribeData!.pointee.cb = vaultChangedCb
		
		waolasc_subscribe_state(scanner, stateSubscribeData)
		waolasc_subscribe_vault(scanner, vaultSubscribeData)
	}
	
	private func unsubscribe() {
		waolasc_unsubscribe_state(scanner, stateSubscribeData)
		waolasc_unsubscribe_vault(scanner, vaultSubscribeData)
		vaultSubscribeData?.deallocate()
		stateSubscribeData?.deallocate()
	}
}

func stateChangedCb(cbiPtr: UnsafePointer<waolasc_cbi_t>?) {
	let cbi: waolasc_cbi_t = cbiPtr!.pointee
	let scanner = fromUnsafeRawPointer(cbi.caller) as Scanner
	scanner.onStateEvent(tasks: cbi.tasks)
}

func vaultChangedCb(cbiPtr: UnsafePointer<waolava_cbi_t>?) {
	let cbi: waolava_cbi_t = cbiPtr!.pointee
	let scanner = fromUnsafeRawPointer(cbi.caller) as Scanner
	scanner.onVaultEvent(opCode: cbi.opCode, hostView: cbi.hostView)
}

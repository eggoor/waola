//
//  StatusMonitor.swift
//  WaolaS
//
//  Created by e on 27.09.2024.
//

import Foundation
import Network

class StatusMonitor {
	private var viewModel: ContentView.ContentViewModel
	private var lastModule: wmodule_t = wm_undefined
	private var lastOpCode: UInt32 = 0;
	private var lastStatusData: UnsafeRawPointer? = nil
	private var updated = false
	
	var timer: Timer?
	
	init (viewModel: ContentView.ContentViewModel) {
		self.viewModel = viewModel
	}
	
	deinit {
		timer?.invalidate()
	}
	
	func start() {
		timer = Timer.scheduledTimer(withTimeInterval: 0.04, repeats: true, block: {
			_ in self.onTimerTick()
		})
	}
	
	func stop() {
		timer?.invalidate()
		timer = nil;
		onTimerTick()
	}
	
	func onTimerTick() {
		var module: wmodule_t = wm_undefined;
		var opCode: UInt32 = 0;
		
		let statusData = viewModel.scanner.getStatus(module: &module, opCode: &opCode);
		processStatus(module: module, opCode: opCode, statusData: statusData);
	}
	
	func processStatus(module:wmodule_t, opCode:UInt32, statusData: UnsafeRawPointer?) {
		if module == wm_undefined {
			DispatchQueue.main.async { [self] in
				viewModel.status = "Undefined module in processStatus";
			}
			print(viewModel.status)
			return;
		}
		
		if lastModule != module	{
			updated = true;
			lastModule = module;
		}
		
		if lastOpCode != opCode	{
			if !updated {
				updated = true;
			}
			
			lastOpCode = opCode;
		}
		
		if updated || module != wm_scanner {
			processUpdate(statusData: statusData);
		}
	}
	
	func processUpdate(statusData: UnsafeRawPointer?)
	{
		switch lastModule {
		case wm_scanner:
			processScannerUpdate();
			break;
		case wm_discoverer:
			processDiscovererOrResolverUpdate(statusData: statusData);
			break;
		case wm_resolver:
			processDiscovererOrResolverUpdate(statusData: statusData);
			break;
		default:
			print("Invalid module identifier '\(lastModule)' is passed to processUpdate.")
		}
		
		if updated {
			updated = false;
		}
	}
	
	func processScannerUpdate()
	{
		var status: String
		
		switch waolasc_opcode_t(lastOpCode) {
		case wco_idle:
			status = "Idle"
		case wco_discovering:
			status = "Discovering"
		case wco_resolving:
			status = "Refreshing"
		case wco_cancelling:
			status = "Cancelling"
		default:
			status = "Invalid operation code '\(lastOpCode)' is passed to processScannerUpdate."
			print(viewModel.status)
		}
		
		DispatchQueue.main.async { [self] in
			viewModel.status = status
		}
	}
	
	func processDiscovererOrResolverUpdate(statusData: UnsafeRawPointer?) {
		if lastStatusData != statusData {
			lastStatusData = statusData;
			if !updated {
				updated = true;
			}
		}
		
		if !updated {
			return
		}
		
		switch lastModule {
		case wm_discoverer:
			processDiscovererUpdate(statusData: lastStatusData!)
		case wm_resolver:
			processResolverUpdate(statusData: lastStatusData!)
		default:
			print("Invalid module identifier '\(lastModule)' is passed to processDiscovererOrResolverUpdate.")
		}
	}
	
	func processDiscovererUpdate(statusData: UnsafeRawPointer)
	{
		switch (waoladi_opcode_t(lastOpCode)) {
		case wdi_scheduling_arp:
			onDiscovererIpAddress(statusData: statusData)
		case wdi_sending_arp:
			onDiscovererIpAddress(statusData: statusData)
		case wdi_got_arp:
			onDiscovererIpAddress(statusData: statusData)
		case wdi_arp_failed:
			onDiscovererIpAddress(statusData: statusData)
		default:
			print("Invalid operation code '\(lastOpCode)' is passed to processDiscovererUpdate.")
		}
	}
	
	func processResolverUpdate(statusData: UnsafeRawPointer)
	{
		switch (waolare_opcode_t(lastOpCode)) {
		case wre_getaddrinfo:
			onResolverHostName(statusData: statusData)
		case wre_getaddrinfo_failed:
			onResolverHostName(statusData: statusData)
		case wre_gotnameinfo:
			onResolverHostName(statusData: statusData)
		case wre_gotaddrinfo:
			onResolverIpAddress(statusData: statusData)
		case wre_getnameinfo:
			onResolverIpAddress(statusData: statusData)
		case wre_getnameinfo_failed:
			onResolverIpAddress(statusData: statusData)
		case wre_sending_arp:
			onResolverIpAddress(statusData: statusData)
		case wre_got_arp:
			onResolverIpAddress(statusData: statusData)
		case wre_arp_failed:
			onResolverIpAddress(statusData: statusData)
		default:
			print("Invalid operation code '\(lastOpCode)' is passed to processResolverUpdate.")
		}
	}
	
	func onDiscovererIpAddress(statusData: UnsafeRawPointer)
	{
		DispatchQueue.main.async { [self] in
			viewModel.status = discovererOpCode2Status(statusData: statusData);
		}
	}
	
	func onResolverHostName(statusData: UnsafeRawPointer)
	{
		DispatchQueue.main.async { [self] in
			viewModel.status = resolverHostnameOpCode2Status(statusData: statusData);
		}
	}
	
	func onResolverIpAddress(statusData: UnsafeRawPointer)
	{
		DispatchQueue.main.async { [self] in
			viewModel.status = resolverIpAddessOpCode2Status(statusData: statusData);
		}
	}
	
	private func discovererOpCode2Status(statusData: UnsafeRawPointer) -> String
	{
		var status: String
		let pInt: UnsafePointer<UInt32> = statusData.assumingMemoryBound(to: UInt32.self)
		let ipAddrData = withUnsafeBytes(of: pInt.pointee) { Data($0) }
		let ipAddrString = IPv4Address(ipAddrData)?.debugDescription ?? "?.?.?.?"
		
		switch (waoladi_opcode_t(lastOpCode)) {
		case wdi_scheduling_arp:
			status = "Scheduling connection to \(ipAddrString)"
		case wdi_sending_arp:
			status = "Trying to connect to \(ipAddrString)"
		case wdi_got_arp:
			status = "\(ipAddrString) connected"
		case wdi_arp_failed:
			status = "Failed to connect to \(ipAddrString)"
		default:
			status = "Invalid operation code '\(lastOpCode)' is passed to discovererOpCode2Status."
			print(status)
		}
		
		return status
	}
	
	private func resolverIpAddessOpCode2Status(statusData: UnsafeRawPointer) -> String
	{
		var status: String
		let pInt: UnsafePointer<UInt32> = statusData.assumingMemoryBound(to: UInt32.self)
		let ipAddrData = withUnsafeBytes(of: pInt.pointee) { Data($0) }
		let ipAddrString = IPv4Address(ipAddrData)?.debugDescription ?? "?.?.?.?"

		switch (waolare_opcode(lastOpCode))	{
		case wre_sending_arp:
			status = "Trying to connect to \(ipAddrString)"
		case wre_got_arp:
			status = "\(ipAddrString) connected"
		case wre_arp_failed:
			status = "Failed to connect to \(ipAddrString)"
		case wre_gotaddrinfo:
			status = "Resolved \(ipAddrString)"
		case wre_getnameinfo:
			status = "Resolving \(ipAddrString)"
		case wre_getnameinfo_failed:
			status = "Failed to resolve \(ipAddrString)"
		default:
			status = "Invalid operation code '\(lastOpCode)' is passed to resolverIpAddessOpCode2Status."
			print(status)
		}
		
		return status
	}
	
	private func resolverHostnameOpCode2Status(statusData: UnsafeRawPointer) -> String
	{
		var status: String
		let cHostname: UnsafePointer<CChar> = statusData.assumingMemoryBound(to: CChar.self)
		let hostname = String(cString: cHostname)
		
		switch (waolare_opcode(lastOpCode))	{
		case wre_getaddrinfo:
			status = "Resolving '\(hostname)'"
		case wre_gotnameinfo:
			status = "Resolved '\(hostname)'"
		case wre_getaddrinfo_failed:
			status = "Failed to resolve '\(hostname)'"
		default:
			status = "Invalid operation code '\(lastOpCode)' is passed to resolverHostnameOpCode2Status."
			print(status)
		}
		
		return status
	}
}

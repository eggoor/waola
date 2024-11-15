//
//  ContentViewModel.swift
//  WaolaS
//
//  Created by e on 04.09.2024.
//

import Foundation
import SwiftData
import AppKit

extension ContentView {
	class ContentViewModel: ObservableObject {
		@Published var hosts = [HostViewModel]()
		@Published var status = "Idle"
		var scanner = Scanner()
		var statusMonitor: StatusMonitor?
		var isBusy = false
		var modelContext: ModelContext
		var timer: Timer?
		
		init(modelContext: ModelContext) {
			self.modelContext = modelContext
			scanner.setViewModel(viewModel: self)
			loadHosts()
			timer = Timer.scheduledTimer(withTimeInterval: 15, repeats: true, block: { _ in
				self.updateView()
			})
		}
		
		deinit {
			statusMonitor = nil
			timer?.invalidate()
		}

		func updateView() {
			objectWillChange.send()
		}
		
		public func scanNetwork() {
			scanner.scanNetwork();
		}
		
		public func refresh() {
			scanner.refresh();
		}
		
		func onStateEvent(tasks: wtask_t) {
			isBusy = tasks != wt_idle
			
			if isBusy {
				if statusMonitor == nil {
					statusMonitor = StatusMonitor(viewModel: self)
				}
				statusMonitor?.start()
			}
			else {
				statusMonitor?.stop()
			}
		}
		
		func onVaultEvent(opCode: waolava_op_t, hostView: OpaquePointer!) {
			switch (opCode){
			case wva_added:
				addHost(hostView: hostView)
			case wva_modified:
				updateHost()
			case wva_deleted:
				deleteHost(hostView: hostView)
				break
			default:
				fatalError("Invalid vault event \(opCode))");
			}
		}
		
		func manuallyAddHost(hostData: HostData) {
			scanner.manuallyAddHost(hostData: hostData)
		}
		
		func editHost(host: HostViewModel, hostData: HostData) {
			scanner.editHost(host: host, hostData: hostData)
		}
		
		func deleteHosts(ids: Set<HostViewModel.ID>) {
			for id in ids {
				for i in 0..<hosts.count {
					if (hosts[i].id == id) {
						scanner.deleteHost(hostView: hosts[i].hostView)
						break
					}
				}
			}
		}
		
		func copySelected(ids: Set<HostViewModel.ID>) {
			if ids.count > 0 {
				var hostsString = "";
				for id in ids {
					for i in 0..<hosts.count {
						if (hosts[i].id == id) {
							hostsString += "\(hosts[i].toString())\n"
						}
					}
				}
				
				let pasteboard = NSPasteboard.general
				pasteboard.clearContents()
				pasteboard.setString(hostsString, forType: .string)
			}
		}
		
		func refreshHosts(ids: Set<HostViewModel.ID>) {
			for id in ids {
				for i in 0..<hosts.count {
					if (hosts[i].id == id) {
						scanner.refreshHost(hostView: hosts[i].hostView)
						break
					}
				}
			}
		}
		
		func wakeUpHosts(ids: Set<HostViewModel.ID>) {
			var selectedHosts: [OpaquePointer?] = []
			for id in ids {
				for i in 0..<hosts.count {
					if (hosts[i].id == id) {
						selectedHosts.append(hosts[i].hostView)
						break
					}
				}
			}
			
			waolah(&selectedHosts, selectedHosts.count);
			objectWillChange.send()
		}
		
		func getHost(id: HostViewModel.ID) -> HostViewModel? {
			var host: HostViewModel? = nil
			for i in 0..<hosts.count {
				if (hosts[i].id == id) {
					host = hosts[i]
					break
				}
			}
			return host;
		}
		
		private func addHost(hostView: OpaquePointer) {
			DispatchQueue.main.async {
				self.hosts.append(HostViewModel(hostView: hostView))
			}
		}
		
		private func updateHost() {
			DispatchQueue.main.async {
				self.objectWillChange.send()
			}
		}
		
		private func deleteHost(hostView: OpaquePointer) {
			DispatchQueue.main.async {
				for i in 0..<self.hosts.count {
					if (self.hosts[i].hostView == hostView) {
						_ = self.hosts.remove(at: i)
						break
					}
				}
			}
		}
		
		func loadHosts() {
			do {
				let descriptor = FetchDescriptor<HostModel>()
				let hosts = try modelContext.fetch(descriptor)
				if (hosts.count > 0) {
					scanner.batchAdd(Hosts: hosts);
				}
				else {
					scanNetwork()
				}				
			} catch {
				fatalError("Fetch failed: \(error)")
			}
		}
		
		func persist() {
			do {
				try modelContext.transaction {
					try modelContext.delete(model: HostModel.self)
					for host in hosts {
						modelContext.insert(host.toHostModel())
					}
					do {
						if modelContext.hasChanges {
							try modelContext.save()
						}
					} catch {
						print("Save failed: \(error) \(error.localizedDescription)")
					}
				}
			} catch {
				print("Model context transaction failed: \(error) \(error.localizedDescription)")
			}
		}
	}
}

//
//  ContentView.swift
//  WaolaS
//
//  Created by e on 31.08.2024.
//

import SwiftUI
import SwiftData

struct ContentView: View {
	@StateObject var viewModel: ContentViewModel

	var sortedHosts: [HostViewModel] {
		return viewModel.hosts.sorted(using: sortOrder)
	}
	
	@State private var selectedHosts = Set<HostViewModel.ID>()
	@State private var isAddingNewHost = false
	@State private var isEditingHost = false
	@State private var isPresentingAboutInfo = false
	@State private var sortOrder = [KeyPathComparator(\HostViewModel.displayName)]
	
	var body: some View {
		ZStack {
			Table(sortedHosts, selection: $selectedHosts, sortOrder: $sortOrder) {
				TableColumn("Friendly name", value: \.displayName)
				TableColumn("Hostname", value: \.hostname.unwrapOrEmpty)
				TableColumn("IP address", value: \.ipAddress.unwrapOrEmpty)
				TableColumn("MAC address", value: \.macAddress.unwrapOrEmpty)
				TableColumn("Last seen online", value: \.lastSeenOnline) {
					host in ContentView.DateTime2LastSeenOnlineString(lastSeenOnlineUtc: host.lastSeenOnline)
				}
				TableColumn("Wakeup result", value: \.wakeupResult.unwrapOrEmpty)
			}
			.frame(minWidth: 720, minHeight: 320)
			.contextMenu(forSelectionType: HostViewModel.ID.self, menu:	{
				items in
				Button("Wake up") {
					wakeUpSelected()
				}
				Divider()
				Button("Refresh") {
					refreshSelected()
				}
				Divider()
				Button("Edit") {
					editSelected()
				}
				Button("Delete") {
					deleteSelected()
				}
			}, primaryAction: {
				items in wakeUpSelected()
			})
			.toolbar {
				ToolbarItemGroup(placement: .principal) {
					Button(action: wakeUpSelected) {
						Label {
							Text("Wake up selected host(s)")
						} icon : {
							Image(systemName: "alarm")
								.resizable().scaledToFit()
						}
					}
					.disabled(selectedHosts.isEmpty)
					.help(Text("Wake up selected host(s)"))
					HStack { Divider().frame(height: 24) }
					Button(action: scanNetwork) {
						Label {
							Text("Scan network")
						} icon : {
							Image(systemName: "network")
								.resizable().scaledToFit()
						}
					}
					.help(Text("Scan network"))
					Button(action: refreshHosts) {
						Label {
							Text("Refresh hosts")
						} icon : {
							Image(systemName: "arrow.clockwise")
								.resizable().scaledToFit()
						}
					}
					.help(Text("Refresh hosts"))
					HStack { Divider().frame(height: 24) }
					Button(action: copySelected) {
						Label {
							Text("Copy selected host(s)")
						} icon : {
							Image(systemName: "doc.on.doc")
								.resizable().scaledToFit()
						}
					}
					.disabled(selectedHosts.isEmpty)
					.help(Text("Copy selected host(s)"))
					HStack { Divider().frame(height: 24) }
					Button(action: addNew) {
						Label {
							Text("Manually add new host")
						} icon : {
							Image(systemName: "plus.circle")
								.resizable().scaledToFit()
						}
					}
					.help(Text("Manually add new host"))
					Button(action: editSelected) {
						Label {
							Text("Edit selected host")
						} icon : {
							Image(systemName: "pencil")
								.resizable().scaledToFit()
						}
					}
					.disabled(selectedHosts.count != 1)
					.help(Text("Edit selected host"))
					Button(action: deleteSelected) {
						Label {
							Text("Delete selected host(s)")
						} icon : {
							Image(systemName: "minus.circle")
								.resizable().scaledToFit()
						}
					}
					.disabled(selectedHosts.isEmpty)
					.help(Text("Delete selected host(s)"))
					HStack { Divider().frame(height: 24) }
					Button(action: about) {
						Label {
							Text("About Waola")
						} icon : {
							Image(systemName: "questionmark.circle")
								.resizable().scaledToFit()
						}
					}
					.help(Text("About Waola"))
				}
			}
			.safeAreaInset(edge: .bottom, alignment: .leading, content: {
				Text("\(viewModel.status)")
			}).padding()
			.sheet(isPresented: $isAddingNewHost) {
				NavigationStack() {
					var hostData = HostData()
					withAnimation {
						HostView(hostData: &hostData)
							.navigationTitle("Add new host")
							.toolbar {
								ToolbarItem(placement: .cancellationAction) {
									Button("Cancel") {
										isAddingNewHost = false
									}
								}
								ToolbarItem(placement: .confirmationAction) {
									Button("Save") {
										isAddingNewHost = false
										viewModel.manuallyAddHost(hostData: hostData)
									}
								}
							}
					}
				}
			}
			.sheet(isPresented: $isEditingHost) {
				NavigationStack() {
					let selectedHost = getSelectedHost()
					var hostData = selectedHost?.getData() ?? HostData()
					withAnimation {
						HostView(hostData: &hostData)
							.navigationTitle("Edit host")
							.toolbar {
								ToolbarItem(placement: .cancellationAction) {
									Button("Cancel") {
										isEditingHost = false
									}
								}
								ToolbarItem(placement: .confirmationAction) {
									Button("Save") {
										isEditingHost = false
										viewModel.editHost(host: selectedHost!, hostData: hostData)
									}
								}
							}
					}
				}
			}
			.sheet(isPresented: $isPresentingAboutInfo) {
				NavigationStack() {
					withAnimation {
						AboutView()
							.navigationTitle("About Waola")
							.toolbar {
								ToolbarItem(placement: .confirmationAction) {
									Button("Close") {
										isPresentingAboutInfo = false
									}
								}
							}
					}
				}
			}
			
			if viewModel.isBusy {
				ProgressView().scaleEffect(2)
			}
		}
	}
	
	private func wakeUpSelected () {
		viewModel.wakeUpHosts(ids: selectedHosts);
	}
	
	private func scanNetwork () {
		viewModel.scanNetwork()
	}
	
	private func refreshSelected() {
		viewModel.refreshHosts(ids: selectedHosts);
	}
	
	private func refreshHosts () {
		viewModel.refresh()
	}
	
	private func copySelected () {
		//
	}
	
	private func addNew() {
		isAddingNewHost = true;
	}
	
	private func editSelected() {
		if (selectedHosts.count == 1) {
			isEditingHost = true;
		}
	}
	
	private func getSelectedHost() -> HostViewModel? {
		var host: HostViewModel? = nil
		let selectedHostId = selectedHosts.first
		if (selectedHostId != nil) {
			host = viewModel.getHost(id: selectedHostId!)
		}
		return host;
	}
	
	private func deleteSelected() {
		viewModel.deleteHosts(ids: selectedHosts);
	}
	
	private func about() {
		isPresentingAboutInfo = true;
	}
	
	init(viewModel: ContentViewModel) {
		_viewModel = StateObject(wrappedValue: viewModel)
	}
}

#Preview {
	let modelContainer: ModelContainer = {
		let schema = Schema([
			HostModel.self,
		])
		let modelConfiguration = ModelConfiguration(schema: schema, isStoredInMemoryOnly: true)
		
		do {
			return try ModelContainer(for: schema, configurations: [modelConfiguration])
		} catch {
			fatalError("Could not create ModelContainer: \(error)")
		}
	}()
	let viewModel = ContentView.ContentViewModel(modelContext: modelContainer.mainContext)
	
	return ContentView(viewModel: viewModel)
}

//
//  WaolaSApp.swift
//  WaolaS
//
//  Created by e on 31.08.2024.
//

import SwiftUI
import SwiftData

@main
struct WaolaSApp: App {
	@NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
	
	var modelContainer: ModelContainer = {
		let schema = Schema([
			HostModel.self,
		])
		let modelConfiguration = ModelConfiguration(schema: schema, isStoredInMemoryOnly: false)
		
		do {
			var modelContainer = try ModelContainer(for: schema, configurations: [modelConfiguration])
			modelContainer.mainContext.autosaveEnabled = false
			return modelContainer
		} catch {
			fatalError("Could not create ModelContainer: \(error) \(error.localizedDescription)")
		}
	}()
	
	var viewModel: ContentView.ContentViewModel
	
	init() {
		if wlog_try_set_level_from_env() != 0 {
			let env_log_level = wlog_get_level()
			waolash_set_log_level(env_log_level)
			waola_set_log_level(env_log_level)
			waolasc_set_log_level(env_log_level)
		}
		viewModel = ContentView.ContentViewModel(modelContext: modelContainer.mainContext)
	}
	
	func onTerminating() {
		viewModel.persist()
	}
	
	var body: some Scene {
		WindowGroup {
			ContentView(viewModel: viewModel)
				.onDisappear(perform: {
					onTerminating()
				})
				.onReceive(NotificationCenter.default.publisher(for: NSApplication.willTerminateNotification)) { _ in
					onTerminating()
				}
		}
		.modelContainer(modelContainer)
	}
}

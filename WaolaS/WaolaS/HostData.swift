//
//  Item.swift
//  WaolaS
//
//  Created by e on 31.08.2024.
//

import Foundation

final class HostData {
	var displayName: String
	var hostname: String
	var ipAddress: String
	var macAddress: String
	
	init() {
		self.displayName = ""
		self.hostname = ""
		self.ipAddress = ""
		self.macAddress = ""
	}
	
	init(displayName: String?, hostname: String?, ipAddress: String?, macAddress: String?) {
		self.displayName = displayName.unwrapOrEmpty
		self.hostname = hostname.unwrapOrEmpty
		self.ipAddress = ipAddress.unwrapOrEmpty
		self.macAddress = macAddress.unwrapOrEmpty
	}
}

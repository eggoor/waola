//
//  Item.swift
//  WaolaS
//
//  Created by e on 31.08.2024.
//

import Foundation
import SwiftData

@Model
final class HostModel : Codable {
	enum CodingKeys: CodingKey {
		case displayName, hostname, ipAddress, macAddress, lastSeenOnline
	}
	
	var displayName: String?
	var hostname: String?
	var ipAddress: String?
	var macAddress: String?
	var lastSeenOnline: Date
	
	init(displayName: String?, hostname: String?, ipAddress: String?, macAddress: String?, lastSeenOnline: Date?) {
		self.displayName = displayName
		self.hostname = hostname
		self.ipAddress = ipAddress
		self.macAddress = macAddress
		self.lastSeenOnline = lastSeenOnline ?? Date.distantPast
	}
	
	required init(from decoder: Decoder) throws {
		let container = try decoder.container(keyedBy: CodingKeys.self)
		
		displayName = try? container.decode(String.self, forKey: .displayName)
		hostname = try? container.decode(String.self, forKey: .hostname)
		ipAddress = try? container.decode(String.self, forKey: .ipAddress)
		macAddress = try? container.decode(String.self, forKey: .macAddress)
		lastSeenOnline = try container.decode(Date.self, forKey: .lastSeenOnline);
	}
	
	func encode(to encoder: Encoder) throws {
		var container = encoder.container(keyedBy: CodingKeys.self)
		try container.encode(displayName, forKey: .displayName)
		try container.encode(hostname, forKey: .hostname)
		try container.encode(ipAddress, forKey: .ipAddress)
		try container.encode(macAddress, forKey: .macAddress)
		try container.encode(lastSeenOnline, forKey: .lastSeenOnline)
	}
}

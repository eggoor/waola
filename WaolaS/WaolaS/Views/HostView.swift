//
//  HostView.swift
//  WaolaS
//
//  Created by e on 17.09.2024.
//

import SwiftUI

struct HostView: View {
	@State private var hostData: HostData
	
	var body: some View {
		Form {
			TextField("Display name", text: $hostData.displayName)
			TextField("Hostname", text: $hostData.hostname)
			TextField("IP address", text: $hostData.ipAddress)
			TextField("MAC address", text: $hostData.macAddress)
		}
		.frame(width: 256)
		.padding()
	}
	
	init(hostData: inout HostData) {
		self.hostData = hostData
	}
}

#Preview {
	var hostData = HostData(displayName: nil, hostname: nil, ipAddress: nil, macAddress: nil)
	return HostView(hostData: &hostData)
}

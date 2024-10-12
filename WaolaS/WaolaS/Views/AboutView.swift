//
//  AboutView.swift
//  WaolaS
//
//  Created by e on 18.09.2024.
//

import SwiftUI

struct AboutView: View {
	let license: String
	var body: some View {
		TabView {
			VStack(spacing: 0) {
				HStack(alignment: .top) {
					Text("Waola is a [Wake-on-LAN](https://en.wikipedia.org/wiki/Wake-on-LAN) utility with embedded [ARP](https://en.wikipedia.org/wiki/Address_Resolution_Protocol)-based network scanner.\n\nAuthor of this tool ​​is a citizen of Ukraine who created and maintains this software literally under attacks of russian missiles.\n\nAlso I try to help our army to fight off the invasion as much as I can.\n\nIf this tool was useful to you and you'd like to help us to defend our freedom and independence - feel free to donate any convenient for you amount to PayPal account [egggor@gmail.com](https://www.paypal.com/ua/home).")
						.textSelection(.enabled)
				}.padding()
			}
			.offset(y: 0)
			.frame(minWidth: 640, maxHeight: 480, alignment: .topLeading)
			.tabItem() {
				Text("About")
			}
			VStack(spacing: 0) {
				HStack(alignment: .top) {
					VStack(alignment: .leading) {
						Text("Idea, architecture and implementation: [egggor@gmail.com](mailto:egggor@gmail.com)\n\nIcon set: TUNUHA")
					}
				}.padding()
			}
			.frame(minWidth: 640, maxHeight: 480, alignment: .topLeading)
			.tabItem() {
				Text("Credits")
			}
			ScrollView() {
				Text(license).frame(maxWidth: .infinity)
			}
			.tabItem() {
				Text("License")
			}
		}
		.frame(width: 640, height: 320)
		.padding()
		.background()
	}
	
	init() {
		license = loadLicense()
	}
}

#Preview {
	return AboutView()
}

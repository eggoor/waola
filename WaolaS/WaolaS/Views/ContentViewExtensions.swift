//
//  ContentViewExtensions.swift
//  WaolaS
//
//  Created by e on 16.09.2024.
//

import Foundation
import SwiftUI

extension ContentView {
	static func DateTime2LastSeenOnlineString(lastSeenOnlineUtc: Date) -> Text {
		return lastSeenOnlineUtc == Date.distantPast
		? Text("Never")
		: checkYears(lastSeenOnlineUtc: lastSeenOnlineUtc)
	}
	
	static func checkYears(lastSeenOnlineUtc: Date) -> Text {
		let now = Date()
		let years = Calendar.current.dateComponents([.year], from: lastSeenOnlineUtc, to: now).year!
		if (years > 2) {
			return Text("\(years) years ago")
		}
		else if (years > 1) {
			return Text("More than a year ago")
		}
		else {
			return checkMonths(now: now, lastSeenOnlineUtc: lastSeenOnlineUtc)
		}
	}
	
	static func checkMonths(now: Date, lastSeenOnlineUtc: Date) -> Text {
		let months = Calendar.current.dateComponents([.month], from: lastSeenOnlineUtc, to: now).month!
		if (months > 10) {
			return Text("About a year ago")
		}
		else if (months >= 8 && months <= 10) {
			return Text("\(months) months ago")
		}
		else if (months > 4 && months < 8) {
			return Text("About 1/2 of a year ago")
		}
		else if (months >= 2) {
			return Text("\(months) months ago")
		}
		else if (months >= 1) {
			return Text("About a month ago")
		}
		else {
			return checkDays(now: now, lastSeenOnlineUtc: lastSeenOnlineUtc)
		}
	}
	
	static func checkDays(now: Date, lastSeenOnlineUtc: Date) -> Text {
		let days = Calendar.current.dateComponents([.day], from: lastSeenOnlineUtc, to: now).day!
		if (days > 2) {
			return Text("\(days) days ago")
		}
		if (days > 2) {
			return Text("\(days) days ago")
		}
		else if (days > 1) {
			return Text("About a day ago")
		}
		else {
			return checkHours(now: now, lastSeenOnlineUtc: lastSeenOnlineUtc)
		}
	}
	
	static func checkHours(now: Date, lastSeenOnlineUtc: Date) -> Text {
		let hours = Calendar.current.dateComponents([.hour], from: lastSeenOnlineUtc, to: now).hour!
		if (hours > 12) {
			return Text("Less then a day ago")
		}
		else if (hours > 2) {
			return Text("\(hours) hours ago")
		}
		else if (hours > 1) {
			return Text("About a hour ago")
		}
		else {
			return checkMinutes(now: now, lastSeenOnlineUtc: lastSeenOnlineUtc)
		}
	}
	
	static func checkMinutes(now: Date, lastSeenOnlineUtc: Date) -> Text {
		let minutes = Calendar.current.dateComponents([.minute], from: lastSeenOnlineUtc, to: now).minute!
		if (minutes > 45) {
			return Text("Less then a hour ago")
		}
		else if (minutes >= 2) {
			return Text("\(minutes) minutes ago")
		}
		else if (minutes >= 1) {
			return Text("About a minute ago")
		}
		else {
			return checkSeconds(now: now, lastSeenOnlineUtc: lastSeenOnlineUtc)
		}
	}
	
	static func checkSeconds(now: Date, lastSeenOnlineUtc: Date) -> Text {
		let seconds = Calendar.current.dateComponents([.second], from: lastSeenOnlineUtc, to: now).second!
		if (seconds > 45) {
			return Text("About a minute ago")
		}
		else {
			return Text("Just now")
		}
	}
}

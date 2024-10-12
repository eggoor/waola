//
//  Helpers.swift
//  WaolaS
//
//  Created by e on 13.09.2024.
//

import Foundation

func toUnsafeRawPointer<T: AnyObject>(_ obj: T) -> UnsafeRawPointer! {
	return UnsafeRawPointer(Unmanaged.passUnretained(obj).toOpaque())
}

func fromUnsafeRawPointer<T: AnyObject>(_ ptr: UnsafeRawPointer) -> T {
	return Unmanaged<T>.fromOpaque(ptr).takeUnretainedValue()
}

func loadLicense() -> String {
	var license = "[GNU LESSER GENERAL PUBLIC LICENSE](https://www.gnu.org/licenses/lgpl-3.0.en.html)"
	
	do {
		license = try String(contentsOfFile: Bundle.main.path(forResource: "LICENSE", ofType: "txt")!)
	}
	catch let error as NSError {
		print(error.localizedDescription)
	}
	
	return license;
}

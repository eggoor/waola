//
//  Extensions.swift
//  WaolaS
//
//  Created by e on 09.09.2024.
//

import Foundation

extension Optional where Wrapped == String {
	var unwrapOrEmpty: Wrapped {
		self ?? ""
	}
}

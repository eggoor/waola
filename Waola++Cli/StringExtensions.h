#pragma once
#include <string>

using namespace System;

namespace WaolaCli {

	ref class StringExtensions
	{
	public:
		static String^ FromUtf8(const char* s);
		static String^ FromUtf8(const std::string& s);

		static char* ToUtf8(String^ s);
	};
}
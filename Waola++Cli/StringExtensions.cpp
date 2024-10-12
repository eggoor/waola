#include "pch.h"

#include "StringExtensions.h"

using namespace System::Text;
using namespace WaolaCli;

using System::Runtime::InteropServices::Marshal;

String^ StringExtensions::FromUtf8(const std::string& str)
{
	return FromUtf8(str.c_str());
}

String^ StringExtensions::FromUtf8(const char* str)
{
	String^ utf16 = nullptr;
	auto stringLength = static_cast<int>(strlen(str));

	if (stringLength > 0) {
		array<unsigned char>^ bytes = gcnew array<unsigned char>(stringLength);

		for (int i = 0; i < stringLength; ++i)
		{
			bytes[i] = str[i];
		}

		utf16 = System::Text::Encoding::UTF8->GetString(bytes);
	}

	return utf16;
}

char* StringExtensions::ToUtf8(String^ str)
{
	char* result = nullptr;

	if (str)
	{
		auto bytes = Encoding::UTF8->GetBytes(str);
		auto lenght = bytes->Length;

		if (lenght > 0) {
			result = new char[lenght + 1];
			memset(result, 0, lenght + 1);
			Marshal::Copy(bytes, 0, IntPtr(result), lenght);
		}
	}

	return result;
}

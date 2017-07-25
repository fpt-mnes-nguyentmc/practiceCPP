#pragma once

#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <locale>
#include <codecvt>
#include <msclr\marshal_cppstd.h>
#include <string>

using namespace System;

namespace CLI
{
	inline std::string ConvertFromUtf16ToUtf8(const std::wstring& wstr)
	{
		std::string convertedString;
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<char> buffer(requiredSize);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		return convertedString;
	}

	std::string ToUnmanaged(System::String^ str)
	{
		return ConvertFromUtf16ToUtf8(msclr::interop::marshal_as<std::wstring>(str));
	}
}
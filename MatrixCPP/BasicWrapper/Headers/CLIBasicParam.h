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

using namespace System;

namespace CLI
{
#pragma region To-Unmanaged

	static int ToUnmanaged(Int32 objInput)
	{
		return static_cast<int>(objInput);
	}

	static float ToUnmanaged(Single objInput)
	{
		return static_cast<float>(objInput);
	}

	static double ToUnmanaged(Double objInput)
	{
		return static_cast<double>(objInput);
	}

	static bool ToUnmanaged(Boolean objInput)
	{
		return static_cast<bool>(objInput);
	}

	static HWND ToUnmanaged(IntPtr^ objInput)
	{
		return (HWND)objInput->ToPointer();
	}

	static std::string ToUnmanaged(String^ objInput)
	{
		std::wstring wstr = msclr::interop::marshal_as<std::wstring>(objInput);
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

#pragma endregion

#pragma region To-Managed

	static Int32 ToManaged(const int& objInput)
	{
		return static_cast<Int32>(objInput);
	}

	static Single ToManaged(const float& objInput)
	{
		return static_cast<Single>(objInput);
	}

	static Double ToManaged(const double& objInput)
	{
		return static_cast<Double>(objInput);
	}

	static Boolean ToManaged(const bool& objInput)
	{
		return static_cast<Boolean>(objInput);
	}

	static String^ ToManaged(std::string objInput)
	{
		int len = static_cast<int>(objInput.length());

		if (len > 0)
		{
			System::Text::UTF8Encoding encoder;
			array<unsigned char>^ d = gcnew array<unsigned char>(len);
			pin_ptr<unsigned char> d_st(&d[0]);

			memcpy(d_st, &objInput[0], sizeof(unsigned char) * len);

			return encoder.GetString(d, 0, len);
		}

		return System::String::Empty;
	}

#pragma endregion
}
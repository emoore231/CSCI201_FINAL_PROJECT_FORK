#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace Moore
{
	namespace Bootstrapper
	{

		extern std::wstring (*GetDataListsFileNameImpl) (std::wostream* Out, std::wistream* In);
		inline std::wstring GetDataListsFileName (std::wostream* Out, std::wistream* In)
		{
			return (*GetDataListsFileNameImpl)(Out, In);
		}

		std::wstring GetDataListsFileNameImplemented (std::wostream* Out, std::wistream* In);

		DLL_EXPORT inline void SetGetDataListsFileName (std::wstring (*Impl) (std::wostream* Out, std::wistream* In) = GetDataListsFileNameImplemented)
		{
			GetDataListsFileNameImpl = Impl;
		}

	}
}
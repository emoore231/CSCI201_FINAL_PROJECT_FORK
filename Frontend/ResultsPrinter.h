#pragma once

#include "../Backend/StateTable.h"

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace Moore
{
	namespace Frontend
	{
		enum class Result
		{
			BROKE,
			POWERLESS,
			NO_CORRUPTION,

			DEAD,
			DETHROWNED,
			INSURRECTION,
			ARMEGGEDON,
			LIZZARDATION,

			SUCCESS
		};
		
		static void (*PrintResultsImpl)(Result r, Backend::StateTable st) = nullptr;

		inline void PrintResults (Result r, Backend::StateTable st)
		{
			return (*PrintResultsImpl)(r, st);
		}

		void PrintResultsImplemented (Result r, Backend::StateTable st);

		DLL_EXPORT inline void SetPrintResults (void (*Impl)(Result r, Backend::StateTable st) = PrintResultsImplemented)
		{
			PrintResultsImpl = Impl;
		}
	}
}
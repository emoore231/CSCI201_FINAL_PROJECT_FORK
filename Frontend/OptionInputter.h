#pragma once

#include <sstream>

#include "../Backend/StateTable.h"
#include "../Shared/Dialogue.h"

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace Moore
{
	namespace Frontend
	{
		int GetOptionImplemented (const Backend::StateTable st, const Shared::Dialogue d);
		static int (*GetOptionImpl) (const Backend::StateTable st, const Shared::Dialogue d) = nullptr;
		inline int GetOption (const Backend::StateTable st, const Shared::Dialogue d)
		{
			return (*GetOptionImpl) (st, d);
		}
		DLL_EXPORT inline void SetGetOptionImpl (int (*Impl) (const Backend::StateTable st, const Shared::Dialogue d) = GetOptionImplemented)
		{
			GetOptionImpl = Impl;
		}
	}
}


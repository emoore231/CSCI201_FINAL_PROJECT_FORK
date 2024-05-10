#pragma once

#include <string>
#include <setjmp.h>

#include "../Backend/DataLists.h"
#include "../Backend/StateTable.h"

namespace Moore
{
	namespace Core
	{
		class Frankenstein;
	}
}

jmp_buf FrankensteinJump;

class Moore::Core::Frankenstein final
{
	virtual ~Frankenstein () = 0; // prevent initialization

	static Backend::DataLists Datas;

	static void LoadDataLists (const Backend::DataLists d)
	{
	}

	static Backend::StateTable States;

	static void LoadStateTables (const Backend::StateTable st)
	{
		States = st;
	}

	static void Execute (::jmp_buf Jumper)
	{
		if (Datas.Dialogues.Dialogues.size() == 0)
		{
			
		}
	}
};


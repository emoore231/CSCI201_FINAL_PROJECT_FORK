#pragma once

#include <string>

#include "../Shared/Dialogue.h"

namespace Moore
{
	namespace Backend
	{
		//parse the datalists string into a list of dialogues
		std::vector<Moore::Shared::Dialogue> ParseDataLists (std::wstring DataLists);
	}
}
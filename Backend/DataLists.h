#pragma once

#include "DialogueTable.h"
#include "Configuration.h"

namespace Moore
{
	namespace Backend
	{
		class DataLists;
	}
}

//stores the dialogues and 100% real configuration
class Moore::Backend::DataLists
{
public:
	DialogueTable Dialogues;
	Configuration Config;

	DataLists (const std::wstring FileName);

	DataLists ()
	{};
};
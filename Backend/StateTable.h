#pragma once

#include <string>

namespace Moore
{
	namespace Backend
	{
		struct StateTable;
	}
}

//state table for current state
struct Moore::Backend::StateTable
{
	unsigned long Cash;
	unsigned long Corruption;
	unsigned long Power;

	std::wstring Party;
	std::wstring Goal;
};


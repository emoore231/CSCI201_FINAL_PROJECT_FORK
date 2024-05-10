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
	long Cash;
	long Corruption;
	long Power;

	std::wstring Party;
	std::wstring Goal;
};


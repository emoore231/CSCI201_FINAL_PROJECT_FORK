#pragma once

#include <vector>

#include "../Shared/Dialogue.h"

namespace Moore
{
	namespace Backend
	{
		class DialogueTable;
	}
}

//stores the dialogues in table format
class Moore::Backend::DialogueTable
{
public:
	std::vector<Moore::Shared::Dialogue> Dialogues;

	DialogueTable (const std::vector<Moore::Shared::Dialogue> dialogues);

	DialogueTable ();

	//locate the dialogue in the table, throw if not found
	const Moore::Shared::Dialogue FindDialogue (const std::wstring Code);
};
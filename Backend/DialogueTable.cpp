#include "DialogueTable.h"

Moore::Backend::DialogueTable::DialogueTable (const std::vector<Moore::Shared::Dialogue> dialogues)
	: Dialogues (dialogues)
{}

Moore::Backend::DialogueTable::DialogueTable ()
{}

//locate the dialogue in the table, throw if not found

const Moore::Shared::Dialogue Moore::Backend::DialogueTable::FindDialogue (const std::wstring Code)
{
	for (const auto& Dialogue : Dialogues)
	{
		if (Dialogue.Code == Code)
		{
			return Dialogue;
		}
	}
	throw std::runtime_error ("DIALOGUE_NOT_FOUND");
}

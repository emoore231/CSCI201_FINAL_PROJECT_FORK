#pragma once

#include "../Backend/StateTable.h"
#include "../Shared/Dialogue.h"
#include "../Shared/Option.h"
#include "../Backend/DataLists.h"
#include "../Backend/DialogueTable.h"
#include "../Frontend/ResultsPrinter.h"

namespace Moore
{
	namespace Core
	{
		class DialogueNavigator;
	}
}

class Moore::Core::DialogueNavigator
{
private:
	static Backend::DataLists* CoreDataLists;

	static Shared::Dialogue CurrentDialogue;

	static Backend::DialogueTable Dialogues;

	static Backend::StateTable States;

public:
	static void LoadDataLists (const Backend::DataLists d);

	static void ClearDataLists ();

	static void LoadInitialDialogue ();

	static void Halt (Frontend::Result r = Frontend::Result::SUCCESS);

	static void AdvanceDialogue (int opt);

	static void AdvanceDialogue ();

	static void Initialize (const Backend::DataLists d);

	
};
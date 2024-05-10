#include "DialogueNavigator.h"

#include "../Frontend/OptionInputter.h"
#include "../Frontend/DialoguePrinter.h"

#include "../Frontend/ResultsPrinter.h"

#include "setjmp.h"

//jumpers for control flow
static jmp_buf Jumper;

extern jmp_buf MainJumper;

//static defs
Moore::Backend::DataLists* Moore::Core::DialogueNavigator::CoreDataLists{};
Moore::Shared::Dialogue Moore::Core::DialogueNavigator::CurrentDialogue{};
Moore::Backend::DialogueTable Moore::Core::DialogueNavigator::Dialogues{};
Moore::Backend::StateTable Moore::Core::DialogueNavigator::States{};

//load the datalists into internal state
inline void Moore::Core::DialogueNavigator::LoadDataLists (const Backend::DataLists d)
{
	CoreDataLists = new Backend::DataLists (d);
	Dialogues = CoreDataLists->Dialogues;
}

//clear the datalists
inline void Moore::Core::DialogueNavigator::ClearDataLists ()
{
	if (CoreDataLists) delete CoreDataLists;
}

//find the initial dialogue (code '/') and load
inline void Moore::Core::DialogueNavigator::LoadInitialDialogue ()
{
	CurrentDialogue = Dialogues.FindDialogue (L"/");
}

//clean up and jump back to main()
inline void Moore::Core::DialogueNavigator::Halt (Frontend::Result r)
{


	Frontend::PrintResults (r, States);

	ClearDataLists ();

	longjmp (Jumper, 1);
}

//advance to the next dialogue
inline void Moore::Core::DialogueNavigator::AdvanceDialogue (int opt)
{
	//check option number
	if (opt > CurrentDialogue.Options.size () - 1 || opt < 0) throw "CORE_EXCEPTION";

	//trailing char patch
	if (CurrentDialogue.Options[opt].Goto.size () > 0 && CurrentDialogue.Options[opt].Goto[CurrentDialogue.Options[opt].Goto.size () - 1] == 65535) CurrentDialogue.Options[opt].Goto.pop_back ();

	CurrentDialogue = Dialogues.FindDialogue (CurrentDialogue.Options[opt].Goto);

	//check halt code
	if (CurrentDialogue.Code == L"~")
	{
		int Option = rand () % 5 + 3;
		Halt (Frontend::Result(Option));
	}

	//check if options are available, halt if none found
	int Available{};
	for (const auto& Opt : CurrentDialogue.Options)
	{
		if (Opt.Cash + States.Cash > 0l) Available++;
		if (Opt.Power + States.Power > 0l) Available++;
		if (Opt.Corruption + States.Corruption > 0) Available++;
		if (Available == 3)
		{
			Available = 1; break;
		}
		else Available = 0;
	}
	if (CurrentDialogue.Options.size () == 0) Available = true;
	if (!Available) Halt (Frontend::Result (rand () % 3));
}

//used for messages
inline void Moore::Core::DialogueNavigator::AdvanceDialogue ()
{//trailing char patch
	if (CurrentDialogue.Goto.size () > 0 && CurrentDialogue.Goto[CurrentDialogue.Goto.size () - 1] == 65535) CurrentDialogue.Goto.pop_back ();
	CurrentDialogue = Dialogues.FindDialogue (CurrentDialogue.Goto);

	//halt check
	if (CurrentDialogue.Code == L"~")
	{
		int Option = rand () % 5 + 3;
		Halt (Frontend::Result (Option));
	}

	//check if options are available, halt if none found
	int Available{};
	for (const auto& Opt : CurrentDialogue.Options)
	{
		if (Opt.Cash + States.Cash > 0l) Available++;
		if (Opt.Power + States.Power > 0l) Available++;
		if (Opt.Corruption + States.Corruption > 0) Available++;
		if (Available == 3) { Available = 1; break; } else Available = 0;
	}
	if (CurrentDialogue.Options.size () == 0) Available = true;
	if (!Available) Halt (Frontend::Result(rand () % 3));
}

//initialize the navigator
void Moore::Core::DialogueNavigator::Initialize (const Backend::DataLists d) try
{
	//set jump
	if (::setjmp (Jumper) == 1)
	{
		longjmp (MainJumper, 1);
	}

	//load stuff
	LoadDataLists (d);

	LoadInitialDialogue ();

	//set the frontend functions for console mode
	Frontend::SetImplementationPointers ();
	Frontend::SetPrintResults ();
	Frontend::SetGetOptionImpl ();

	//main loop
	while (true)
	{
		switch (CurrentDialogue.Type)
		{//handle menus
			case Shared::Dialogue::type::MENU:
				{
					//i/o
					Frontend::DialoguePrinter (CurrentDialogue, States);
					int val = Frontend::GetOption (States, CurrentDialogue);

					//update states
					States.Cash += CurrentDialogue.Options[val].Cash; 
					States.Power += CurrentDialogue.Options[val].Power;
					States.Corruption += CurrentDialogue.Options[val].Corruption;

					if (CurrentDialogue.Options[val].Party.size () != 0) States.Party = CurrentDialogue.Options[val].Party;
					if (CurrentDialogue.Options[val].Goal.size () != 0) States.Goal = CurrentDialogue.Options[val].Goal;

					//print selmsg
					Frontend::SelMessagePrinter (CurrentDialogue.Options[val].SelMsg, States);

					//next dialogue
					AdvanceDialogue (val);
				}
				break;
			case Shared::Dialogue::type::MSG:
				{
					//print and next
					Frontend::MessagePrinter (CurrentDialogue, States);
					AdvanceDialogue ();
				}
				break;
			default:
				throw std::runtime_error ("OoPS");
				break;
		}
	}
}
catch (...)
{
	//handle uncaught exceptions
	longjmp (MainJumper, 2);
}
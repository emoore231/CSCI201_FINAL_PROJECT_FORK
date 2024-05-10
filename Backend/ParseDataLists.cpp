#include "ParseDataLists.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

#include "../Shared/Option.h"
#include "../Shared/Dialogue.h"

#if !defined(NDEBUG) && defined(TRACE)
#define TRACE(MSG) std::wcout << L"TRACE: " << MSG << std::endl
#else
#define TRACE(MSG) NULL
#endif // DEBUG

//crapout test, not needed cos eofbit exception
static const constexpr int MaxLines = 10000;

//data classes
//local to impl, will be converted later
struct OptionData
{
	std::wstring Code;
	std::wstring Title;
	std::wstring Desc;
	std::wstring SelMsg;
	std::wstring Party;
	std::wstring Goal;
	std::wstring Goto;
	long Cash;
	long Corruption;
	long Power;

	operator Moore::Shared::Option ()
	{
		if (Code.size () > 0 && Code[Code.size () - 1] == 65535) Code.pop_back ();
		if (Code.size () > 0 && Code[Code.size () - 1] == 65535) Goto.pop_back ();
		return Moore::Shared::Option (Code, Title, Desc, SelMsg, Party, Goal, Goto, Cash, Corruption, Power);
	}
};


//stores the data for the dialogues
struct DialogueData
{
	enum class type
	{
		MENU,
		MSG
	} Type;

	std::wstring Code;
	std::wstring Title;
	std::wstring Desc;
	std::wstring Goto;
	std::vector<OptionData> Options;

	DialogueData ()
		: Type (), Code (), Title (), Desc (), Goto (), Options ()
	{}

	DialogueData
	(
		const type _Type,
		const std::wstring _Code,
		const std::wstring _Title,
		const std::wstring _Desc,
		const std::wstring _Goto,
		const std::vector<OptionData> _Options
	) :
		Type (_Type), Code (_Code), Desc (_Desc), Goto (_Goto), Options (_Options)
	{}

	operator Moore::Shared::Dialogue () 
	{
		std::vector<Moore::Shared::Option> Opts;
		for (auto& Option : Options)
		{
			Opts.push_back (Moore::Shared::Option (Option));
		}
		if (Code.size () > 0 && Code[Code.size () - 1] == 65535) Code.pop_back ();
		if (Code.size () > 0 && Code[Code.size () - 1] == 65535) Goto.pop_back ();
		return Moore::Shared::Dialogue ((Moore::Shared::Dialogue::type) Type, Code, Title, Desc, Goto, Opts);
	}
};


/**
 * @brief Loads the docs portion of the data lists, all text is discarded
 * @param FileData The file string
 * @return Docs string
 */
static inline std::wstring LoadDocs (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	std::wstring Token;
	std::wstring Return;
	//search for token
	while (std::getline(FileStr, Token), Token.find(L"BEGIN_DOCS") == std::string::npos)
	{
		TRACE (L"FINDDOCS " << Token);
	}

	TRACE (L"" << L"DOCS FOUND!");

	//add text until stop token
	while (std::getline (FileStr, Token), Token.find (L"END_DOCS") == std::string::npos)
	{
		Return += Token + L"\n";
		TRACE (L"FINDENDDOCS " << Token);
	}

	TRACE (L"" << L"DOCSEND FOUND!");

	return Return;
}

/**
 * @brief Loads the configuration portion of the data lists
 * @param FileData The file string
 * @return The configuration string
 */
static inline std::wstring LoadConfiguration (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	std::wstring Token;
	std::wstring Return;
	//scan start token
	while (std::getline (FileStr, Token), Token.find (L"BEGIN_CONFIGURATION") == std::string::npos)
	{
		TRACE (L"FINDCONFIGURATION " << Token);
	}

	TRACE (L"" << L"CONFIGURATION FOUND!");

	//add until halt token
	while (std::getline (FileStr, Token), Token.find (L"END_CONFIGURATION") == std::string::npos)
	{
		Return += Token + L"\n";
		TRACE (L"FINDENDCONFIGURATION " << Token);
	}

	TRACE (L"" << L"ENDCONFIGURATION FOUND!");
	return Return;
}

static inline std::wstring LoadData (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	std::wstring Token;
	std::wstring Return;
	//scan start token
	while (std::getline (FileStr, Token), Token.find (L"BEGIN_DATA") == std::string::npos)
	{
		TRACE (L"FINDDATA " << Token);
	}

	TRACE (L"" << L"DATA FOUND!");

	//add until halt token
	while (std::getline (FileStr, Token), Token.find (L"END_DATA") == std::string::npos)
	{
		Return += Token + L"\n";
		TRACE (L"FINDENDDATA " << Token);
	}

	TRACE (L"" << L"DATAEND FOUND!");
	return Return;
}

static inline std::wstring LoadListDialogues (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	std::wstring Token;
	std::wstring Return;
	//scan start token
	while (std::getline (FileStr, Token), Token.find (L"BEGIN_LIST NAME=DIALOGUES") == std::string::npos)
	{
		TRACE (L"FINDDATA " << Token);
	}

	TRACE (L"" << L"DATA FOUND!");

	//add until halt token
	while (std::getline (FileStr, Token), Token.find (L"END_LIST NAME=DIALOGUES") == std::string::npos)
	{
		Return += Token + L"\n";
		TRACE (L"FINDENDDATA " << Token);
	}

	TRACE (L"" << L"DATAEND FOUND!");
	return Return;
}

/**
 * @brief Loads the dialogues from the list
 * @param FileData The list data
 * @return Vector of dialogue datas
 */
static inline std::vector<std::wstring> LoadDialogues (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	FileStr.exceptions (std::ios::eofbit);
	std::wstring Token;
	std::vector<std::wstring> Return;
	//load each dialogue from list and add ot array
	int i = 0;
	try 
	{
		do
		{
			Return.push_back (std::wstring ());
			//scan for tokens
			while (std::getline (FileStr, Token), Token.find (L"BEGIN_DIALOGUE") == std::string::npos)
			{
				TRACE (L"FIND_DIALOG " << Token);
			}

			TRACE (L"" << L"DIALOG FOUND!");

			//add until stop token
			while (std::getline (FileStr, Token), Token.find (L"END_DIALOGUE") == std::string::npos)
			{
				Return[i] += Token + L"\n";
				TRACE (L"FIND_END_DIALOG " << Token);
			}

			TRACE (L"" << L"END_DIALOG FOUND!");
		}
		while (++i);
	}
	catch (...)
	{
		//add to vector at eof
		Return.pop_back ();
		return Return;
	}
}


static inline std::vector<std::wstring> LoadDialogueOptions (const std::wstring FileData)
{
	std::wistringstream FileStr (FileData);
	FileStr.exceptions (std::ios::eofbit);
	std::wstring Token;
	std::vector<std::wstring> Return;
	int i = 0;
	try
	{
		do
		{
			//scan start token
			Return.push_back (std::wstring ());
			while (std::getline (FileStr, Token), Token.find (L"BEGIN_OPTION") == std::string::npos)
			{
				TRACE (L"FIND_DIALOG_OPTION " << Token);
			}

			TRACE (L"" << L"DIALOG OPTION FOUND!");

			//add until stop token
			while (std::getline (FileStr, Token), Token.find (L"END_OPTION") == std::string::npos)
			{
				Return[i] += Token + L"\n";
				TRACE (L"FIND_END_DIALOG_OPTION " << Token);
			}

			TRACE (L"" << L"END_DIALOG OPTION FOUND!");
		}
		while (++i);
	}
	catch (...)
	{
		Return.pop_back ();
		return Return;
	}
	return std::vector<std::wstring>();
}

//tags for dialogue options
static const std::vector<std::wstring> OptionTags =
{
	L"CODE",
	L"DESCRIPTION",
	L"TITLE",
	L"CASH",
	L"CORRUPTION",
	L"POWER",
	L"SELECTION_MSG",
	L"PARTY",
	L"GOAL",
	L"GOTO"
};

static inline OptionData ParseOption (std::wstring OptionsData)
{
	OptionData Option{};

	std::vector<std::thread> Threads;

	//scan for each option tag
	for (const auto& OptionTag : OptionTags)
	{
		Threads.push_back (std::thread (
			[&]() -> int
			{
				try
				{
					std::wistringstream OptionsStr (OptionsData);
					OptionsStr.exceptions (std::ios::eofbit);
					std::wstring Token;

					while (std::getline (OptionsStr, Token), Token.find (OptionTag) == std::string::npos)
					{
						TRACE (L"FIND_OPTION_TAG " << OptionTag << Token);
					}

					size_t Pos = Token.find (OptionTag);
					std::wistringstream TokenStr (Token);

					//scan for tag
					size_t i = 0;
					wchar_t TokenCh;
					while (!TokenStr.eof () && i < OptionTag.size ())
					{
						TokenCh = TokenStr.get ();
						if (TokenCh == OptionTag[i])
						{
							++i;
						}
						else
						{
							i = 0;
						}
					}

					//scan for '='
					while (!TokenStr.eof ())
					{
						TokenCh = TokenStr.get ();
						if (TokenCh == L'=')
						{
							break;
						}
						else
						{
							continue;
						}
					}

					//load the rest of the line
					std::wstring OptionsData;
					while (!TokenStr.eof ())
					{
						TokenCh = TokenStr.get ();
						OptionsData.push_back (TokenCh);
					}

					//find option and set
					if (OptionTag == L"CODE")
					{
						Option.Code = OptionsData;
					}
					else if (OptionTag == L"DESCRIPTION")
					{
						int j = 0;
						//scan for open quote
						while (OptionsData[j++] != L'\"')
						{
						}

						//copy until close quote
						while (OptionsData[++j - 1] != L'\"')
						{
							Option.Desc.push_back (OptionsData[j - 1]);
						}
					}
					else if (OptionTag == L"TITLE")
					{
						int j = 0;
						//scan for open quote
						while (OptionsData[j++] != L'\"')
						{
						}

						//copy until close quote
						while (OptionsData[++j - 1] != L'\"')
						{
							Option.Title.push_back (OptionsData[j - 1]);
						}
					}
					else if (OptionTag == L"CASH")
					{
						OptionsData.erase (std::remove (OptionsData.begin (), OptionsData.end (), L'\''), OptionsData.end ());
						Option.Cash = std::stol (OptionsData);
					}
					else if (OptionTag == L"CORRUPTION")
					{
						OptionsData.erase (std::remove (OptionsData.begin (), OptionsData.end (), L'\''), OptionsData.end ());
						Option.Corruption = std::stol (OptionsData);
					}
					else if (OptionTag == L"POWER")
					{
						OptionsData.erase (std::remove (OptionsData.begin (), OptionsData.end (), L'\''), OptionsData.end ());
						Option.Power = std::stol (OptionsData);
					}
					else if (OptionTag == L"SELECTION_MSG")
					{
						int j = 0;
						//scan for open quote
						while (OptionsData[j++] != L'\"')
						{
						}

						//copy unyil close quote
						while (OptionsData[++j - 1] != L'\"')
						{
							Option.SelMsg.push_back (OptionsData[j - 1]);
						}
					}
					else if (OptionTag == L"PARTY")
					{
						Option.Party = OptionsData;
					}
					else if (OptionTag == L"GOAL")
					{
						Option.Goal = OptionsData;
					}
					else if (OptionTag == L"GOTO")
					{
						Option.Goto = OptionsData;
					}
					else
					{
						throw std::runtime_error ("OPTION_TAG_INVALID");
					}
				}
				catch (...)
				{
					return 1;
				}
				return 0;
			}
			));
	}

	//join all threads
	for (size_t i = 0; i < Threads.size(); i++)
	{
		Threads[i].join ();
	}
	
	return Option;
}

//tags in dialogue root
static const std::vector<std::wstring> DialogueTags =
{
	L"TYPE",
	L"CODE",
	L"TITLE",
	L"DESCRIPTION",
	L"GOTO"
};

static inline DialogueData ParseDialogue (std::wstring DialogueStr)
{
	//handle options
	std::vector<OptionData> Options;
	std::vector<std::wstring> OptionsStrs = LoadDialogueOptions (DialogueStr);

	for (const auto& OptionStr : OptionsStrs)
	{
		Options.push_back (ParseOption (OptionStr));
	}

	DialogueData Dialogue;
	Dialogue.Options = Options;

	std::vector<std::thread> Threads;

	//scan for each dialogue tag
	for (const auto& DialogueTag : DialogueTags)
	{ 
		//create threads
		Threads.push_back (std::thread (
			[&]() -> int
			{
				try
				{
					std::wistringstream OptionsStr (DialogueStr);
					OptionsStr.exceptions (std::ios::eofbit);
					std::wstring Token;

					//scan for tag
					while (std::getline (OptionsStr, Token), Token.find (DialogueTag) == std::string::npos)
					{
						TRACE (L"FIND_OPTION_TAG " << DialogueTag << Token);
					}

					size_t Pos = Token.find (DialogueTag);
					std::wistringstream TokenStr (Token);

					//scan for tag
					size_t i = 0;
					wchar_t TokenCh;
					while (!TokenStr.eof () && i < DialogueTag.size ())
					{
						TokenCh = TokenStr.get ();
						if (TokenCh == DialogueTag[i])
						{
							++i;
						}
						else
						{
							i = 0;
						}
					}

					//scan for =
					while (!TokenStr.eof ())
					{
						TokenCh = TokenStr.get ();
						if (TokenCh == L'=')
						{
							break;
						}
						else
						{
							continue;
						}
					}

					//load the rest of the line
					std::wstring OptionsData;
					while (!TokenStr.eof ())
					{
						TokenCh = TokenStr.get ();
						OptionsData.push_back (TokenCh);
					}

					if (DialogueTag == L"CODE")
					{
						Dialogue.Code = OptionsData;
					}
					else if (DialogueTag == L"DESCRIPTION")
					{
						int j = 0;
						//scan for open quote
						while (OptionsData[j++] != L'\"')
						{
						}

						//copy until close quote
						while (OptionsData[++j - 1] != L'\"')
						{
							Dialogue.Desc.push_back (OptionsData[j - 1]);
						}
					}
					else if (DialogueTag == L"TITLE")
					{
						int j = 0;
						//scan for open quote
						while (OptionsData[j++] != L'\"')
						{
						}

						//copy until close quote
						while (OptionsData[++j - 1] != L'\"')
						{
							Dialogue.Title.push_back (OptionsData[j - 1]);
						}
					}
					else if (DialogueTag == L"GOTO")
					{
						Dialogue.Goto = OptionsData;
					}
					else if (DialogueTag == L"TYPE")
					{
						if (OptionsData[0] == L'M' && OptionsData[1] == L'E' && OptionsData[2] == L'N' && OptionsData[3] == L'U')
						{
							Dialogue.Type = DialogueData::type::MENU;
						}
						else if (OptionsData[0] == L'M' && OptionsData[1] == L'S' && OptionsData[2] == L'G') // fix end of string 65536
						{
							Dialogue.Type = DialogueData::type::MSG;
						}
						else
						{
							throw std::runtime_error ("DIALOGUE_TYPE_INVALID");
						}
					}
					else
					{
						throw std::runtime_error ("OPTION_TAG_INVALID");
					}
				}
				catch (...)
				{
					return 1;
				}
				return 0;
			}
		));
		
		OptionData Option;

	}

	//join threads
	for (size_t i = 0; i < Threads.size(); i++)
	{
		Threads[i].join ();
	}
	return Dialogue;
}

//wrapper for the ParseDialogue to make it multithreaded
static inline int ParseDialogueThreadable (DialogueData* Out, std::wstring DialogueStr)
{
	try
	{
		*Out = ParseDialogue (DialogueStr);
	}
	catch (const std::exception&)
	{
		return 1;
	}
	return 0;
}

//parse all dialogues in the lsit
static inline std::vector<DialogueData> ParseListDialogues (std::wstring Data)
{
	std::wstring DialogueListData = LoadListDialogues (Data);
	std::vector<std::wstring> DialoguesData = LoadDialogues (DialogueListData);
	std::vector<DialogueData> Dialogues;

	for (auto& DialogueData	: DialoguesData)
	{
		Dialogues.push_back (ParseDialogue (DialogueData));
	}

	return Dialogues;
}

//mt oprimized
static inline std::vector<DialogueData> ParseListDialoguesMT (std::wstring Data)
{
	std::wstring DialogueListData = LoadListDialogues (Data);
	std::vector<std::wstring> DialoguesData = LoadDialogues (DialogueListData);
	std::vector<DialogueData> Dialogues(DialoguesData.size ());
	std::vector<std::thread> Threads;
	//i know volatile does nothing in this context
	for (volatile size_t i = 0; i < DialoguesData.size(); i++)
	{
		Threads.push_back (std::thread (ParseDialogueThreadable, &Dialogues[i], DialoguesData[i]));
	}

	for (volatile size_t i = 0; i < DialoguesData.size (); i++)
	{
		Threads[i].join();
	}

	return Dialogues;
}

#include "../Shared/Exceptions.h"

//parse the datalists, entry point to translation unit
std::vector<Moore::Shared::Dialogue> Moore::Backend::ParseDataLists (std::wstring DataLists) try
{
	std::wstring Docs = LoadDocs (DataLists);
	std::wstring Configuration = LoadConfiguration (DataLists); //nothing is ever done with the config
	std::wstring Data = LoadData (DataLists);

	std::vector<std::wstring> Dialogues = LoadDialogues (Data);

	std::vector<DialogueData> LoadedDialogues = ParseListDialoguesMT (Data);
	std::vector<Moore::Shared::Dialogue> Return;

	for (auto& Dialogue : LoadedDialogues)
	{
		Return.push_back (Moore::Shared::Dialogue (Dialogue));
	}

	return Return;
}
catch (...)
{
	throw Shared::BackendException ("PARSING_ERROR", Shared::BackendException::BackendComponent::DATALISTS_PARSE);
}

/* Program name: Insurrection
* Author: Evan Moore
* Date last updated: 2024-05-06
* Purpose: A text-based choose-your-own-adventure game
*/


#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>


#include <fcntl.h>
#include <stdio.h>
#include <setjmp.h>


#include "Bootstrapper/GetDataListsFileName.h"

#include "Backend/LoadDataLists.h"
#include "Backend/ParseDataLists.h"
#include "Backend/DataLists.h"

#include "Shared/Exceptions.h"
#include "Frontend/DialoguePrinter.h"
#include "Frontend/OptionInputter.h"
#include "Core/DialogueNavigator.h"

extern jmp_buf MainJumper{};

const std::wstring Header = L""
L"\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" L"\n"
L"\u2551                              !!! INSURRECTION !!!                           \u2551" L"\n"
L"\u2551                                                                             \u2551" L"\n"
L"\u2551                 INTELLECUTAL PROPERTY OF MOORE SPECIFICATIONS               \u2551" L"\n"
L"\u2551                                                                             \u2551" L"\n"
L"\u2551                       For best experience, use on Windows (R)               \u2551" L"\n"
L"\u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D" L"\n";

//fancy wait time, busy work
#define QUICKSTART false

#if (QUICKSTART == false) 
#define WAIT(TIME) std::this_thread::sleep_for (std::chrono::milliseconds (TIME))
#define GET_FILE(OUT, IN) Moore::Bootstrapper::GetDataListsFileName (&std::wcout, &std::wcin)
#else
#define WAIT(TIME) NULL
#define GET_FILE(OUT, IN) L"default.txt"
#endif

#if defined(_WIN32)
#include <Windows.h>
#define CLEAR ::system("cls") // clear the console
#include <io.h>
#define SET_MODE _setmode (_fileno (stdout), _O_U16TEXT);
#else
#define CLEAR
#define SET_MODE
#endif

//os specific stuffs
#if defined(_WIN32)
#include <Windows.h>
#define SET_CODEPAGE NULL// ::SetConsoleCP (CP_WINUNICODE); ::SetConsoleOutputCP (CP_WINUNICODE);
#else
#define SET_CODEPAGE NULL
#endif

#ifdef __APPLE__
#error(GET A REAL COMPUTER, NO SUPPORT HERE!)
#endif


#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class Status
{
public:
	const bool Success;
	
	Status ()
		: Success (true)
	{}
};

DLL_EXPORT Status UniversalMain (const std::vector<std::wstring> Args);


int main (int argc, char** argv)
{
	//special character stuffs
	SET_MODE;
	SET_CODEPAGE;

	//convert args
	std::vector<std::string> NarrowArgs;

	for (size_t i = 0; i < argc; i++)
	{
		NarrowArgs.push_back (std::string (argv[i]));
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Converter;
	std::vector<std::wstring> Args;

	for (const auto& NarrowArg : NarrowArgs)
	{
		Args.push_back(Converter.from_bytes (NarrowArg));
	}

	Status s = UniversalMain (Args);

	return !s.Success;
}

int wmain (int argc, wchar_t** argv)
{
	//special chars
	SET_MODE;
	SET_CODEPAGE;

	//convert args
	std::vector<std::wstring> Args;

	for (size_t i = 0; i < argc; i++)
	{
		Args.push_back (std::wstring (argv[i]));
	}

	Status s = UniversalMain (Args);

	return !s.Success;
}

extern std::string Data;

DLL_EXPORT Status UniversalMain (const std::vector<std::wstring> Args)
{


	//set jump buffer, execution will return here always
	switch (::setjmp (MainJumper))
	{
		//finished successfully
		case 1:
			{
				std::wcout << L"Do you want to play again? (y/n)" << std::endl;
				wchar_t opt;
				std::wcin >> opt;
				switch (opt)
				{
					case L'y':
					case L'Y':
						std::wcout << L"NOT IMPLEMENTED" << std::endl;
						::exit (0);
						goto Again;
					case L'n':
					case L'N':
						return Status ();
					default:
						longjmp (MainJumper, 1);
				}
			}
			break;
		//critical error
		case 2:
			{
				std::wcout << L"A critical error has occurred, the program has been reset. Do you want to report this error? (y/n)" << std::endl;
				wchar_t opt;
				std::wcin >> opt;
				switch (opt)
				{
					case L'y':
					case L'Y':
						std::wcout << "To report this error, please contact the support in your documentation." << std::endl;
					case L'n':
					case L'N':
						break;
					default:
						longjmp (MainJumper, 2);
				}
				//play again
				longjmp (MainJumper, 1);
			}
			break;
		//datalists error
		case 3:
			{
				std::wcout << "An error in the specified datalists file has been detected, do you want to reload the program with a shipping datalists file? (y/n)" << std::endl;
				wchar_t opt;
				std::wcin >> opt;
				switch (opt)
				{
					case L'y':
					case L'Y':
						goto Again;
					case L'n':
					case L'N':
						return Status ();
					default:
						longjmp (MainJumper, 1);
				}
			}
			break;
		//set the buffer, no jump, continue flow of execution
		default:
			break;
	}

Again:


	//random init
	srand ((unsigned int) time (0));

	std::wcout << Header << std::endl << std::endl << std::endl;

	WAIT (1500);

	std::wcout << L"Please load the data lists for Insurrection (R) \n"
				  L"Consult you manual for assistance               \n"
				  L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
		<< std::endl;

	WAIT (1500);

//	//get data lists file name
//
//		//load function pointer
//	Moore::Bootstrapper::SetGetDataListsFileName ();
//    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Converter;
//    std::wstring DataListsFile = Converter.from_bytes (Data);//GET_FILE (&std::wcout, &std::wcin);
//
//	std::wcout << L"Loading data lists \n"
//				  L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
//		<< std::endl
//		<< L"[";
//
//	//fake loading time
//	for (size_t i = 0; i < 100; i++)
//	{
//		std::wcout << L".";
//		WAIT (rand () % 100 + 0);
//	}
//
//	std::wcout << L"]" << std::endl
//		<< L"\nData lists loaded\n" 
//		   L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
//		<< std::endl;
//
//	WAIT (500);
//
//	std::wcout << L"\nInitializing program\n"
//				  L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
//		<< std::endl;
//
//	//initialize core and hand control 
//	const auto DataLists = Moore::Backend::DataLists (DataListsFile);
//
//	const auto State = Moore::Backend::StateTable{ 100,100,100 };

	Moore::Backend::DataLists DataLists;

	DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		.SetType(Moore::Shared::Dialogue::type::MENU)
		.SetCode (L"/")
		.SetTitle (L"Choose your political alligence...")
		.SetDesc (L"How do you plan to lie?")
		.AddOption (Moore::Shared::Option ().SetCode (L"KNOW_KNOTHING")
			.SetDesc (L"The party that can neither be confirmed nor denied.")
			.SetTitle (L"The Know-Knothings")
			.SetCash (1000000000l)
			.SetCorruption (40)
			.SetPower (100)
			.SetSelMsg (L"You really do know nothing")
			.SetParty (L"KNOW_KNOTHING")
			.SetGoto (L"POLITICAL_GOAL"))
		.AddOption (Moore::Shared::Option ().SetCode (L"KING_WHIG")
			.SetDesc (L"A group of political enthusiasts who enjoyed debating and strategizing, but often got caught up in the excitement and forgot to actually implement their plans.")
			.SetTitle (L"The King's Whigs")
			.SetCash (5000000000l)
			.SetCorruption (60)
			.SetPower (100)
			.SetSelMsg (L"Powder up that fancy whig of yours")
			.SetParty (L"KING_WHIG")
			.SetGoto (L"POLITICAL_GOAL"))
		.AddOption (Moore::Shared::Option ().SetCode (L"REGRESSIVE")
			.SetDesc (L"A party of idealists who believe that the government can solve all of society's problems - but can't seem to agree on how to do it without raising taxes.")
			.SetTitle (L"The Regressives")
			.SetCash (500000000l)
			.SetCorruption (90)
			.SetPower (100)
			.SetSelMsg (L"You really went back that far?")
			.SetParty (L"REGRESSIVE")
			.SetGoto (L"POLITICAL_GOAL"))
		.AddOption (Moore::Shared::Option ().SetCode (L"BROWN_BACKER")
			.SetDesc (L"The party that thought they could save the world by printing their own money - on toilet paper!")
			.SetTitle (L"The Brown-Backers")
			.SetCash (2000000000l)
			.SetCorruption (30)
			.SetPower (100)
			.SetSelMsg (L"What do you back again?")
			.SetParty (L"BROWN_BACKER")
			.SetGoto (L"POLITICAL_GOAL"))
		.AddOption (Moore::Shared::Option ().SetCode (L"DEPOPULIST")
			.SetDesc (L"A party of idealistic dreamers who thought they could fix all the world's problems by redistributing wealth - but lost it all on them horses.")
			.SetTitle (L"The Depopulists")
			.SetCash (1500000000l)
			.SetCorruption (80)
			.SetPower (100)
			.SetSelMsg (L"You're with George's Guides, right?")
			.SetParty (L"DEPOPULUST")
			.SetGoto (L"POLITICAL_GOAL"))
		.AddOption (Moore::Shared::Option ().SetCode (L"STATES_INFRINGEMENTS")
			.SetDesc (L"Why don't we just call them the 'Party of their confederates'?")
			.SetTitle (L"The State's Infringements")
			.SetCash (250000000)
			.SetCorruption (80)
			.SetPower (100)
			.SetSelMsg (L"You're one infringement.")
			.SetParty (L"STATES_INFRINGEMENTS")
			.SetGoto (L"POLITICAL_GOAL"))
	);

	DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		.SetType (Moore::Shared::Dialogue::type::MENU)
		.SetCode (L"POLITICAL_GOAL")
		.SetTitle (L"Choose your political goals...")
		.SetDesc (L"What do you really want to do?")
		.AddOption (Moore::Shared::Option ().SetCode (L"CONSPIRACY")
			.SetDesc (L"You seek out powerful figures to subvert the government from within.")
			.SetTitle (L"Conspiracy")
			.SetSelMsg (L"You can no longer confirm nor deny anything")
			.SetGoto (L"START1"))
		.AddOption (Moore::Shared::Option ().SetCode (L"COUP")
			.SetDesc (L"You pay the military to do your dirty work")
			.SetTitle (L"Coup de-tat")
			.SetSelMsg (L"Go back to Newburg")
			.SetGoto (L"START1"))
		.AddOption (Moore::Shared::Option ().SetCode (L"REFORM")
			.SetDesc (L"You see the corruption in the political system and want to fix it")
			.SetTitle (L"Reform")
			.SetSelMsg (L"There ain't no reform here!")
			.SetGoto (L"NO_REFORM"))
	);

	 DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MSG)
		 .SetCode (L"NO_REFORM")
		 .SetTitle (L"Do you really think there is any reform here?")
		 .SetDesc (L"BECAUSE REFORM GOT THROWN OUT THE WINDOW BY A SWITCH IN TIME")
		 .SetGoto (L"POLITICAL_GOAL_NOREFORM")
	 );

	 DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"POLITICAL_GOAL_NOREFORM")
		 .SetTitle (L"Be realistic!")
		 .SetDesc (L"There ain't no reform here!")
		 .AddOption (Moore::Shared::Option ().SetCode (L"CONSPIRACY")
			 .SetDesc (L"You seek out powerful figures to subvert the government from within.")
			 .SetTitle (L"Conspiracy")
			 .SetSelMsg (L"You can no longer confirm nor deny anything")
			 .SetGoto (L"START1"))
		 .AddOption (Moore::Shared::Option ().SetCode (L"COUP")
			 .SetDesc (L"You pay the military to do your dirty work")
			 .SetTitle (L"Coup de-tat")
			 .SetSelMsg (L"Go back to Newburg")
			 .SetGoto (L"START1"))
	 );

	 DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MSG)
		 .SetCode (L"START1")
		 .SetTitle (L"President of the Confederated Unions of Acirema")
		 .SetDesc (L"You have just been elected to the presidency of Acirmea by a vote of 372 to 368, the closest margin in history. You need to be extremely cautious with your every decision as to not lose your power.")
		 .SetGoto (L"MILITARY_ACTION"));

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"MILITARY_ACTION")
		 .SetTitle (L"Military action!")
		 .SetDesc (L"Your chief advisor has proposed a plan for you to take power by force. The final plan is hundreds of pages long, but involves the creation of union and local militias and recruitment of the radical youth. After these militias are formed, they will cause chaos with each other on FacePage by swinging insults at each other so as to rope the rest of the public into believing that they are working for the current powerbrokers. Then, we will need to purchase an army of clown cars to disguise these youths so they can storm Parliment with their sharpened cardboard signs.")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"ACCEPT")
			 .SetCash (-10000000)
			 .SetPower (-20)
			 .SetCorruption (20)
			 .SetTitle (L"Accept")
			 .SetSelMsg (L"You have just accepted the proposal and have recruited tens of thousands of youths. Massive success was found in the first and second phases of the plan as thousands of insults had rounded the FacePage network. However, the clown car distributor was somewhat suspicious about your order of one thousand armored cars, and he reported you to the media. The headline 'New President Using Clown Cars to Promote National Clown Remembrance Day' made the front pages of all the major papers. The new insults that are being thrown are at the poor distributor for not thinking this was one big April Fools Day joke.")
			 .SetGoto (L"PUBLIC_DECEPTION")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"REJECT")
			 .SetCash (0)
			 .SetPower (10)
			 .SetCorruption (0)
			 .SetTitle (L"REJECT")
			 .SetSelMsg (L"You rejected the proposal, and the chief advisor has just walked out screaming 'IF YOU WONT DO IT, THEN I WILL'. For the first time in history, he actually carried out his threat and began to implement the plan. While he enjoyed minor success with the insults, he found a source of one thousand clown cars and fitted them with armor plating and clown flags. You now find yourself stuck in the Blue House with thousands of clowns marching towards the front doors. Your advisor forgot, however, to actually arm these clowns so all they have is party poppers and ballon dogs. The next day comes and all that is left of the last day is the headline 'APRIL FOOLS CLOWNS TRY TO OVERTHROW THE GOVERNMENT WITH SILLY STRING'.")
			 .SetGoto (L"PUBLIC_DECEPTION")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"PUBLIC_DECEPTION")
		 .SetTitle (L"Public Deception")
		 .SetDesc (L"As you are too fancy to read the daily news, you send the Secretary of Waste Management Systems to get the weekly news. When you read the headlines, you realize that you are losing approval ratings quickly. It turns out that the secretary has a solution! Back up the waste pipes and televise your unblocking it and make you the hero.")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"ACCEPT")
			 .SetCash (-20000000)
			 .SetPower (-20)
			 .SetCorruption (30)
			 .SetTitle (L"Accept")
			 .SetSelMsg (L"You decide to go along with this plan. You send the lonely janitor to flip the switch labeled 'STOP SEWAGE DISSAPATION SYSTEMS' and you immediately smell a horrible odor. It seems that you plan is working better than expected, however, all of a sudden, your Vice President comes running in and shouts that the house across the street has just started dumping sewage onto the Blue House Lawn. You decide to not look at the mess and wait about two hours for the protests to gather. As you walk out, you suddenly see cardboard signs stating, 'BROWN BACKERS ARE RESPONSIBLE!' and 'DOWN WITH THE BROWN BACKERS'. You then go to the Civil Waste Union with a spear in hand as you start poking out the 'clog', but on the fourth swing, a huge amount of waste splashes onto your expensive suit and ruins it. You then call the secretary to flip the switch back, and you are greeted to cheers from the crowd for clearing the sewage. The next day, you pick up the daily paper to see the headline 'PRESIDENT DRENCHED IN SEWAGE AFTER ORDERING THE WASTE SYSTEM TURNED OFF, GETS WHAT HE DESERVED!'. What a disaster.")
			 .SetGoto (L"ALIENS")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"REJECT")
			 .SetCash (0)
			 .SetPower (50)
			 .SetCorruption (0)
			 .SetTitle (L"Reject")
			 .SetSelMsg (L"You find the plan to be completely ridiculous and immediately fire the idiot who proposed it to you. However, you immediately regret your decision when the World Class Thrown backs up and he is nowhere to call for help. You then find out that he flipped the switch labeled 'BACK UP SEWAGE' and the entire city is overflowing. It seems that the switch wont go back, even as you pull with all of your executive might. You decide to call the media and report the action to them and they respond by placing the headline 'GINHSAW OVERFLOWS WHILE BH JANITOR FIDDLES'. The cardboard signs soon follow suit demanding the arrest of the janitor. You send the Federal Security Bureau to storm his residence and stage a trial. He is found guilty of High Treason amongst other crimes against Acirema. The people adore you for throwing away that janitor and clearing out the sewage.")
			 .SetGoto (L"ALIENS")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"ALIENS")
		 .SetTitle (L"'Issues' from Area 72")
		 .SetDesc (L"You are sitting comfortably in your LazyChair when a black suited agent of Extraterrestrial Affairs approaches you. You are wondering what on Earth this nut is going to say now when he brings one of the captured aliens in front of you and ominously asks, 'Do you want to win this December?'. He says that he will release this alien on live television to promote your 'Enough Said!' agenda to the extraterrestrial audience. The plan looks foolproof.")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"ACCEPT")
			 .SetCash (-50000000)
			 .SetPower (-50)
			 .SetCorruption (20)
			 .SetTitle (L"Accept this foolproof plan")
			 .SetSelMsg (L"You decide to go along with the plan, but you suddenly wonder how on Earth this alien will help your campaign, seeing as aliens can not even vote. Whatever, that's a problem for the Minister of Election Management. You then take the alien into the press room and give him a pre-prepared speech to read to the public. The camera is turned on and before you have time to think, the alien delivers it to the public. All of a sudden, the telephones start ringing like crazy, making you suspicious. You realize that you sent the broadcast to the PEOPLE instead of the aliens, and now they know that aliens are real. Fifty thousand youths lead the march for Alien Recognition to the front door of the Blue House, demanding that the government release all records locked in the Extraterrestrial Office of Information Management. This is a complete disaster.")
			 .SetGoto (L"PUBLIC_TERROR")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"REJECT")
			 .SetCash (0)
			 .SetPower (75)
			 .SetCorruption (30)
			 .SetTitle (L"Reject")
			 .SetSelMsg (L"You decide to vaporize this nut and his alien for the absurd plan. However, instead of removing the two of them, they are immediately transported to the planet of Aracadius where the native aliens proclaim the five hundredth and forty third coming. It seems that the aliens know that you are responsible and will use their telepathy powers on the people to get them to vote for you this December. This is a massive win.")
			 .SetGoto (L"PUBLIC_TERROR")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"PUBLIC_TERROR")
		 .SetTitle (L"Public Terror")
		 .SetDesc (L"The Secretary of A Safe Land is suddenly screaming about something. You walk to his office and find him with a stack of pages in his hands. He calmly explains that these pages are a plan to grab power from the public by fear. Initially, these plans sounded too extreme, but after a long explanation, you start to understand that they may just be what you need. Do you want to accept the plan?")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"ACCEPT")
			 .SetCash (-20000000)
			 .SetPower (30)
			 .SetCorruption (50)
			 .SetTitle (L"Accept")
			 .SetSelMsg (L"You have decided to accept the plan. You have one problem already, however: you need to decide which landmark you are going to destroy and blame on terrorists. After a long contemplation, you decide to choose the Porcelain Throne of Selliasrev. While the locals see the monument as a disgrace and would rejoice in its destruction, the rest of the country travels from far and wide to visit it. You now search the local chat forums for a truck bomber that will have the guts to blow up the beautiful masterpiece and stumble upon a man by the pseudonym of Tapuajsiew. The day of the attack, you make sure that all of the national media conglomerates have their cameras pointed and that you have an alibi. On the fateful day, you watch the television, and low and behold, the throne comes toppling down, with the sealed sludge spilling into the streets. The cardboard signs are once again out with the slogan 'ABOUT TIME', as the locals have been petitioning the local government for its demolition. The rest of the country is in shock, however, and they immediately back your Stop Porcelain Terrorism Bill.")
			 .SetGoto (L"SECRET_SOCIETY")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"REJECT")
			 .SetCash (0)
			 .SetPower (50)
			 .SetCorruption (20)
			 .SetTitle (L"Reject")
			 .SetSelMsg (L"You inquire about the plans, and when it becomes clear that he won't divulge any more details until you accept, you decide to have him drug out by the collar. In defiance, he throws a handful of salt into your eyes, blinding you, and runs out faster than a Flying Fortress at top speed. The next day, you see on the news the images of a debris pile that used to be the Porcelain Throne of Selliasrev. The commentators are now saying that your own supporters were behind the attack and that you may have been responsible. The people have shown up to your front door demanding answers.")
			 .SetGoto (L"OH_CRAP")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"OH_CRAP")
		 .SetTitle (L"Whoopsie!")
		 .SetDesc (L"The people believe that you are the one responsible for the bombing of the throne, and they mean business. What do you tell them?")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"WAS_ME")
			 .SetCash (0)
			 .SetPower (-80)
			 .SetCorruption (30)
			 .SetTitle (L"That was me!")
			 .SetSelMsg (L"You peacefully tell them that it was not you who attacked the throne, but they 'know better'. They then recall enough members of the House of United Delegates to push through a vote to impeach. You somehow survive a vote to convict in the Council of Confederated Delegates, however.")
			 .SetGoto (L"SECRET_SOCIETY")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"WASNT_ME")
			 .SetCash (0)
			 .SetPower (-90)
			 .SetCorruption (20)
			 .SetTitle (L"Of course it wasn't me!")
			 .SetSelMsg (L"You told them that it was you who was responsible for the attack. They grab you by the neck and forcibly remove you from the presidency, but before they succeed, a fierce bolt of Lightning shatters through the crowd and the Zineese have arrived. They tell the people the truth and command them to go home. What on earth just happened, you ask yourself as you wake from a coma, somehow still with the powers of the Presidency.")
			 .SetGoto (L"SECRET_SOCIETY")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"SECRET_SOCIETY")
		 .SetTitle (L"Who are these people?")
		 .SetDesc (L"You are relaxing on the sofa watching Lockmat when all of a sudden, the room fills with smoke. Three men in dark suits appear from the cloud and reveal themselves as members of the Order of the Itanimulli. They want one thing, consent. They tell you that you can either choose to follow their every word and become a puppet for them or they will remove you from the Presidency.")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"JOIN")
			 .SetCash (100000000)
			 .SetPower (-20)
			 .SetCorruption (500)
			 .SetTitle (L"Join")
			 .SetSelMsg (L"You decide to follow along with their demands. They then pass a stack of pages to you detailing Operation S'doohwatron, the plan to stage a mass uprising by using the elderly to shout at them kids to follow along with the agenda. In a flash, they disappear, and you now have to find the octogenarians to use. After looking for hours, you found the Sunshine Paradise in Delaganda. You begin recruiting them and before you know it, there are over one thousand on your side. After giving them one thousand dollars a piece to purchase supplies, you eagerly wait for them to return. However, instead of returning, they pack their bags and run to Adanaca. It seems that the Sunshine Paradise was the name of an island prison housing the most dangerous criminals known to mankind. Whoopsie!")
			 .SetGoto (L"CIVIL_WAR")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"REFUSE")
			 .SetCash (0)
			 .SetPower (-30)
			 .SetCorruption (-20)
			 .SetTitle (L"Absolutely Not!")
			 .SetSelMsg (L"You tell them to get lost. They pull the rug out from under you and grab you by the feet and drag you into a room marked Office 93. There lays a machine that they place you into, and before you know it, they have activated it and you suddenly grew a strong sweet tooth for chocolate milk. You run as fast as you can to the cafeteria and steal a little carton of milk and drink it quickly. It looks like they're gone. That was weird!")
			 .SetGoto (L"CIVIL_WAR")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (
		 Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MENU)
		 .SetCode (L"CIVIL_WAR")
		 .SetTitle (L"Civil War")
		 .SetDesc (L"Acirema is on the verge of Civil War! It seems that the divisions are caused by two major coalitions, the Freedom Fight Against Dog Leashes and the Victims For Dog Attack Justice League. It all started when a lady by the name of Edith was walking down the street, when all of a sudden, a door-to-door salesman approaches her and explains the benefits of SuperEars, a hearing aid company. They have that perfect in-the-ear canal design she was told. Well, because she couldn't say no to nobody, she spends several thousand on a pair of hearing aids. After she was done with business, she walks across the street to the dry cleaners where she picks up her load of laundry. It is at this moment that a wild chihuahua runs up to her and bits her leg. She screams in agony. Later that day, she told her husband about the incident, and he called his legal team, Rabinowitz, Rabinowitz, and Rabinowitz. They inform him that they will take the case up to the Super-Duper Court of All Courts. Unfortunately for the old lady, she lost the landmark case, prompting national outrage. Soon, both sides started recruiting militaries of dogs to fight each other and show whether leashes are really needed or not. What do you do? ")
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"TACTICAL_NUKE")
			 .SetCash (-10000000)
			 .SetPower (-10)
			 .SetCorruption (200)
			 .SetTitle (L"Nuke the protesters!")
			 .SetSelMsg (L"This is the Big One, you tell the Disjointed Captains of Staff, 'Just Nuke 'Em!'. They quickly told you you're crazy and that you don't belong in the Presidency. They quickly stage a coup to have you overthrown when all of a sudden, an army of angry chihuahuas runs up to them and immobilizes them for long enough that you can get away from them. Why did you have to say that n-word?")
			 .SetGoto (L"~")
		 )
		 .AddOption (
			 Moore::Shared::Option ()
			 .SetCode (L"FIGHT_IT_OUT")
			 .SetCash (0)
			 .SetPower (20)
			 .SetCorruption (50)
			 .SetTitle (L"Let them fight!")
			 .SetSelMsg (L"You decide to just let the protesters to fight it out. They decide on March 4th to take their armies of dogs and brawl it out n one giant free for all. On the fateful day, however, massive storms arrived at Rock Oil Stadium, prompting the event organizers to shut it down. Not to be deterred, the belligerents took it outside in the rain and done it the old fashioned way. After hours of fighting, the Victims For Dog Attack Justice League took the victory and established universal leash laws across all seventy unions of the federation.")
			 .SetGoto (L"~")
		 )
	 );

	 DataLists.Dialogues.Dialogues.push_back (Moore::Shared::Dialogue ()
		 .SetType (Moore::Shared::Dialogue::type::MSG)
		 .SetGoto (L"~")
		 .SetCode (L"~"));


	Moore::Core::DialogueNavigator::Initialize (DataLists);
	

	return Status ();
}

/*


			Moore::Shared::Option ().SetCode (L"KNOW_KNOTHING")
				.SetDesc (L"The party that can neither be confirmed nor denied.")
				.SetTitle (L"The Know-Knothings")
				.SetCash (1000000000l)
				.SetCorruption (40)
				.SetPower (100)
				.SetSelMsg (L"You really do know nothing")
				.SetParty (L"KNOW_KNOTHING")
				.SetGoto (L"POLITICAL_GOAL"),
			Moore::Shared::Option ().SetCode (L"KING_WHIG")
				.SetDesc (L"A group of political enthusiasts who enjoyed debating and strategizing, but often got caught up in the excitement and forgot to actually implement their plans.")
				.SetTitle (L"The King's Whigs")
				.SetCash (5000000000l)
				.SetCorruption (60)
				.SetPower (100)
				.SetSelMsg (L"Powder up that fancy whig of yours")
				.SetParty (L"KING_WHIG")
				.SetGoto (L"POLITICAL_GOAL"),
			Moore::Shared::Option ().SetCode (L"REGRESSIVE")
				.SetDesc (L"A party of idealists who believe that the government can solve all of society's problems - but can't seem to agree on how to do it without raising taxes.")
				.SetTitle (L"The Regressives")
				.SetCash (500000000l)
				.SetCorruption (90)
				.SetPower (100)
				.SetSelMsg (L"You really went back that far?")
				.SetParty (L"REGRESSIVE")
				.SetGoto (L"POLITICAL_GOAL"),
			Moore::Shared::Option ().SetCode (L"BROWN_BACKER")
				.SetDesc (L"The party that thought they could save the world by printing their own money - on toilet paper!")
				.SetTitle (L"The Brown-Backers")
				.SetCash (2000000000l)
				.SetCorruption (30)
				.SetPower (100)
				.SetSelMsg (L"What do you back again?")
				.SetParty (L"BROWN_BACKER")
				.SetGoto (L"POLITICAL_GOAL"),
			Moore::Shared::Option ().SetCode (L"DEPOPULIST")
				.SetDesc (L"A party of idealistic dreamers who thought they could fix all the world's problems by redistributing wealth - but lost it all on them horses.")
				.SetTitle (L"The Depopulists")
				.SetCash (1500000000l)
				.SetCorruption (80)
				.SetPower (100)
				.SetSelMsg (L"You're with George's Guides, right?")
				.SetParty (L"DEPOPULUST")
				.SetGoto (L"POLITICAL_GOAL"),
			Moore::Shared::Option ().SetCode (L"STATES_INFRINGEMENTS")
				.SetDesc (L"Why don't we just call them the 'Party of their confederates'?")
				.SetTitle (L"The State's Infringements")
				.SetCash (250000000)
				.SetCorruption (80)
				.SetPower (100)
				.SetSelMsg (L"You're one infringement.")
				.SetParty (L"STATES_INFRINGEMENTS")
				.SetGoto (L"POLITICAL_GOAL")
*/
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

	//get data lists file name

		//load function pointer
	Moore::Bootstrapper::SetGetDataListsFileName ();

	const std::wstring DataListsFile = GET_FILE (&std::wcout, &std::wcin);

	std::wcout << L"Loading data lists \n"
				  L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
		<< std::endl
		<< L"[";

	//fake loading time
	for (size_t i = 0; i < 100; i++)
	{
		std::wcout << L".";
		WAIT (rand () % 100 + 0);
	}

	std::wcout << L"]" << std::endl
		<< L"\nData lists loaded\n" 
		   L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
		<< std::endl;

	WAIT (500);

	std::wcout << L"\nInitializing program\n"
				  L"\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D"
		<< std::endl;

	//initialize core and hand control 
	const auto DataLists = Moore::Backend::DataLists (DataListsFile);

	const auto State = Moore::Backend::StateTable{ 100,100,100 };

	Moore::Core::DialogueNavigator::Initialize (DataLists);
	

	return Status ();
}
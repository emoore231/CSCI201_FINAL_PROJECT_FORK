#include "GetDataListsFileName.h"

#include <vector>
#include <string>
//#include <filesystem>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma region PLATFORM_SPECIFIC

#if defined(_WIN32)
#include <windows.h>

//list all files in working directory because c++14 doesnt have this functionality
static inline std::vector<std::wstring> GetFilesInWorkingDir ()
{
	std::wstring Path = L".";

	//handle crap
	WIN32_FIND_DATAW FindData;
	HANDLE hFind = FindFirstFileW ((Path + L"/*").c_str (), &FindData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error (std::string () + "ERROR: WIN32 INVALID HANDLE @ GetValidDataListsFiles() in Bootstrapper/GetValidDataListsFiles.cpp");
	}

	std::vector<std::wstring> Files;

	//list all files
	do
	{
		std::wstring CurrentFileName = FindData.cFileName;
		if (CurrentFileName != L"." && CurrentFileName != L".." && !(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Files.push_back (CurrentFileName);
		}
	}
	while (FindNextFileW (hFind, &FindData));

	FindClose (hFind);

	return Files;
}

#else // assume linux
static inline std::vector<std::wstring> GetFilesInWorkingDir () 
{
	return std::vector<std::wstring>();
}
#endif

#include <locale>
#include <codecvt>


#pragma endregion PLATFORM_SPECIFIC

//validate the file has the magic word
static inline bool ValidateDataListsMagicWord (const std::wstring FileName)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Converter;
	auto FileNarrow = Converter.to_bytes(FileName);
	std::wfstream File (FileNarrow.c_str());
	std::wstring MagicWord;
	std::getline (File, MagicWord);

	if (MagicWord == L"$$MOORE-INSURRECTION-STANDARD-DATA-FORMAT$$") // must be newline after the magic word in file, MUST BE EXACT
	{
		File.close ();
		return true;
	}
	else
	{
		File.close ();
		return false; 
	}
}

//list all valid datalists files in the work dir
static inline std::vector<std::wstring> GetValidDataListsFiles ()
{
	const auto FileNames = GetFilesInWorkingDir ();

	std::vector<std::wstring> DataListsFileNames;

	//validate
	for (const auto& FileName : FileNames) try
	{
		if (ValidateDataListsMagicWord (FileName))
		{
			DataListsFileNames.push_back (FileName);
		}
	}
	catch (...)
	{
		continue; // normal for code to throw here randomly
	}
	return DataListsFileNames;
}

//user choose load method
static inline int ChooseLoadingMethod (std::wostream* Out, std::wistream* In)
{
	int Choice;
	while (true) try
	{
		*Out << L"Load data lists file:" << std::endl
			<< L"[1] Load default file" << std::endl
			<< L"[2] Load from filename" << std::endl;

		std::wstring Line;
		std::getline (*In, Line);
		std::wistringstream LineStream (Line);
		LineStream >> Choice;
		if (Choice == 1 || Choice == 2)
		{
			return --Choice;
		}
	}
	catch (...)
	{
		*Out << "Invalid option" << std::endl;
		continue;
	}
}

//load by filename, or number
std::wstring Moore::Bootstrapper::GetDataListsFileNameImplemented (std::wostream* Out, std::wistream* In)
{
	while (true)
	{
		if (!ChooseLoadingMethod (Out, In))
			// load from shipping version
		{
			const auto FileNames = GetValidDataListsFiles ();

			if (!FileNames.size ())
			{
				*Out << "No files found, load by filename or CTRL+C to quit" << std::endl;
				// goto 
			}

			*Out << L"Please select a file from the list" << std::endl;

			//list files
			int i = 0;
			for (const auto& FileName : FileNames)
			{
				*Out << L"[" << ++i << L"] " << FileName << std::endl;
			}

			//select file
			try
			{
				int Choice;
				std::wstring Line;
				std::getline (*In, Line);
				std::wistringstream LineStream (Line);
				LineStream >> Choice, Choice--;

				if (Choice < 0 || Choice > FileNames.size() - 1)
				{
					throw std::runtime_error("INVALID CHOICE");
				}

				return FileNames[Choice];
			}
			catch (const std::exception&)
			{
				*Out << "Invalid option" << std::endl;
				continue;
			}
		}
		else
			//load by filename
		{
			try
			{
				*Out << L"Enter data lists file path" << std::endl;
				std::wstring Path;
				std::getline (*In, Path);
				if (!ValidateDataListsMagicWord (Path))
				{
					throw std::runtime_error ("INVALID_FILE");
				}

				return Path;
			}
			catch (const std::exception&)
			{
				*Out << L"File name invalid or file invalid" << std::endl;
			}
		}
	}
}

extern std::wstring (*Moore::Bootstrapper::GetDataListsFileNameImpl) (std::wostream* Out, std::wistream* In) = nullptr;
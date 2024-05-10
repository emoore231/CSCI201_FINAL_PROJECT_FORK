#include "LoadDataLists.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>


//load teh datalists file into a string
std::wstring Moore::Backend::LoadDataListsFile (const std::wstring FileName)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Converter;
	auto File = Converter.to_bytes(FileName);
	std::wfstream DataLists (File.c_str());
	std::wstringstream DataListsData;
	DataListsData << DataLists.rdbuf ();

	DataLists.close ();

	return DataListsData.str();
}


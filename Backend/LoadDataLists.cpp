#include "LoadDataLists.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>



//load teh datalists file into a string
std::wstring Moore::Backend::LoadDataListsFile (const std::wstring FileName)
{
	std::wfstream DataLists (FileName);
	std::wstringstream DataListsData;
	DataListsData << DataLists.rdbuf ();

	DataLists.close ();

	return DataListsData.str();
}


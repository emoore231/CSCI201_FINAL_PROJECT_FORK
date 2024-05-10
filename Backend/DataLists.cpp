#include "DataLists.h"

#include "ParseDataLists.h"
#include "LoadDataLists.h"

Moore::Backend::DataLists::DataLists (std::wstring FileName)
	: Dialogues (ParseDataLists (Moore::Backend::LoadDataListsFile (FileName))), Config () // not implemented
{

}

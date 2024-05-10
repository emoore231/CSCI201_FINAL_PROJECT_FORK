#include "Option.h"

Moore::Shared::Option::Option ()
	: Code (L"INIT"), Title (), Desc (), SelMsg (), Party (), Goal (), Goto (), Cash (), Corruption (), Power ()
{}

Moore::Shared::Option::Option (const std::wstring code, const std::wstring title, const std::wstring desc, const std::wstring selMsg, const std::wstring party, const std::wstring goal, const std::wstring _goto, const long cash, const long corruption, const long power)
	: Code (code), Title (title), Desc (desc), SelMsg (selMsg),
	Party (party), Goal (goal), Goto (_goto), Cash (cash),
	Corruption (corruption), Power (power)
{}
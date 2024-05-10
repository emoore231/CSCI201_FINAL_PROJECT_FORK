#include "OptionInputter.h"

int Moore::Frontend::GetOptionImplemented (const Backend::StateTable st, const Shared::Dialogue d)
{
	//store valid options
	std::vector<bool> ValidOptions;

	//test for validity
	for (int i = 0; i < d.Options.size (); i++)
	{
		bool Valid{ true };
		if (d.Options[i].Cash < 0l && st.Cash < -d.Options[i].Cash) Valid = false;
		if (d.Options[i].Power < 0l && st.Power < -d.Options[i].Power) Valid = false;
		if (d.Options[i].Corruption < 0l && st.Corruption < -d.Options[i].Corruption) Valid = false;
		ValidOptions.push_back (Valid);
	}

	//print and input
	std::wcout << L"Choose an option: ";

	int Opt;
	while (true)
	{
		std::wstring Line;
		std::getline (std::wcin, Line);
		std::wcout << std::endl;
		std::wistringstream LineStr (Line);
		LineStr.exceptions (std::ios::failbit | std::ios::badbit);

		try
		{
			LineStr >> Opt;
			if (Opt < 1 || Opt > d.Options.size ()) throw std::runtime_error ("INV_INP");
			if (ValidOptions[--Opt]) break; else throw std::runtime_error ("OPT_UNV");
		}
		catch (const std::exception&)
		{
			//retry if unavailale
			std::wcout << L"The option you selected is invalid, please select an available option from the list." << std::endl;
		}
	}
	return Opt;
}

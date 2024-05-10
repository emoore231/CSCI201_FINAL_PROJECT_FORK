#include "DialoguePrinter.h"

//os specific stuffs
#if defined(_WIN32)
#include <Windows.h>
#define CLEAR ::system("cls") // clear the console
#else
#define CLEAR
#endif

void Moore::Frontend::DialoguePrinterImplemented (const Shared::Dialogue d, const Backend::StateTable st)
{
	CLEAR;

	//print state
	std::wcout << L"CURRENT STATS: " << std::endl << std::endl
		<< L"POLITICAL PARTY: " << st.Party << std::endl
		<< L"GOAL: " << st.Goal << std::endl
		<< L"CASH: " << st.Cash << std::endl
		<< L"POWER: " << st.Power << std::endl
		<< L"CORRUPTION: " << st.Corruption << std::endl << std::endl;

	std::wcout << d.Title << std::endl << std::endl;
	std::wcout << '\t' << d.Desc << std::endl << std::endl;

	//print each option
	for (int i = 0; i < d.Options.size (); i++)
	{
		std::wcout << L"[" << i + 1 << L"] : " << d.Options[i].Title << std::endl;
		//unavailability messages
		if (d.Options[i].Cash < 0l && st.Cash < -d.Options[i].Cash) std::wcout << L"OPTION UNAVAILABLE: NOT ENOUGH CASH" << std::endl;
		if (d.Options[i].Power < 0l && st.Power < -d.Options[i].Power) std::wcout << L"OPTION UNAVAILABLE: NOT ENOUGH POWER" << std::endl;
		if (d.Options[i].Corruption < 0l && st.Corruption < -d.Options[i].Corruption) std::wcout << L"OPTION UNAVAILABLE: NOT ENOUGH CORRUPTION" << std::endl;

		//option description
		std::wcout << d.Options[i].Desc << std::endl
			<< (d.Options[i].Cash > 0 ? "CASH GAIN: " : "CASH REQUIRED: ") << (d.Options[i].Cash > 0 ? d.Options[i].Cash : -d.Options[i].Cash) << std::endl
			<< (d.Options[i].Power > 0 ? "POWER GAIN: " : "POWER REQUIRED: ") << (d.Options[i].Power > 0 ? d.Options[i].Power : -d.Options[i].Power) << std::endl
			<< (d.Options[i].Corruption > 0 ? "CORRUPTION GAIN: " : "CORRUPTION REQUIRED: ") << (d.Options[i].Corruption > 0 ? d.Options[i].Corruption : -d.Options[i].Corruption) << std::endl << std::endl;

	}

}

//print message
void Moore::Frontend::MessagePrinterImplemented (const Shared::Dialogue d, const Backend::StateTable st)
{
	CLEAR;
	//state
	std::wcout << L"CURRENT STATS: " << std::endl << std::endl
		<< L"POLITICAL PARTY: " << st.Party << std::endl
		<< L"GOAL: " << st.Goal << std::endl
		<< L"CASH: " << st.Cash << std::endl
		<< L"POWER: " << st.Power << std::endl
		<< L"CORRUPTION: " << st.Corruption << std::endl << std::endl;
	//message content
	std::wcout << d.Title << std::endl << std::endl;
	std::wcout << '\t' << d.Desc << std::endl << std::endl << L"PRESS ENTER TO CONTINUE" << std::endl;

	//wait for enter
	std::wstring a;
	std::getline (std::wcin, a);
}

//print selection message
void Moore::Frontend::SelMessagePrinterImplemented (const std::wstring SelMsg, const Backend::StateTable st)
{
	CLEAR;

	//state
	std::wcout << L"CURRENT STATS: " << std::endl << std::endl
		<< L"POLITICAL PARTY: " << st.Party << std::endl
		<< L"GOAL: " << st.Goal << std::endl
		<< L"CASH: " << st.Cash << std::endl
		<< L"POWER: " << st.Power << std::endl
		<< L"CORRUPTION: " << st.Corruption << std::endl << std::endl;
	//print message
	std::wcout << L"ALERT!" << std::endl << std::endl;
	std::wcout << '\t' << SelMsg << std::endl << std::endl << L"PRESS ENTER TO CONTINUE" << std::endl;

	//wait for enter
	std::wstring a;
	std::getline (std::wcin, a);
}

#pragma once

#include <string>
#include <iostream>

namespace Moore
{
	namespace Shared
	{
		class Option;
	}
}

class Moore::Shared::Option
{
public:
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

	Option& SetCode(std::wstring lhs) { Code = lhs; return *this; }
	Option& SetTitle(std::wstring lhs) { Title = lhs; return *this; }
	Option& SetDesc(std::wstring lhs) { Desc = lhs; return *this; }
	Option& SetSelMsg(std::wstring lhs) { SelMsg = lhs; return *this; }
	Option& SetParty(std::wstring lhs) { Party = lhs; return *this; }
	Option& SetGoal(std::wstring lhs) { Goal = lhs; return *this; }
	Option& SetGoto(std::wstring lhs) { Goto = lhs; return *this; }

	Option& SetCash(long lhs) { Cash = lhs; return *this; }
	Option& SetCorruption (long lhs) { Corruption = lhs; return *this; }
	Option& SetPower (long lhs) { Power = lhs; return *this; }

public:
	Option ();

	Option (
		const std::wstring code,
		const std::wstring title,
		const std::wstring desc,
		const std::wstring selMsg,
		const std::wstring party,
		const std::wstring goal,
		const std::wstring _goto,
		const long cash,
		const long corruption,
		const long power
	);

	//debug tracing
	template <typename Char, typename CharTraits>
	friend std::basic_ostream<Char, CharTraits>& operator<< (std::wostream& lhs, const Option& rhs)
	{
		lhs << L"OPTION_TRACING:" << std::endl
			<< L"    CODE: " << rhs.Code << std::endl
			<< L"    TITLE: " << rhs.Title << std::endl
			<< L"    DESCRIPTION: " << rhs.Desc << std::endl
			<< L"    SELECTION_MESSAGE: " << rhs.SelMsg << std::endl
			<< L"    PARTY: " << rhs.Party << std::endl
			<< L"    GOAL: " << rhs.Goal << std::endl
			<< L"    GOTO: " << rhs.Goto << std::endl
			<< L"    CASH: " << rhs.Cash << std::endl
			<< L"    CORRUPTION: " << rhs.Corruption << std::endl
			<< L"    POWER: " << rhs.Power << std::endl;
		return lhs;
	}
};
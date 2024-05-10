#pragma once

#include <string>
#include <iostream>
#include <vector>

#include ".\Option.h"

namespace Moore
{
	namespace Shared
	{
		class Dialogue;
	}
}

//final storage for dialogues
class Moore::Shared::Dialogue
{
public:
	enum class type
	{
		MENU,
		MSG
	} Type;

	std::wstring Code;
	std::wstring Title;
	std::wstring Desc;
	std::wstring Goto;
	std::vector<Option> Options;

	Dialogue ();

	Dialogue
	(
		const type _Type,
		const std::wstring _Code,
		const std::wstring _Title,
		const std::wstring _Desc,
		const std::wstring _Goto,
		const std::vector<Option> _Options
	);

	//debug tracing
	template <typename Char, typename CharTraits>
	friend std::basic_ostream<Char, CharTraits>& operator<< (std::wostream& lhs, const Dialogue& rhs)
	{
		lhs << L"DIALOGUE_TRACING:" << std::endl
			<< L"    CODE: " << rhs.Code << std::endl
			<< L"    TITLE: " << rhs.Title << std::endl
			<< L"    DESCRIPTION: " << rhs.Desc << std::endl
			<< L"    GOTO: " << rhs.Goto << std::endl;

		for (auto& Option : Options)
		{
			lhs << Option;
		}

		return lhs;
	}
};
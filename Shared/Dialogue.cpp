#include "Dialogue.h"

Moore::Shared::Dialogue::Dialogue ()
	: Type (), Code (), Title (), Desc (), Goto (), Options ()
{}

Moore::Shared::Dialogue::Dialogue (const type _Type, const std::wstring _Code, const std::wstring _Title, const std::wstring _Desc, const std::wstring _Goto, const std::vector<Option> _Options) :
	Type (_Type), Code (_Code), Desc (_Desc), Goto (_Goto), Options (_Options), Title(_Title)
{}

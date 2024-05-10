#include "ResultsPrinter.h"

//os specific stuffs
#if defined(_WIN32)
#include <Windows.h>
#define CLEAR ::system("cls") // clear the console
#else
#define CLEAR
#endif

#include <iostream>

//print the results
void Moore::Frontend::PrintResultsImplemented (Result r, Backend::StateTable st)
{
	CLEAR;

	std::wcout << L"END OF GAME" << std::endl << std::endl;

	//final result
	switch (r)
	{
		case Moore::Frontend::Result::BROKE:
			{
				std::wcout << L"BROKE - You ran out of money, and were no longer able to pay your special interests. They are no longer backing you, and without their support, you quickly lost a recall election. Your vice president it seems has gained their favor and has turned his back on you. You are now known as 'The Hole Digger' from all of the smear campaigns your old allies have started against you for digging your own hole." << std::endl;
			}
			break;
		case Moore::Frontend::Result::POWERLESS:
			{
				std::wcout << L"POWERLESS - You ran out of power. The first rule of the presidency is that you need power, and without it, you are as useless as the 'Cotton Lint in Founder Adam's Pocket'. You were no longer able to sway the forces that were, and they swiftly destroyed you. You have since been imprisoned on Hiker's Island for charges of all kinds, and you will never see the sun again." << std::endl;
			}
			break;
		case Moore::Frontend::Result::NO_CORRUPTION:
			{
				std::wcout << L"NO CORRUPTION - It seems that your generosity has gotten the better of you and you were unwilling to take the means to justify the ends. You were no longer able to sway the forces that were, and they swiftly destroyed you. You have since been imprisoned on Hiker's Island for charges of all kinds, and you will never see the sun again." << std::endl;
			}
			break;
		case Moore::Frontend::Result::DEAD:
			{
				std::wcout << L"DEAD - You died." << std::endl;
			}
			break;
		case Moore::Frontend::Result::DETHROWNED:
			{
				std::wcout << L"DETHROWNED - It looks like you angered the wrong committee, and they have staged a terror attack on the Towering Levitus at the heart of the country and blamed you for it. You suddenly find yourself in front of the elite squad of the military you thought you controlled as they take you into court for a televised trial for Treason. How ironic!" << std::endl;
			}
			break;
		case Moore::Frontend::Result::INSURRECTION:
			{
				std::wcout << L"INSURRECTION - The people have had enough of your rule and have successfully staged an insurrection to get rid of you. After the chaos, you are now in the Mountains Complex Presidential Bunker, fleeing from the armed militants. The new ruler has just rose to power and is taking the military to your location to 'close loose ends'. Asides from your impending doom, it seems that the new ruler has taken a liking to your war-time policies and has declared a permanate War on Insurrectionists." << std::endl;
			}
			break;
		case Moore::Frontend::Result::ARMEGGEDON:
			{
				std::wcout << L"ARMEGGEDON - You ruthlessly pulled the levers of power past their breaking point, and one of your predictions failed. Aisatsae and Aisrue have both launched thermonuclear missiles at your country, starting a nuclear war. The planet itself broke into many pieces in space, killing all lifeform. You somehow manage to escape to a top-secret satalite bunker, but you die within weeks of malnutrition. There is Nothing left because of you." << std::endl;
			}
			break;
		case Moore::Frontend::Result::LIZZARDATION://them lizzard people
			{
				std::wcout << L"LIZZARDIZED - You have been taken against your will into the infamous ROOM 101 and placed inside the transmutation machine. You entered human and left as a lizard person. You try to think of the things that led you here, but it seems that you remember nothing. Your only thoughts are praise for those around you, those who placed you inside the machine. You are then taken out of ROOM 101, but you were never in that dreadful room. In fact, there was never a ROOM 101. You try to ask the security escort what is going on, but you can not understand his language as if it is ancient. You try to remember who you were, you remember nothing." << std::endl;
			}
			break;
		case Moore::Frontend::Result::SUCCESS:
			{
				std::wcout << L"SUCCESS - You somehow made it past everyone and everything that stood in the way to power and you have overtaken Acirema. The Deep Decadence that you fought so feverously has just fallen you swiftly took the reins of power. You suddenly realize your need to maintain the dictatorship by any and all means necessary, so you take the remaining military and suppress by force the last public dissenters. While the old Department of Truth has been toppled, you institute the brand new Bureau of The True Truth to regulate the public information and 'protect' it. You remain the sole leader of the Superstate of Acirema until your death." << std::endl;
			}
			break;
		default:
			break;
	}

	//print final state
	std::wcout << L"RESULTS: " << std::endl << std::endl
		<< L"ENDING CASH: " << st.Cash << std::endl
		<< L"ENDING POWER: " << st.Power << std::endl
		<< L"ENDING CORRUPTION: " << st.Corruption << std::endl << std::endl
		<< L"PRESS ENTER TO CONTINUE" << std::endl;

	//wait for enter
	std::wstring a;
	std::getline (std::wcin, a);
	return;
}

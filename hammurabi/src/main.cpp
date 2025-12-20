#include "services/GameEngine.h"
#include "utils/utility.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	
	static bool firstRun = true;
	
	do
	{
		GameEngine engine;
		
	if (firstRun)
	{
			engine.ShowMainScreen();
		firstRun = false;
	}
	
		engine.Run();

		system("cls");
		std::cout << "\n\n";
		std::cout << std::setw(60) << std::setfill(' ') << "Хотите сыграть снова? Y/N\n";
	}
	while (ProcessOneshotInput());
	
	return 0;
}

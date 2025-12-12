#pragma once

#include "../domain/GameState.h"
#include "../domain/CityState.h"
#include "../domain/Statistics.h"
#include "SaveManager.h"
#include "InputHandler.h"
#include "DisplayManager.h"
#include <cstdint>
#include <random>

class GameEngine
{
public:
	GameEngine();
	void Run();
	bool LoadGame();
	void ShowMainScreen();

private:
	void BeginRound();
	void ProcessPlayerInput();
	void EndRound();
	
	bool CheckGameOver() const;
	bool CheckWin();
	void CalculateAcrePrice();
	void ProcessHarvest();
	void ProcessRats();
	void ProcessHunger();
	void ProcessNewPeople();
	void ProcessPlague();
	void ApplyPlayerDecisions(const PlayerDecisions& decisions);

private:
	CityState m_State;
	GameStatistics m_Stats;
	GameState m_GameState;
	
	SaveManager m_SaveManager;
	InputHandler m_InputHandler;
	DisplayManager m_DisplayManager;
	
	std::mt19937 m_RandomGenerator;
};


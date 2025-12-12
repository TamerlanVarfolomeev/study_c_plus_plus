#include "GameEngine.h"
#include "../config/GameConfig.h"
#include "../utils/utility.h"
#include <algorithm>
#include <ctime>
#include <random>

GameEngine::GameEngine()
	: m_State(),
	m_Stats(),
	m_GameState(GameState::Ongoing),
	m_RandomGenerator(static_cast<unsigned>(std::time(nullptr)))
{
	CalculateAcrePrice();
}

void GameEngine::Run()
{
	if (m_SaveManager.HasSaves())
	{
		LoadGame();
	}
	
	while (m_GameState == GameState::Ongoing)
	{
		BeginRound();
		ProcessPlayerInput();
		EndRound();
		
		bool gameOver = CheckGameOver();
		bool win = CheckWin();
		if (gameOver || win)
		{
			m_GameState = GameState::Finished;
		}
	}
}

bool GameEngine::LoadGame()
{
	return m_SaveManager.LoadGame(m_State, m_Stats);
}

void GameEngine::ShowMainScreen()
{
	m_DisplayManager.ShowMainScreen();
}

void GameEngine::BeginRound()
{
	m_DisplayManager.ShowRoundStart(m_State);
	
	m_State.DeadFromHunger = 0;
	m_State.NewPeople = 0;
	m_State.HasPlague = false;
	m_State.WheatEatenByRats = 0;
	
	CalculateAcrePrice();
}

void GameEngine::ProcessPlayerInput()
{
	PlayerDecisions decisions = m_InputHandler.GetPlayerDecisions(m_State);
	ApplyPlayerDecisions(decisions);
}

void GameEngine::ApplyPlayerDecisions(const PlayerDecisions& decisions)
{
	if (decisions.BuyLand > 0)
	{
		uint32_t cost = decisions.BuyLand * m_State.AcrePrice;
		m_State.Area = m_State.Area + decisions.BuyLand;
		m_State.WheatReserves = m_State.WheatReserves - cost;
	}
	else if (decisions.SellLand > 0)
	{
		uint32_t income = decisions.SellLand * m_State.AcrePrice;
		m_State.Area = m_State.Area - decisions.SellLand;
		m_State.WheatReserves = m_State.WheatReserves + income;
	}
	
	m_State.WheatConsumed = decisions.WheatForFood;
	m_State.WheatReserves = m_State.WheatReserves - decisions.WheatForFood;
	
	float seeds = decisions.AcresToPlant * GameConfig::Game::SEEDS_PER_ACRE;
	uint32_t seedsNeeded = (uint32_t)seeds;
	m_State.WorkableArea = decisions.AcresToPlant;
	m_State.WheatReserves = m_State.WheatReserves - seedsNeeded;
}

void GameEngine::EndRound()
{
	ProcessHarvest();
	ProcessRats();
	ProcessHunger();
	ProcessNewPeople();
	ProcessPlague();
	
	if (CheckGameOver())
	{
		m_DisplayManager.ShowGameOver();
		ProcessOneshotInput(false);
		return;
	}
	
	bool wantSave = m_InputHandler.RequestSave();
	if (wantSave)
	{
		bool saved = m_SaveManager.SaveGame(m_State, m_Stats);
		if (saved)
		{
			m_GameState = GameState::Finished;
		}
		return;
	}
	
	// Переход к следующему раунду
	m_State.Round++;
}

void GameEngine::CalculateAcrePrice()
{
	std::uniform_int_distribution<uint32_t> dist(
		GameConfig::Game::MIN_ACRE_PRICE,
		GameConfig::Game::MAX_ACRE_PRICE
	);
	m_State.AcrePrice = dist(m_RandomGenerator);
}

void GameEngine::ProcessHarvest()
{
	std::uniform_int_distribution<uint32_t> dist(
		GameConfig::Game::MIN_WHEAT_PER_ACRE,
		GameConfig::Game::MAX_WHEAT_PER_ACRE
	);
	m_State.WheatPerAcre = dist(m_RandomGenerator);
	uint32_t harvested = m_State.WorkableArea * m_State.WheatPerAcre;
	m_State.WheatReserves = m_State.WheatReserves + harvested;
}

void GameEngine::ProcessRats()
{
	std::uniform_real_distribution<float> dist(0.0f, GameConfig::Game::RATS_EAT_MAX_PERCENT);
	float coeff = dist(m_RandomGenerator);
	float eaten = coeff * (float)m_State.WheatReserves;
	m_State.WheatEatenByRats = (uint32_t)eaten;
	m_State.WheatReserves = m_State.WheatReserves - m_State.WheatEatenByRats;
}

void GameEngine::ProcessHunger()
{
	uint32_t oldPop = m_State.Population;
	
	uint32_t peopleFed = m_State.WheatConsumed / GameConfig::Game::WHEAT_PER_PERSON;
	uint32_t minVal = m_State.Population;
	if (peopleFed < minVal)
		minVal = peopleFed;
	m_State.DeadFromHunger = m_State.Population - minVal;
	
	float deadPercent = 0.0f;
	if (oldPop > 0)
	{
		deadPercent = (float)m_State.DeadFromHunger / (float)oldPop;
	}
	m_Stats.SetRoundStatistics(m_State.Round, deadPercent);
	
	m_State.Population = m_State.Population - m_State.DeadFromHunger;
}

void GameEngine::ProcessNewPeople()
{
	uint32_t wheatBeforeRats = m_State.WheatReserves + m_State.WheatEatenByRats;
	
	int32_t part1 = (int32_t)m_State.DeadFromHunger / 2;
	int32_t part2 = (5 - (int32_t)m_State.WheatPerAcre) * (int32_t)wheatBeforeRats / 600;
	int32_t newPeople = part1 + part2 + 1;
	
	if (newPeople < 0)
		newPeople = 0;
	if (newPeople > (int32_t)GameConfig::Game::MAX_NEW_PEOPLE)
		newPeople = (int32_t)GameConfig::Game::MAX_NEW_PEOPLE;
	
	m_State.NewPeople = (uint32_t)newPeople;
	m_State.Population = m_State.Population + m_State.NewPeople;
}

void GameEngine::ProcessPlague()
{
	std::uniform_int_distribution<uint32_t> dist(1, 100);
	m_State.HasPlague = (dist(m_RandomGenerator) <= GameConfig::Game::PLAGUE_PROBABILITY);
	
	if (m_State.HasPlague)
	{
		m_State.Population /= 2;
	}
}

bool GameEngine::CheckGameOver() const
{
	RoundStatistics roundStats = m_Stats.GetRoundStatistics(m_State.Round);
	return roundStats.DeadFromHungerPercent >= GameConfig::Game::MAX_DEAD_FROM_HUNGER;
}

bool GameEngine::CheckWin()
{
	if (m_State.Round >= GameConfig::Game::MAX_ROUNDS)
	{
		m_DisplayManager.ShowFinalRating(m_State, m_Stats);
		ProcessOneshotInput(false);
		return true;
	}
	return false;
}


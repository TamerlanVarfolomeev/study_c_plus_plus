#include "DisplayManager.h"
#include "../config/GameConfig.h"
#include "../utils/utility.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdint>

DisplayManager::DisplayManager()
{
}

void DisplayManager::ShowMainScreen() const
{
	system("cls");
	
	std::vector<std::string> artLines = LoadMainScreenArt();
	
	std::vector<std::string> textLines = {
		"",
		"═════════════════════════════════════════════════════════",
		"               ПРАВИТЕЛЬ ЕГИПТА - ХАММУРАПИ",
		"═════════════════════════════════════════════════════════",
		"",
		"   Добро пожаловать в древний Вавилон!",
		"",
		"   Вы - правитель города, управляйте ресурсами мудро.",
		"",
		"   Нажмите любую клавишу, чтобы начать игру...",
		""
	};
	
	PrintArtWithText(artLines, textLines);
	ProcessOneshotInput(false);
}

void DisplayManager::ShowRoundStart(const CityState& state) const
{
	system("cls");
	
	std::vector<std::string> advisorArt;
	if (state.HasPlague)
	{
		advisorArt = LoadRatArt();
	}
	else
	{
		advisorArt = LoadAdvisorArt();
	}
	
	std::vector<std::string> textLines = BuildRoundStartText(state);
	
	PrintArtWithText(advisorArt, textLines);
}

void DisplayManager::ShowFinalRating(const CityState& state, const GameStatistics& stats) const
{
	system("cls");
	
	GameStatistics::Rating rating = stats.GetRating(state.Area, state.Population);
	
	std::cout << GameConfig::Messages::GAME_FINISHED;
	
	switch (rating)
	{
	case GameStatistics::Rating::Poor:
		std::cout << GameConfig::Ratings::POOR;
		break;
	case GameStatistics::Rating::Fair:
		std::cout << GameConfig::Ratings::FAIR;
		break;
	case GameStatistics::Rating::Good:
		std::cout << GameConfig::Ratings::GOOD;
		break;
	case GameStatistics::Rating::Excellent:
		std::cout << GameConfig::Ratings::EXCELLENT;
		break;
	}
}

void DisplayManager::ShowGameOver() const
{
	std::cout << GameConfig::Messages::GAME_OVER_HUNGER;
	std::cout << "Нажмите любую клавишу, чтобы продолжить.";
}

std::vector<std::string> DisplayManager::LoadMainScreenArt() const
{
	return LoadASCIIArtFromFile(GameConfig::Paths::MAIN_SCREEN);
}

std::vector<std::string> DisplayManager::LoadAdvisorArt() const
{
	return LoadASCIIArtFromFile(GameConfig::Paths::ADVISOR_ART);
}

std::vector<std::string> DisplayManager::LoadRatArt() const
{
	return LoadASCIIArtFromFile(GameConfig::Paths::RAT_ART);
}

void DisplayManager::PrintArtWithText(const std::vector<std::string>& art, const std::vector<std::string>& text) const
{
	size_t maxHeight = art.size();
	if (text.size() > maxHeight)
		maxHeight = text.size();
	
	size_t maxArtWidth = 0;
	for (size_t i = 0; i < art.size(); i++)
	{
		if (art[i].length() > maxArtWidth)
			maxArtWidth = art[i].length();
	}
	
	size_t ART_WIDTH = maxArtWidth + 5;
	
	for (size_t i = 0; i < maxHeight; i++)
	{
		if (i < art.size())
		{
			std::cout << std::left << std::setw((int)ART_WIDTH) 
				<< std::setfill(' ') << art[i];
		}
		else
		{
			for (size_t j = 0; j < ART_WIDTH; j++)
				std::cout << " ";
		}
		
		if (i < text.size())
		{
			std::cout << text[i];
		}
		
		std::cout << std::endl;
	}
}

std::vector<std::string> DisplayManager::BuildRoundStartText(const CityState& state) const
{
	std::vector<std::string> textLines;
	
	textLines.push_back("Мой повелитель, я хочу поведать тебе о " + 
		std::to_string(state.Round) + " годе твоего правления");
	textLines.push_back("");
	
	if (state.DeadFromHunger > 0)
		textLines.push_back(std::to_string(state.DeadFromHunger) + " человек умерло от голода.");
	else
		textLines.push_back("");
	
	if (state.NewPeople > 0)
		textLines.push_back(std::to_string(state.NewPeople) + " человек прибыло в наш великий город");
	else
		textLines.push_back("");
	
	if (state.HasPlague)
		textLines.push_back("Наш город также постигла чума.");
	else
		textLines.push_back("");
	
	textLines.push_back("Сейчас в городе " + std::to_string(state.Population) + " жителей.");
	textLines.push_back("");
	textLines.push_back("С каждого акра было собрано " + std::to_string(state.WheatPerAcre) + " бушелей,");
	textLines.push_back("а всего собрано " + std::to_string(state.WorkableArea * state.WheatPerAcre) + " бушелей.");
	textLines.push_back("");
	textLines.push_back("Крысы съели " + std::to_string(state.WheatEatenByRats) + " бушелей пшена!");
	textLines.push_back("");
	textLines.push_back("В запасах города осталось " + std::to_string(state.WheatReserves) + " бушелей.");
	textLines.push_back("");
	
	textLines.push_back("В этом году цена акра составляет " + 
		std::to_string(state.AcrePrice) + " бушелей пшена.");
	
	return textLines;
}


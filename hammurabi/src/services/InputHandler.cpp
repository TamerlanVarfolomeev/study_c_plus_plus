#include "InputHandler.h"
#include "../config/GameConfig.h"
#include "../utils/utility.h"
#include <iostream>
#include <algorithm>

InputHandler::InputHandler()
{
}

PlayerDecisions InputHandler::GetPlayerDecisions(const CityState& state) const
{
	PlayerDecisions decisions;
	decisions.BuyLand = 0;
	decisions.SellLand = 0;
	decisions.WheatForFood = 0;
	decisions.AcresToPlant = 0;
	
	bool boughtLand = false;
	decisions.BuyLand = GetBuyLand(state);
	if (decisions.BuyLand > 0)
	{
		boughtLand = true;
	}
	
	if (!boughtLand)
	{
		decisions.SellLand = GetSellLand(state);
	}
	
	decisions.WheatForFood = GetWheatForFood(state);
	decisions.AcresToPlant = GetAcresToPlant(state);
	
	return decisions;
}

int32_t InputHandler::GetBuyLand(const CityState& state) const
{
	std::string input;
	while (true)
	{
		std::cout << "\nСколько акров земли повелеваешь купить? ";
		int32_t buyAmount = ProcessIntegerInput(input, GameConfig::Messages::INTEGER_INPUT_ERROR);
		
		if (buyAmount <= 0)
			return 0;
		
		if (ValidateBuyLand(buyAmount, state))
			return buyAmount;
	}
}

int32_t InputHandler::GetSellLand(const CityState& state) const
{
	std::string input;
	while (true)
	{
		std::cout << "Сколько акров земли повелеваешь продать? ";
		int32_t sellAmount = ProcessIntegerInput(input, GameConfig::Messages::INTEGER_INPUT_ERROR);
		
		if (sellAmount <= 0)
			return 0;
		
		if (ValidateSellLand(sellAmount, state))
			return sellAmount;
	}
}

int32_t InputHandler::GetWheatForFood(const CityState& state) const
{
	std::string input;
	while (true)
	{
		std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
		int32_t wheatAmount = ProcessIntegerInput(input, GameConfig::Messages::INTEGER_INPUT_ERROR);
		
		if (ValidateWheatForFood(wheatAmount, state))
			return wheatAmount;
	}
}

int32_t InputHandler::GetAcresToPlant(const CityState& state) const
{
	std::string input;
	while (true)
	{
		std::cout << "Сколько акров земли повелеваешь засеять? ";
		int32_t acres = ProcessIntegerInput(input, GameConfig::Messages::INTEGER_INPUT_ERROR);
		
		if (ValidateAcresToPlant(acres, state))
			return acres;
	}
}

bool InputHandler::ValidateBuyLand(int32_t buyAmount, const CityState& state) const
{
	uint32_t cost = buyAmount * state.AcrePrice;
	if (cost > state.WheatReserves)
	{
		std::cout << "Правитель, у нас нет столько пшена. У нас "
			<< state.WheatReserves << ", а вы хотите потратить " << cost << ".\n";
		return false;
	}
	return true;
}

bool InputHandler::ValidateSellLand(int32_t sellAmount, const CityState& state) const
{
	if (sellAmount > state.Area)
	{
		std::cout << "Правитель, у нас нет столько земель. У нас всего "
			<< state.Area << " акров.\n";
		return false;
	}
	return true;
}

bool InputHandler::ValidateWheatForFood(int32_t wheatAmount, const CityState& state) const
{
	if (wheatAmount > state.WheatReserves)
	{
		std::cout << "Правитель, у нас нет столько пшена. У нас всего "
			<< state.WheatReserves << " бушелей.\n";
		return false;
	}
	return true;
}

bool InputHandler::ValidateAcresToPlant(int32_t acres, const CityState& state) const
{
	float seedsFloat = acres * GameConfig::Game::SEEDS_PER_ACRE;
	uint32_t seedsNeeded = (uint32_t)seedsFloat;
	uint32_t maxAcresByPeople = state.Population * GameConfig::Game::ACRES_PER_PERSON;
	
	if (seedsNeeded > state.WheatReserves)
	{
		std::cout << "Правитель, помилуй, у нас нет столько пшена для семян. У нас всего "
			<< state.WheatReserves << " бушелей.\n";
		return false;
	}
	
	if (acres > (int32_t)maxAcresByPeople)
	{
		std::cout << "Правитель, помилуй, у нас нет столько людей. У нас всего "
			<< state.Population << " человек, которые могут обработать "
			<< maxAcresByPeople << " акров.\n";
		return false;
	}
	
	if (acres > (int32_t)state.Area)
	{
		std::cout << "Правитель, помилуй, у нас нет столько земли. У нас всего "
			<< state.Area << " акров.\n";
		return false;
	}
	
	return true;
}

bool InputHandler::RequestSave() const
{
	std::cout << GameConfig::Messages::SAVE_ROUND_PROMPT;
	return ProcessOneshotInput();
}


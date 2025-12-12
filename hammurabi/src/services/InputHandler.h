#pragma once

#include "../domain/CityState.h"
#include <cstdint>
#include <string>

struct PlayerDecisions
{
	int32_t BuyLand;
	int32_t SellLand;
	int32_t WheatForFood;
	int32_t AcresToPlant;
};

class InputHandler
{
public:
	InputHandler();
	PlayerDecisions GetPlayerDecisions(const CityState& state) const;
	bool RequestSave() const;

private:
	int32_t GetBuyLand(const CityState& state) const;
	int32_t GetSellLand(const CityState& state) const;
	int32_t GetWheatForFood(const CityState& state) const;
	int32_t GetAcresToPlant(const CityState& state) const;
	bool ValidateBuyLand(int32_t buyAmount, const CityState& state) const;
	bool ValidateSellLand(int32_t sellAmount, const CityState& state) const;
	bool ValidateWheatForFood(int32_t wheatAmount, const CityState& state) const;
	bool ValidateAcresToPlant(int32_t acres, const CityState& state) const;
};


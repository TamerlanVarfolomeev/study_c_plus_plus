#include "CityState.h"

CityState::CityState()
	: Population(100),
	Area(1000),
	WheatReserves(2800),
	Round(1),
	AcrePrice(0),
	WorkableArea(0),
	WheatPerAcre(5),
	WheatConsumed(2000),
	DeadFromHunger(0),
	NewPeople(0),
	WheatEatenByRats(0),
	HasPlague(false)
{
}

CityState::CityState(uint32_t population, uint32_t area, uint32_t wheatReserves)
	: Population(population),
	Area(area),
	WheatReserves(wheatReserves),
	Round(1),
	AcrePrice(0),
	WorkableArea(0),
	WheatPerAcre(5),
	WheatConsumed(2000),
	DeadFromHunger(0),
	NewPeople(0),
	WheatEatenByRats(0),
	HasPlague(false)
{
}


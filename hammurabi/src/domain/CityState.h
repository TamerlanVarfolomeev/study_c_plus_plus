#pragma once

#include <cstdint>

struct CityState
{
	uint32_t Population;        // Население города
	uint32_t Area;              // Количество акров земли
	uint32_t WheatReserves;     // Запасы пшеницы (бушели)
	
	uint32_t Round;             // Номер раунда (1-10)
	uint32_t AcrePrice;         // Цена одного акра земли в этом году
	
	uint32_t WorkableArea;      // Количество акров, засеянных в этом году
	uint32_t WheatPerAcre;      // Урожайность (бушелей с акра)
	uint32_t WheatConsumed;     // Пшеница, использованная на еду

	uint32_t DeadFromHunger;    // Умерло от голода в прошлом раунде
	uint32_t NewPeople;         // Прибыло новых людей в прошлом раунде
	uint32_t WheatEatenByRats;  // Пшеница, съеденная крысами
	bool HasPlague;             // Была ли чума в прошлом раунде

	CityState();
	CityState(uint32_t population, uint32_t area, uint32_t wheatReserves);
};


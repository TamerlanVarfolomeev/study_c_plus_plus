#include "Statistics.h"
#include <algorithm>
#include <numeric>

GameStatistics::GameStatistics()
{
	m_Rounds.fill(RoundStatistics{});
}

void GameStatistics::SetRoundStatistics(uint32_t round, float deadFromHungerPercent)
{
	if (round > 0 && round <= MAX_ROUNDS)
	{
		m_Rounds[round - 1].DeadFromHungerPercent = deadFromHungerPercent;
	}
}

RoundStatistics GameStatistics::GetRoundStatistics(uint32_t round) const
{
	if (round > 0 && round <= MAX_ROUNDS)
	{
		return m_Rounds[round - 1];
	}
	return RoundStatistics{};
}

float GameStatistics::CalculateAverageDeadFromHunger() const
{
	float sum = 0.0f;
	for (int i = 0; i < MAX_ROUNDS; i++)
	{
		sum = sum + m_Rounds[i].DeadFromHungerPercent;
	}
	return sum / (float)MAX_ROUNDS;
}

int32_t GameStatistics::CalculateAcresPerPerson(uint32_t area, uint32_t population) const
{
	if (population == 0)
		return 0;
	int32_t result = (int32_t)(area / population);
	return result;
}

GameStatistics::Rating GameStatistics::GetRating(uint32_t area, uint32_t population) const
{
	float averageDead = CalculateAverageDeadFromHunger();
	int32_t acresPerPerson = CalculateAcresPerPerson(area, population);
	
	if (averageDead > 0.33f && acresPerPerson < 7)
		return Rating::Poor;
	else if (averageDead > 0.1f && acresPerPerson < 9)
		return Rating::Fair;
	else if (averageDead > 0.03f && acresPerPerson < 10)
		return Rating::Good;
	else
		return Rating::Excellent;
}


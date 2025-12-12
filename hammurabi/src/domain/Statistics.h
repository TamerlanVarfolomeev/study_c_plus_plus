#pragma once

#include <cstdint>
#include <array>

// Статистика одного раунда
struct RoundStatistics
{
	float DeadFromHungerPercent;
	RoundStatistics() : DeadFromHungerPercent(0.0f) {}
};

class GameStatistics
{
public:
	static constexpr size_t MAX_ROUNDS = 10;
	
	GameStatistics();
	void SetRoundStatistics(uint32_t round, float deadFromHungerPercent);
	RoundStatistics GetRoundStatistics(uint32_t round) const;
	float CalculateAverageDeadFromHunger() const;
	int32_t CalculateAcresPerPerson(uint32_t area, uint32_t population) const;
	enum class Rating
	{
		Poor,           // Плохо
		Fair,           // Удовлетворительно
		Good,           // Хорошо
		Excellent       // Отлично
	};
	
	Rating GetRating(uint32_t area, uint32_t population) const;

private:
	std::array<RoundStatistics, MAX_ROUNDS> m_Rounds;
};


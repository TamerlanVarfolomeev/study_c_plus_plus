#pragma once

#include "../domain/CityState.h"
#include "../domain/Statistics.h"
#include <string>
#include <vector>

class DisplayManager
{
public:
	DisplayManager();
	void ShowMainScreen() const;
	void ShowRoundStart(const CityState& state) const;
	void ShowFinalRating(const CityState& state, const GameStatistics& stats) const;
	void ShowGameOver() const;

private:
	std::vector<std::string> LoadMainScreenArt() const;
	std::vector<std::string> LoadAdvisorArt() const;
	std::vector<std::string> LoadRatArt() const;
	void PrintArtWithText(const std::vector<std::string>& art, const std::vector<std::string>& text) const;
	std::vector<std::string> BuildRoundStartText(const CityState& state) const;
};


#pragma once

#include "../domain/CityState.h"
#include "../domain/Statistics.h"
#include <filesystem>
#include <string>
#include <vector>

class SaveManager
{
public:
	SaveManager();
	bool HasSaves() const;
	bool RequestLoad() const;
	bool LoadGame(CityState& state, GameStatistics& stats) const;
	bool SaveGame(const CityState& state, const GameStatistics& stats) const;
	std::vector<std::filesystem::path> GetSaveFiles() const;

private:
	std::filesystem::path m_SavesPath;
	std::filesystem::path ChooseSaveFile() const;
	bool LoadFromFile(const std::filesystem::path& filePath, CityState& state, GameStatistics& stats) const;
	bool SaveToFile(const std::filesystem::path& filePath, const CityState& state, const GameStatistics& stats) const;
};


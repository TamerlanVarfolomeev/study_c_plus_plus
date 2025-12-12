#include "SaveManager.h"
#include "../config/GameConfig.h"
#include "../utils/utility.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

SaveManager::SaveManager()
	: m_SavesPath(GameConfig::Paths::SAVES_DIR)
{
}

bool SaveManager::HasSaves() const
{
	if (!std::filesystem::exists(m_SavesPath))
		return false;
	
	for (const auto& entry : std::filesystem::directory_iterator(m_SavesPath))
	{
		if (entry.is_regular_file() && entry.file_size() > 0)
			return true;
	}
	return false;
}

bool SaveManager::RequestLoad() const
{
	std::cout << GameConfig::Messages::LOAD_SAVE_PROMPT;
	return ProcessOneshotInput();
}

std::vector<std::filesystem::path> SaveManager::GetSaveFiles() const
{
	std::vector<std::filesystem::path> saveFiles;
	
	if (!std::filesystem::exists(m_SavesPath))
		return saveFiles;
	
	for (const auto& entry : std::filesystem::directory_iterator(m_SavesPath))
	{
		if (entry.is_regular_file())
			saveFiles.push_back(entry.path());
	}
	
	return saveFiles;
}

std::filesystem::path SaveManager::ChooseSaveFile() const
{
	std::cout << GameConfig::Messages::SAVE_FILE_PROMPT;
	
	std::vector<std::filesystem::path> saveFiles = GetSaveFiles();
	std::unordered_map<uint32_t, std::filesystem::path> saveMap;
	
	uint32_t index = 1;
	for (size_t i = 0; i < saveFiles.size(); i++)
	{
		std::cout << index << ": " << saveFiles[i].filename() << std::endl;
		saveMap[index] = saveFiles[i];
		index = index + 1;
	}
	
	std::string input;
	std::filesystem::path chosenFile;
	
	while (std::cin >> input)
	{
		try
		{
			uint32_t fileNum = std::stoul(input);
			if (fileNum > 0 && fileNum <= saveFiles.size())
			{
				chosenFile = saveMap[fileNum];
				break;
			}
			std::cout << "Введен неправильный номер файла, попробуйте еще раз.\n";
		}
		catch (const std::invalid_argument&)
		{
			auto it = std::find_if(saveMap.begin(), saveMap.end(),
				[&input](const auto& pair)
				{
					auto inputPath = std::filesystem::path(input);
					return pair.second.filename() == inputPath
						|| pair.second.filename().stem() == inputPath;
				});
			
			if (it != saveMap.end())
			{
				chosenFile = it->second;
				break;
			}
			std::cout << "Такого сохранения нет, попробуйте еще раз.\n";
		}
	}
	
	return chosenFile;
}

bool SaveManager::LoadFromFile(const std::filesystem::path& filePath, CityState& state, GameStatistics& stats) const
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cout << GameConfig::Messages::LOAD_ERROR;
		return false;
	}
	
	file >> state.Round;
	file >> state.DeadFromHunger;
	file >> state.NewPeople;
	file >> state.HasPlague;
	file >> state.Population;
	
	file >> state.WheatPerAcre;
	file >> state.WorkableArea;
	file >> state.WheatEatenByRats;
	file >> state.WheatReserves;
	file >> state.WheatConsumed;
	
	file >> state.Area;
	file >> state.AcrePrice;
	
	for (uint32_t i = 1; i <= GameStatistics::MAX_ROUNDS; i++)
	{
		float deadPercent;
		file >> deadPercent;
		stats.SetRoundStatistics(i, deadPercent);
	}
	
	return true;
}

bool SaveManager::LoadGame(CityState& state, GameStatistics& stats) const
{
	if (!HasSaves() || !RequestLoad())
		return false;
	
	std::filesystem::path chosenFile = ChooseSaveFile();
	if (chosenFile.empty())
		return false;
	
	return LoadFromFile(chosenFile, state, stats);
}

bool SaveManager::SaveToFile(const std::filesystem::path& filePath, const CityState& state, const GameStatistics& stats) const
{
	if (!std::filesystem::exists(m_SavesPath))
		std::filesystem::create_directory(m_SavesPath);
	
	std::ofstream file(filePath, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		std::cout << GameConfig::Messages::SAVE_ERROR;
		return false;
	}
	
	file << state.Round << std::endl;
	file << state.DeadFromHunger << std::endl;
	file << state.NewPeople << std::endl;
	file << state.HasPlague << std::endl;
	file << state.Population << std::endl;
	
	file << state.WheatPerAcre << std::endl;
	file << state.WorkableArea << std::endl;
	file << state.WheatEatenByRats << std::endl;
	file << state.WheatReserves << std::endl;
	file << state.WheatConsumed << std::endl;
	
	file << state.Area << std::endl;
	file << state.AcrePrice << std::endl;
	
	for (uint32_t i = 1; i <= GameStatistics::MAX_ROUNDS; i++)
	{
		RoundStatistics roundStats = stats.GetRoundStatistics(i);
		file << roundStats.DeadFromHungerPercent << std::endl;
	}
	
	return true;
}

bool SaveManager::SaveGame(const CityState& state, const GameStatistics& stats) const
{
	system("cls");
	std::cout << GameConfig::Messages::SAVE_NAME_PROMPT;
	
	std::string filename;
	std::cin >> filename;
	
	std::filesystem::path fullPath = m_SavesPath / filename;
	
	return SaveToFile(fullPath, state, stats);
}


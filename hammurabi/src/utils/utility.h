#pragma once

#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

// Универсальная функция для загрузки ASCII-арта из файла в вектор строк
inline std::vector<std::string> LoadASCIIArtFromFile(const std::filesystem::path& filePath)
{
	std::vector<std::string> artLines;
	
	std::ifstream file{filePath, std::ios::in | std::ios::binary};
	
	if (!file.is_open())
	{
		return artLines;
	}

	size_t fileSize = 0;
	if (std::filesystem::exists(filePath))
	{
		fileSize = std::filesystem::file_size(filePath);
	}
	
	if (fileSize == 0)
	{
		file.close();
		return artLines;
	}
	
	std::string content;
	content.resize(fileSize);
	file.read(&content[0], fileSize);
	file.close();
	
	if (content.empty() || fileSize == 0)
	{
		return artLines;
	}
	
	std::string line;
	for (size_t i = 0; i < content.length(); ++i)
	{
		char c = content[i];
		if (c == '\r' && i + 1 < content.length() && content[i + 1] == '\n')
		{
			// Windows line ending
			artLines.push_back(line);
			line.clear();
			i++; // Пропускаем \n
		}
		else if (c == '\n')
		{
			// Unix line ending
			artLines.push_back(line);
			line.clear();
		}
		else
		{
			line += c;
		}
	}
	
	if (!line.empty())
	{
		artLines.push_back(line);
	}
	
	return artLines;
}

// Обработка однократного ввода (Y/N)
inline bool ProcessOneshotInput(bool isValidated = true, char acceptChar = 'Y', char denyChar = 'N')
{
	if (isValidated)
	{
		while (true)
		{
			char input = 0;

			input = std::toupper(_getch());

			if (input == std::toupper(acceptChar))
				return true;
			if (input == std::toupper(denyChar))
				return false;
		}
	}
	_getch();
	return true;
}

// Обработка целочисленного ввода
inline int32_t ProcessIntegerInput(std::string& input, const std::string& errorOutput = "Only integer input is allowed. Try again. ")
{
	while (std::cin >> input)
	{
		try
		{
			return std::stoi(input);
		}
		catch (const std::exception&)
		{
			std::cout << errorOutput;
		}
	}
	return -1;
}


#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

// Конфигурация игры - все константы и настройки
namespace GameConfig
{
	// Начальные параметры игры
	namespace Initial
	{
		constexpr uint32_t POPULATION = 100;
		constexpr uint32_t AREA = 1000;
		constexpr uint32_t WHEAT_RESERVES = 2800;
		constexpr uint32_t WHEAT_CONSUMED = 2000;
		constexpr uint32_t WHEAT_PER_ACRE = 5;
	}
	
	// Параметры игры
	namespace Game
	{
		constexpr uint32_t MAX_ROUNDS = 10;
		constexpr uint32_t WHEAT_PER_PERSON = 20;        // Бушелей пшеницы на человека в год
		constexpr uint32_t ACRES_PER_PERSON = 10;         // Максимум акров на одного человека
		constexpr float SEEDS_PER_ACRE = 0.5f;            // Бушелей семян на акр
		constexpr uint32_t MIN_ACRE_PRICE = 17;          // Минимальная цена акра
		constexpr uint32_t MAX_ACRE_PRICE = 26;           // Максимальная цена акра
		constexpr uint32_t MIN_WHEAT_PER_ACRE = 1;       // Минимальный урожай с акра
		constexpr uint32_t MAX_WHEAT_PER_ACRE = 6;        // Максимальный урожай с акра
		constexpr float RATS_EAT_MAX_PERCENT = 0.07f;    // Максимальный процент пшеницы, съедаемой крысами
		constexpr uint32_t PLAGUE_PROBABILITY = 15;      // Вероятность чумы в процентах
		constexpr float MAX_DEAD_FROM_HUNGER = 0.45f;     // Максимальный процент умерших от голода (проигрыш)
		constexpr uint32_t MAX_NEW_PEOPLE = 50;          // Максимум новых людей за раунд
	}
	
	// Пути к файлам
	namespace Paths
	{
		const std::filesystem::path SAVES_DIR = "./Saves/";
		const std::filesystem::path MAIN_SCREEN = "./Screens/MainScren.txt";
		const std::filesystem::path ADVISOR_ART = "./Screens/advisor.txt";
		const std::filesystem::path RAT_ART = "./Screens/rat.txt";
	}
	
	// Сообщения для пользователя
	namespace Messages
	{
		const std::string INTEGER_INPUT_ERROR = "Повелитель, введи число, я не понимаю. ";
		const std::string LOAD_SAVE_PROMPT = "Вы хотели бы продолжить игру с сохранения? Y/N\n";
		const std::string SAVE_FILE_PROMPT = "Введите номер или название сохранения.\n";
		const std::string SAVE_NAME_PROMPT = "Введите название файла: ";
		const std::string SAVE_ERROR = "Не удалось сохранить файл, приносим своиз извинения. Скилл ишью.";
		const std::string LOAD_ERROR = "Не удалось открыть файл, начинаем новую игру.\n";
		const std::string GAME_OVER_HUNGER = "\n\nПо вашей вине погибло слишком много людей! Вы не достойны быть правителем!\n";
		const std::string SAVE_ROUND_PROMPT = "\nОстановить игру и сохранить раунд? Y/N";
		const std::string GAME_FINISHED = "\nПовелитель, ты окончил свое правление!\n";
	}
	
	// Оценки правления
	namespace Ratings
	{
		const std::string POOR = "Из - за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании.\n";
		const std::string FAIR = "Вы правили железной рукой, подобно Нерону и Ивану Грозному.Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем\n";
		const std::string GOOD = "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова\n";
		const std::string EXCELLENT = "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше\n\n";
	}
}


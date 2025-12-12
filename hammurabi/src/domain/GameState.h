#pragma once

#include <cstdint>

// Состояние игры
enum class GameState : uint8_t
{
	Ongoing,    // Игра продолжается
	Finished,   // Игра завершена
};


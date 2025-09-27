#pragma once
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "UI.h"
#include <unordered_map>

namespace ApplesGame 
{
	// Table of records
	struct Record {
		std::string name;
		int score;
	};

	struct Game
	{
		uint32_t mode;
		Player player;
		Apple* apples;
		int numApples;
		int initialNumApples;
		Rock rocks[NUM_ROCKS];

		int deathCount = 0;

		// Global game data
		int numEatenApples = 0;
		bool isGameFinished = false;
		bool isGameBeaten = false;
		float timeSinceGameFinish = 0.f;
		sf::RectangleShape background;

		// Resources 
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;
		sf::SoundBuffer appleEatBuffer;
		sf::SoundBuffer deathBuffer;
		sf::Sound eatSound;
		sf::Sound deathSound;

		// UI
		sf::Font font;
		UI uiState;

		std::unordered_map<std::string, int> leaderboard{
			{"Alice", 60},
			{"Bob",   85},
			{"Carol", 100},
			{"Dave",  40}
		};
	};

	enum class GameSettingBits : uint32_t {
		IsGameWithAcceleration = 1 << 0,
		IsGameInfinite = 1 << 1,

	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DeinializeGame(Game& game);
	bool compareRecords(const Record& a, const Record& b);
}

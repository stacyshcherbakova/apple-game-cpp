#include "Game.h"
#include <cassert>

namespace ApplesGame {

	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);
		game.player.sprite.setRotation(0);
		game.numApples = game.initialNumApples;

		// Init apples
		for (int i = 0; i < game.numApples; ++i)
		{
			InitApple(game.apples[i], game);
		}

		// Init rocks
		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRock(game.rocks[i], game);
		}

		game.numEatenApples = 0;
		game.isGameBeaten = false;
		game.isGameFinished = false;
		game.timeSinceGameFinish = 0;
	}

	void InitGame(Game& game)
	{
		game.font.loadFromFile(
			"C:/Windows/Fonts/arial.ttf"
		);  
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
		assert(game.appleEatBuffer.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.deathBuffer.loadFromFile(RESOURCES_PATH + "\\Death.wav"));
		game.eatSound.setBuffer(game.appleEatBuffer);
		game.deathSound.setBuffer(game.deathBuffer);
		//assert(game.font.loadFromFile(
		//	RESOURssssCES_PATH + "\\OpenSans-Regular.ttf"
		//));

		game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);

		InitUI(game.uiState, game.font);

		RestartGame(game);
	}

	void HandleInput(Game& game) {
		// Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			game.player.direction = PlayerDirection::Right;
			game.player.sprite.setRotation(0);
			SetSpriteSize(game.player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			game.player.direction = PlayerDirection::Up;
			game.player.sprite.setRotation(270);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			game.player.direction = PlayerDirection::Left;
			game.player.sprite.setRotation(180);
			SetSpriteSize(game.player.sprite, PLAYER_SIZE, -PLAYER_SIZE);

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			game.player.direction = PlayerDirection::Down;
			game.player.sprite.setRotation(90);
		}

	}

	void UpdatePlayerState(Game& game, float deltaTime) {
		// Update player state
		switch (game.player.direction)
		{
		case PlayerDirection::Right:
		{
			game.player.position.x += game.player.speed * deltaTime;
			break;
		}
		case PlayerDirection::Up:
		{
			game.player.position.y -= game.player.speed * deltaTime;
			break;
		}
		case PlayerDirection::Left:
		{
			game.player.position.x -= game.player.speed * deltaTime;
			break;
		}
		case PlayerDirection::Down:
		{
			game.player.position.y += game.player.speed * deltaTime;
			break;
		}
		}
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		// Update game state
		if (!game.isGameFinished)
		{
			HandleInput(game);
			UpdatePlayerState(game, deltaTime);

			// Find player collisions with apples
			for (int i = 0; i < game.numApples; ++i)
			{
				if (IsCirclesCollide(game.player.position, PLAYER_SIZE / 2.f,
					game.apples[i].position, APPLE_SIZE / 2.f))
				{
					game.eatSound.play();
					
					if (game.mode & static_cast<uint32_t>(GameSettingBits::IsGameInfinite)) {
						game.apples[i].position = GetRandomPositionInScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
					}
					else {
							const int last = game.numApples - 1;
							game.apples[i] = game.apples[last];
							--game.numApples;
							++game.numEatenApples;
						if (game.numApples == 0) {
							game.isGameBeaten = true;
							game.isGameFinished = true;
							game.player.speed = 0.f;
							game.timeSinceGameFinish = 0.f;
							game.background.setFillColor(sf::Color::Green);
							break;
							
						}
					}

					if (game.mode & static_cast<uint32_t>(GameSettingBits::IsGameWithAcceleration)) {
						game.player.speed += ACCELERATION;
					}
				}
			}


			// Find player collisions with rocks
			for (int i = 0; i < NUM_ROCKS; ++i)
			{
				if (IsRectanglesCollide(game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
					game.rocks[i].position, { ROCK_SIZE, ROCK_SIZE }))
				{
					game.deathSound.play();
					game.isGameFinished = true;
					game.timeSinceGameFinish = 0.f;
					game.deathCount += 1;
				}
			}

			// Check screen borders collision
			if (game.player.position.x - PLAYER_SIZE / 2.f < 0.f || game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
				game.player.position.y - PLAYER_SIZE / 2.f < 0.f || game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
			{ 
				game.deathSound.play();
				game.isGameFinished = true;
				game.timeSinceGameFinish = 0.f;
				game.deathCount += 1;
			}
		}

		else
		{
			if (game.timeSinceGameFinish <= PAUSE_LENGTH)
			{
				game.timeSinceGameFinish += deltaTime;
				game.background.setFillColor(game.isGameBeaten ? sf::Color::Green : sf::Color::Red);
			}

			else
			{
				// Reset backgound
				game.background.setFillColor(sf::Color::Black);

				RestartGame(game);
			}
		}
		UpdateUI(game.uiState, game);
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);
		DrawPlayer(game.player, window);
		//game.player.shape.setPosition(game.player.position.x, game.player.position.y);
		//window.draw(game.player.shape);
		for (int i = 0; i < game.numApples; ++i)
		{
			/*game.apples[i].shape.setPosition(game.apples[i].position.x, game.apples[i].position.y);
			window.draw(game.apples[i].shape);*/
				DrawApple(game.apples[i], window);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			//game.rocks[i].shape.setPosition(game.rocks[i].position.x, game.rocks[i].position.y);
			//window.draw(game.rocks[i].shape);
			DrawRock(game.rocks[i], window);
		}
		DrawUI(game.uiState, window);

	}

	void DeinializeGame(Game& game)
	{
		delete [] game.apples;
	}

	bool compareRecords(const Record& a, const Record& b) {
		if (a.score != b.score) return a.score > b.score;
		return a.name < b.name;
	}
}

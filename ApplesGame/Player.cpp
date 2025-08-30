#include "Player.h"
#include "Game.h"

namespace ApplesGame {

	void InitPlayer(Player& player, const Game& game)
	{
		// Init player state
		player.position = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
		player.speed = INITIAL_SPEED;
		player.direction = PlayerDirection::Right;

		// Init player sprite
		player.sprite.setTexture(game.playerTexture);
		SetSpriteSize(player.sprite, PLAYER_SIZE, PLAYER_SIZE);
		SetSpriteRelativeOrigin(player.sprite, 0.5f, 0.5f);

		//// Init player shape
		//player.shape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
		//player.shape.setFillColor(sf::Color::Red);
		//player.shape.setOrigin(PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f);
		//player.shape.setPosition(player.position.x, player.position.y);
	}
	void DrawPlayer(Player& player, sf::RenderWindow& window) {
		player.sprite.setPosition(player.position.x, player.position.y);
		window.draw(player.sprite);
	}
}
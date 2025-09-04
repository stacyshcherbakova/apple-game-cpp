#pragma once
#include "SFML/Graphics.hpp"

namespace ApplesGame 
{
	struct UI {
		sf::Text score;
		sf::Text gameOver;
		sf::Text inputHint;
		sf::Text youWon;
		bool isGameOver;
		bool isGameBeaten;

	};

	struct Game;

	void InitUI(UI& ui, const sf::Font& font);
	void UpdateUI(UI& ui, Game& game);
	void DrawUI(UI& ui, sf::RenderWindow& window);
}

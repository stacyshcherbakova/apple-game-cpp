#include "Game.h"
#include "UI.h"
//#include "Math.h"
#include <string>

namespace ApplesGame 
{
	void InitUI(UI& ui, const sf::Font& font) {
		ui.score.setFont(font);
		ui.score.setCharacterSize(24);
		ui.score.setFillColor(sf::Color::Yellow);

		ui.inputHint.setFont(font);
		ui.inputHint.setCharacterSize(24);
		ui.inputHint.setFillColor(sf::Color::Yellow);
		ui.inputHint.setString("Use arrow keys to move, ESC to exit");
		ui.inputHint.setOrigin(GetTextOrigin(ui.inputHint, { 1.f, 0.f }));

		ui.isGameOver = false;
		ui.gameOver.setFont(font);
		ui.gameOver.setCharacterSize(48);
		ui.gameOver.setFillColor(sf::Color::Yellow);
		ui.gameOver.setString("GAME OVER");
		ui.gameOver.setOrigin(GetTextOrigin(ui.gameOver, { 0.5f, 0.5f }));

		ui.isGameBeaten = false;
		ui.youWon.setFont(font);
		ui.youWon.setCharacterSize(48);
		ui.youWon.setFillColor(sf::Color::Yellow);
		ui.youWon.setString("YOU WON!");
		ui.youWon.setOrigin(GetTextOrigin(ui.youWon, { 0.5f, 0.5f }));
	}

	void UpdateUI(UI& ui, Game& game) {
		ui.isGameOver = game.isGameFinished;
		ui.isGameBeaten = game.isGameBeaten;
		ui.score.setString("Score: " + std::to_string(game.numEatenApples) + " " + "Deaths: " + std::to_string(game.deathCount) + " / 3");
	}

	void DrawUI(UI& ui, sf::RenderWindow& window) {
		ui.score.setPosition(10.f, 10.f);
		window.draw(ui.score);

		ui.inputHint.setPosition(window.getSize().x - 10.f, 10.f);
		window.draw(ui.inputHint);

		if (ui.isGameOver)
		{
			ui.gameOver.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
			window.draw(ui.gameOver);
		}
		if (ui.isGameBeaten)
		{
			ui.youWon.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
			window.draw(ui.youWon);
		}
	}
}
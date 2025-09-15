// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"
#include <iostream>
using namespace std;

int main()
{
    using namespace ApplesGame;
    int seed = (int)time(nullptr);
    srand(seed);

    // Init window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

    // Init game clocks
    sf::Clock gameClock;
    float lastTime = 0.f;

    sf::Font font;
    font.loadFromFile(
        "C:/Windows/Fonts/arial.ttf"
    );

    sf::Text promptApples("How many apples? (Enter to confirm)", font, 24);
    promptApples.setPosition(40.f, 40.f);

    sf::Text promptAcceleration("Choose acceleration mode (enter number):", font, 24);
    promptAcceleration.setPosition(40.f, 40.f);

    sf::Text promptInfinite("Infinite apples re-spawn? (enter number):", font, 24);
    promptInfinite.setPosition(40.f, 40.f);

    sf::Text accelOption1("1 = With Acceleration", font, 20);
    accelOption1.setPosition(60.f, 90.f);

    sf::Text accelOption2("2 = No Acceleration", font, 20);
    accelOption2.setPosition(60.f, 120.f);

    sf::Text infiniteOption1("1 = Yes", font, 20);
    infiniteOption1.setPosition(60.f, 90.f);

    sf::Text infiniteOption2("2 = No", font, 20);
    infiniteOption2.setPosition(60.f, 120.f);

    std::string inputStr;
    sf::Text inputText("", font, 32);
    inputText.setPosition(40.f, 210.f);

    enum class InputState { Acceleration, Infinite, Apples, Playing };
    InputState state = InputState::Acceleration;

    Game game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if ((state == InputState::Acceleration || state == InputState::Infinite || state == InputState::Apples) &&
                event.type == sf::Event::TextEntered) {
                const uint32_t u = event.text.unicode;

                if (u == 8) {
                    if (!inputStr.empty()) inputStr.pop_back();
                }
                else if (u >= '0' && u <= '9') {
                    if (inputStr.size() < 6) inputStr.push_back(static_cast<char>(u));
                }
            }

            // Commit on Enter
            if ((state == InputState::Acceleration || state == InputState::Infinite || state == InputState::Apples) &&
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {

                if (state == InputState::Acceleration) {
                    if (inputStr == "1") {
                        game.mode |= static_cast<uint32_t>(GameSettingBits::IsGameWithAcceleration);
                        state = InputState::Infinite;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                    }
                    else if (inputStr == "2") {
                        state = InputState::Infinite;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                        continue;
                    }
                    else {
                        if (inputStr != "1" && inputStr != "2") {
                            inputStr.clear();
                            continue;
                        }
                    }
                }
                else if (state == InputState::Infinite) {
                    if (inputStr == "1") {
                        game.mode |= static_cast<uint32_t>(GameSettingBits::IsGameInfinite);
                        state = InputState::Apples;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                    }
                    else if (inputStr == "2") {
                        state = InputState::Apples;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                        continue;
                    }
                    else {
                        if (inputStr != "1" && inputStr != "2") {
                            inputStr.clear();
                            continue;
                        }
                    }
                }
                else if (state == InputState::Apples) {
                    if (!inputStr.empty()) {
                        int n = std::stoi(inputStr);
                        if (n > 0) {
                            game.initialNumApples = n;
                            game.numApples = n;
                            game.apples = new Apple[game.initialNumApples];
                            InitGame(game);

                            gameClock.restart();
                            lastTime = 0.f;

                            state = InputState::Playing;
                            inputStr.clear();
                        }
                    }
                }
            }
        }

        window.clear();


        if (state == InputState::Acceleration) {
            inputText.setString(inputStr);

            window.draw(promptAcceleration);
            window.draw(accelOption1);
            window.draw(accelOption2);
            window.draw(inputText);

            window.display();
            continue;
        }
        else if (state == InputState::Infinite) {
            inputText.setString(inputStr);

            window.draw(promptInfinite);
            window.draw(infiniteOption1);
            window.draw(infiniteOption2);
            window.draw(inputText);

            window.display();
            continue;
        }
        else if (state == InputState::Apples) {
            inputText.setString(inputStr);

            window.draw(promptApples);
            window.draw(inputText);

            window.display();
            continue;
        }


        else if (state == InputState::Playing) {
            float currentTime = gameClock.getElapsedTime().asSeconds();
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            if (game.deathCount == 3) {

                static sf::Clock gameOverClock;

                if (!game.isGameFinished) {
                    sf::Clock gameOverClock;
                    game.isGameFinished = true;
                }

                if (gameOverClock.getElapsedTime().asSeconds() >= 3.f) {
                    window.close();
                    return 0;
                }
            }

            else if (game.numApples == 0) {
                static sf::Clock gameOverClock;

                if (!game.isGameBeaten) {
                    sf::Clock gameOverClock;
                    game.isGameBeaten = true;
                    game.isGameFinished = true;
                }

                if (gameOverClock.getElapsedTime().asSeconds() >= 3.f) {
                    window.close();
                    return 0;
                }
            }
           
            UpdateGame(game, deltaTime);
            DrawGame(game, window);
        }

        window.display();

    }

    DeinializeGame(game);
    return 0;
}
	

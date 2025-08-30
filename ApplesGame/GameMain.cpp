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

    sf::Text prompt1("How many apples? (Enter to confirm)", font, 24);
    prompt1.setPosition(40.f, 180.f);

    sf::Text prompt2("Which game mode? (Enter numbre to confirm)", font, 24);
    prompt2.setPosition(40.f, 40.f);

    sf::Text option1("1 = With Acceleration", font, 20);
    option1.setPosition(60.f, 90.f);

    sf::Text option2("2 = No Acceleration", font, 20);
    option2.setPosition(60.f, 120.f);

    std::string inputStr;
    sf::Text inputText("", font, 32);
    inputText.setPosition(40.f, 150.f);

    enum class InputState { Mode, Apples, Playing };
    InputState state = InputState::Mode;

    Game game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if ((state == InputState::Mode || state == InputState::Apples) &&
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
            if ((state == InputState::Mode || state == InputState::Apples) &&
                event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {

                if (state == InputState::Mode) {
                    if (inputStr == "1") {
                        game.mode = static_cast<uint32_t>(GameSettingBits::IsGameWithAcceleration);
                        state = InputState::Apples;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                    }
                    else if (inputStr == "2") {
                        game.mode = static_cast<uint32_t>(GameSettingBits::IsGameNoAcceleration);
                        state = InputState::Apples;
                        inputStr.clear();
                        inputText.setPosition(40.f, 210.f);
                    }
                    else {
                        // invalid mode: ignore or flash a message
                    }
                }
                else if (state == InputState::Apples) {
                    if (!inputStr.empty()) {
                        int n = std::stoi(inputStr);
                        if (n > 0) {
                            game.numApples = n;
                            game.apples = new Apple[game.numApples];
                            InitGame(game);

                            gameClock.restart();
                            lastTime = 0.f;

                            state = InputState::Playing;
                            inputStr.clear();
                        }
                        else {
                            // invalid n: ignore or show warning
                        }
                    }
                }
            }
        }

        window.clear();


        if (state == InputState::Mode) {
            inputText.setString(inputStr);

            window.draw(prompt2); // what mode?
            window.draw(option1);
            window.draw(option2);
            window.draw(inputText);

            window.display();
            continue;
        }


        else if (state == InputState::Apples) {
            inputText.setString(inputStr);
            window.draw(prompt1);  // "How many apples?"
            window.draw(inputText);
            window.display();
            continue;
        }

        else if (state == InputState::Playing) {
            float currentTime = gameClock.getElapsedTime().asSeconds();
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            // Normal game update/draw
            UpdateGame(game, deltaTime);
            DrawGame(game, window);
        }

        window.display();

    }

    DeinializeGame(game);
    return 0;
}
	

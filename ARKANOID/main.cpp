#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Game.h"

enum class GameStateEnum { Menu, Playing, GameOver, Exiting };

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Arkanoid");
    window.setFramerateLimit(60);

    Game game(window.getSize().x, window.getSize().y);
    GameStateEnum state = GameStateEnum::Menu;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        return 1;
    }

    const int MENU_ITEMS = 3;
    sf::Text menuItems[MENU_ITEMS];
    menuItems[0].setString("GRAJ");
    menuItems[1].setString("WCZYTAJ GRE");
    menuItems[2].setString("WYJSCIE");

    for (int i = 0; i < MENU_ITEMS; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setCharacterSize(32);
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setPosition(640.f / 2.f - 120.f, 180.f + i * 60.f);
    }

    int menuSelected = 0;
    menuItems[menuSelected].setFillColor(sf::Color::Yellow);

    sf::Text overItems[2];
    overItems[0].setString("GRAJ JESZCZE RAZ");
    overItems[1].setString("WYJSCIE");

    for (int i = 0; i < 2; i++) {
        overItems[i].setFont(font);
        overItems[i].setCharacterSize(28);
        overItems[i].setFillColor(sf::Color::White);
        overItems[i].setPosition(640.f / 2.f - 140.f, 260.f + i * 50.f);
    }

    int overSelected = 0;
    overItems[overSelected].setFillColor(sf::Color::Yellow);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: 0");

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            if (state == GameStateEnum::Menu) {
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Up) {
                        menuSelected = (menuSelected - 1 + MENU_ITEMS) % MENU_ITEMS;
                    }
                    else if (e.key.code == sf::Keyboard::Down) {
                        menuSelected = (menuSelected + 1) % MENU_ITEMS;
                    }
                    else if (e.key.code == sf::Keyboard::Enter) {
                        if (menuSelected == 0) {
                            game.reset();
                            state = GameStateEnum::Playing;
                        }
                        else if (menuSelected == 1) {
                            if (game.loadFromFile("zapis.txt")) {
                                std::cout << "Gra wczytana!\n";
                                state = GameStateEnum::Playing;
                            }
                            else {
                                std::cout << "Brak pliku zapis.txt lub blad odczytu.\n";
                            }
                        }
                        else if (menuSelected == 2) {
                            window.close();
                        }
                    }
                }
            }
            else if (state == GameStateEnum::Playing) {
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Escape) {
                        state = GameStateEnum::Menu;
                    }
                    else if (e.key.code == sf::Keyboard::F5) {
                        if (game.saveToFile("zapis.txt")) {
                            std::cout << "Gra zapisana!\n";
                        }
                        else {
                            std::cout << "Blad zapisu!\n";
                        }
                    }
                }
            }
            else if (state == GameStateEnum::GameOver) {
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Up) {
                        overSelected = (overSelected - 1 + 2) % 2;
                    }
                    else if (e.key.code == sf::Keyboard::Down) {
                        overSelected = (overSelected + 1) % 2;
                    }
                    else if (e.key.code == sf::Keyboard::Enter) {
                        if (overSelected == 0) {
                            game.reset();
                            state = GameStateEnum::Playing;
                        }
                        else if (overSelected == 1) {
                            window.close();
                        }
                    }
                }
            }
        }

        sf::Time dt = clock.restart();

        if (state == GameStateEnum::Playing) {
            game.update(dt);

            if (game.isBallLost()) {
                state = GameStateEnum::GameOver;
            }
        }

        window.clear(sf::Color(10, 10, 20));

        if (state == GameStateEnum::Menu) {
            for (int i = 0; i < MENU_ITEMS; i++)
                menuItems[i].setFillColor(sf::Color::White);
            menuItems[menuSelected].setFillColor(sf::Color::Yellow);

            for (int i = 0; i < MENU_ITEMS; i++)
                window.draw(menuItems[i]);
        }
        else if (state == GameStateEnum::Playing) {
            game.render(window);

            scoreText.setString("Score: " + std::to_string(game.getScore()));
            sf::FloatRect sb = scoreText.getLocalBounds();
            scoreText.setPosition(
                window.getSize().x - sb.width - 10.f,
                window.getSize().y - sb.height - 20.f
            );
            window.draw(scoreText);
        }
        else if (state == GameStateEnum::GameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(40);
            gameOverText.setString("KONIEC GRY");
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(640.f / 2.f - 130.f, 150.f);
            window.draw(gameOverText);

            for (int i = 0; i < 2; i++)
                overItems[i].setFillColor(sf::Color::White);
            overItems[overSelected].setFillColor(sf::Color::Yellow);

            for (int i = 0; i < 2; i++)
                window.draw(overItems[i]);
        }

        window.display();
    }

    return 0;
}

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "paletka.h"
#include "pilka.h"
#include "cegielka.h"

class Game {
private:
    Paletka m_paletka;
    Pilka   m_pilka;
    std::vector<Cegielka> m_bloki;

    float m_width;
    float m_height;
    float m_blockWidth;
    float m_blockHeight;

    int m_score;

public:
    Game(float width = 640.f, float height = 480.f);

    void loadLevel();
    void reset();
    void update(sf::Time dt);
    void render(sf::RenderTarget& target);

    bool isBallLost() const;
    int  getScore() const { return m_score; }

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#include "Game.h"
#include "GameState.h"
#include <algorithm>

Game::Game(float width, float height)
    : m_width(width),
    m_height(height),
    m_blockWidth(0.f),
    m_blockHeight(0.f),
    m_paletka(width / 2.f, height - 40.f, 80.f, 15.f, 7.f),
    m_pilka(width / 2.f, height / 2.f, 3.f, 3.f, 7.f),
    m_score(0)
{
    loadLevel();
}

void Game::loadLevel() {
    m_bloki.clear();

    const int kolumny = 8;
    const int wiersze = 5;

    float slotWidth = m_width / kolumny;
    float slotHeight = 25.f;

    m_blockWidth = slotWidth - 4.f;
    m_blockHeight = slotHeight - 4.f;

    float startY = 40.f;

    for (int r = 0; r < wiersze; r++) {
        for (int c = 0; c < kolumny; c++) {
            float x = slotWidth * c + slotWidth / 2.f;
            float y = startY + r * slotHeight;

            m_bloki.emplace_back(x, y, m_blockWidth, m_blockHeight);
        }
    }
}

void Game::reset() {
    m_paletka = Paletka(m_width / 2.f, m_height - 40.f, 80.f, 15.f, 7.f);
    m_pilka = Pilka(m_width / 2.f, m_height / 2.f, 3.f, 3.f, 7.f);
    m_score = 0;
    loadLevel();
}

void Game::update(sf::Time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        m_paletka.moveLeft();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        m_paletka.moveRight();

    m_paletka.clampToBounds(m_width);

    m_pilka.move();
    m_pilka.collideWalls(m_width, m_height);
    m_pilka.collidePaddle(m_paletka);

    for (auto& c : m_bloki) {
        m_score += c.collideBall(m_pilka);
    }

    m_bloki.erase(
        std::remove_if(
            m_bloki.begin(),
            m_bloki.end(),
            [](const Cegielka& c) { return !c.isAlive(); }),
        m_bloki.end()
    );
}

void Game::render(sf::RenderTarget& target) {
    for (auto& c : m_bloki)
        c.draw(target);

    m_paletka.draw(target);
    m_pilka.draw(target);
}

bool Game::isBallLost() const {
    return (m_pilka.getY() - m_pilka.getRadius()) > m_height;
}


bool Game::saveToFile(const std::string& filename) const {
    GameState state;
    state.capture(m_paletka, m_pilka, m_bloki);
    return state.saveToFile(filename);
}

bool Game::loadFromFile(const std::string& filename) {
    GameState state;
    if (!state.loadFromFile(filename))
        return false;

    state.apply(m_paletka, m_pilka, m_bloki, m_blockWidth, m_blockHeight);
    return true;
}

#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <fstream>

#include "paletka.h"
#include "pilka.h"
#include "cegielka.h"

struct BlockData {
    float x, y;
    int hp;
};

class GameState {
public:
    sf::Vector2f paddlePosition;
    sf::Vector2f ballPosition;
    sf::Vector2f ballVelocity;
    std::vector<BlockData> blocks;

    GameState() = default;

    void capture(const Paletka& p, const Pilka& b, const std::vector<Cegielka>& cegielki) {
        paddlePosition = sf::Vector2f(p.getX(), p.getY());
        ballPosition = sf::Vector2f(b.getX(), b.getY());
        ballVelocity = sf::Vector2f(b.getVx(), b.getVy());

        blocks.clear();
        blocks.reserve(cegielki.size());
        for (const auto& c : cegielki) {
            BlockData d;
            d.x = c.getX();
            d.y = c.getY();
            d.hp = c.getHP();
            blocks.push_back(d);
        }
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "PADDLE " << paddlePosition.x << " " << paddlePosition.y << "\n";
        file << "BALL " << ballPosition.x << " " << ballPosition.y << " "
            << ballVelocity.x << " " << ballVelocity.y << "\n";
        file << "BLOCKS_COUNT " << blocks.size() << "\n";

        for (const auto& b : blocks) {
            file << b.x << " " << b.y << " " << b.hp << "\n";
        }

        return true;
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string label;

        if (!(file >> label >> paddlePosition.x >> paddlePosition.y)) return false;

        if (!(file >> label >> ballPosition.x >> ballPosition.y
            >> ballVelocity.x >> ballVelocity.y)) return false;

        int blocksCount = 0;
        if (!(file >> label >> blocksCount)) return false;

        blocks.clear();
        blocks.reserve(blocksCount);

        for (int i = 0; i < blocksCount; ++i) {
            BlockData d;
            if (!(file >> d.x >> d.y >> d.hp)) return false;
            blocks.push_back(d);
        }

        return true;
    }

    void apply(Paletka& p, Pilka& b, std::vector<Cegielka>& cegielki,
        float blockWidth, float blockHeight) const
    {
        p.setPosition(paddlePosition.x, paddlePosition.y);
        b.setPosition(ballPosition.x, ballPosition.y);
        b.setVelocity(ballVelocity.x, ballVelocity.y);

        cegielki.clear();
        cegielki.reserve(blocks.size());

        for (const auto& data : blocks) {
            Cegielka c(data.x, data.y, blockWidth, blockHeight);
            c.setHP(data.hp);
            cegielki.push_back(c);
        }
    }
};
#pragma once

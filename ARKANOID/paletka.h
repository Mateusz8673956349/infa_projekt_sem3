#pragma once
#include <SFML/Graphics.hpp>

class Paletka {
private:
    float x;
    float y;
    float szerokosc;
    float wysokosc;
    float predkosc;
    sf::RectangleShape shape;

public:
    Paletka(float startX, float startY, float szer, float wys, float v)
        : x(startX),
        y(startY),
        szerokosc(szer),
        wysokosc(wys),
        predkosc(v)
    {
        shape.setSize({ szerokosc, wysokosc });
        shape.setOrigin(szerokosc / 2.f, wysokosc / 2.f);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Cyan);
    }

    void moveLeft() {
        x -= predkosc;
        shape.setPosition(x, y);
    }

    void moveRight() {
        x += predkosc;
        shape.setPosition(x, y);
    }

    void clampToBounds(float width) {
        float halfW = szerokosc / 2.f;

        if (x - halfW < 0.f) {
            x = halfW;
        }
        if (x + halfW > width) {
            x = width - halfW;
        }

        shape.setPosition(x, y);
    }

    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        shape.setPosition(x, y);
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(shape);
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getSzerokosc() const { return szerokosc; }
    float getWysokosc() const { return wysokosc; }
};

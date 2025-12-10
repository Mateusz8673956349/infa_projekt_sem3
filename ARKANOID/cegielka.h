#pragma once
#include <SFML/Graphics.hpp>
#include "pilka.h"

class Cegielka {
private:
    float x, y;
    float szer, wys;
    int zycia;
    sf::RectangleShape shape;

    void updateColor() {
        if (zycia >= 3) {
            shape.setFillColor(sf::Color::Red);
        }
        else if (zycia == 2) {
            shape.setFillColor(sf::Color(255, 165, 0));
        }
        else if (zycia == 1) {
            shape.setFillColor(sf::Color::Yellow);
        }
    }

public:
    Cegielka(float X, float Y, float Szer, float Wys)
        : x(X), y(Y), szer(Szer), wys(Wys), zycia(3)
    {
        shape.setSize({ szer, wys });
        shape.setOrigin(szer / 2.f, wys / 2.f);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }

    bool isAlive() const {
        return zycia > 0;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    int getHP() const { return zycia; }

    void setHP(int hp) {
        zycia = hp;
        if (zycia > 0) updateColor();
    }

    int collideBall(Pilka& ball) {
        if (!isAlive()) return 0;

        float bx = ball.getX();
        float by = ball.getY();
        float r = ball.getRadius();

        float left = x - szer / 2.f;
        float right = x + szer / 2.f;
        float top = y - wys / 2.f;
        float bottom = y + wys / 2.f;

        bool intersects =
            bx + r >= left &&
            bx - r <= right &&
            by + r >= top &&
            by - r <= bottom;

        if (!intersects) return 0;

        int points = 0;
        if (zycia == 3)      points = 10;
        else if (zycia == 2) points = 20;
        else if (zycia == 1) points = 30;

        zycia--;
        if (zycia > 0) updateColor();

        float penLeft = (bx + r) - left;
        float penRight = right - (bx - r);
        float penTop = (by + r) - top;
        float penBottom = bottom - (by - r);

        float penX = std::min(penLeft, penRight);
        float penY = std::min(penTop, penBottom);

        const float EPS = 0.001f;

        if (penX < penY - EPS) {
            ball.bounceX();
            if (penLeft < penRight)
                ball.setX(left - r);
            else
                ball.setX(right + r);

        }
        else if (penY < penX - EPS) {
            ball.bounceY();
            if (penTop < penBottom)
                ball.setY(top - r);
            else
                ball.setY(bottom + r);

        }
        else {
            ball.bounceX();
            ball.bounceY();
            if (penX < penY) {
                if (penLeft < penRight)
                    ball.setX(left - r);
                else
                    ball.setX(right + r);
            }
            else {
                if (penTop < penBottom)
                    ball.setY(top - r);
                else
                    ball.setY(bottom + r);
            }
        }

        return points;
    }

    void draw(sf::RenderTarget& window) {
        if (isAlive())
            window.draw(shape);
    }
};

#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "paletka.h"

class Pilka {
private:
    float x, y;
    float vx, vy;
    float radius;
    sf::CircleShape shape;

public:
    Pilka(float startX, float startY, float startVx, float startVy, float r)
        : x(startX),
        y(startY),
        vx(startVx),
        vy(startVy),
        radius(r)
    {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void move() {
        x += vx;
        y += vy;
        shape.setPosition(x, y);
    }

    void bounceX() { vx = -vx; }
    void bounceY() { vy = -vy; }

    void collideWalls(float width, float height) {
        if (x - radius <= 0.f) {
            x = radius;
            bounceX();
        }
        else if (x + radius >= width) {
            x = width - radius;
            bounceX();
        }

        if (y - radius <= 0.f) {
            y = radius;
            bounceY();
        }


        shape.setPosition(x, y);
    }

    bool collidePaddle(const Paletka& p) {
        float palX = p.getX();
        float palY = p.getY();
        float palW = p.getSzerokosc();
        float palH = p.getWysokosc();

        float palLeft = palX - palW / 2.f;
        float palRight = palX + palW / 2.f;
        float palTop = palY - palH / 2.f;

        bool withinX =
            (x >= palLeft && x <= palRight);

        bool touchingFromTop =
            (y + radius) >= palTop &&
            (y - radius) < palTop;

        if (withinX && touchingFromTop) {
            vy = -std::abs(vy);
            y = palTop - radius;
            shape.setPosition(x, y);
            return true;
        }

        return false;
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(shape);
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getVx() const { return vx; }
    float getVy() const { return vy; }
    float getRadius() const { return radius; }

    void setY(float newY) {
        y = newY;
        shape.setPosition(x, y);
    }

    void setX(float newX) {
        x = newX;
        shape.setPosition(x, y);
    }

    void setPosition(float nx, float ny) {
        x = nx;
        y = ny;
        shape.setPosition(x, y);
    }

    void setVelocity(float nvx, float nvy) {
        vx = nvx;
        vy = nvy;
    }
};

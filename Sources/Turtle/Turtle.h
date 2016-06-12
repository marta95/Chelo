#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace Turtle {
    const double Pi = 3.14159265358979323846;

    double degToRad(double degrees);

    class Turtle
    {
            sf::Vector2f position = { 400.0f, 300.0f };
            sf::Color colour = sf::Color::Red;
            double lineWidth = 1.0f;
            double rotation = 90;
            std::vector<sf::RectangleShape> lines;
            bool penDown = true;

            void drawLine(double lineLength);
            void move(double lineLength);

        public:
            Turtle &moveForward(double lineLength);
            Turtle &moveBackwards(double lineLength);
            Turtle &rotateLeft(double degrees);
            Turtle &rotateRight(double degrees);
            Turtle &penOn();
            Turtle &penOff();
            Turtle &penColour(std::string newColour);
            Turtle &penSize(double pixelSize);
            std::vector<sf::RectangleShape> getLines();
    };
}
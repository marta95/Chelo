#include "Turtle.h"

namespace Turtle {
    double degToRad(double degrees)
    {
        return Pi/180.0f * degrees;
    }

    void Turtle::drawLine(double lineLength)
    {
        sf::RectangleShape line({ static_cast<float>(lineLength),
                                  static_cast<float>(lineWidth) });
        line.setPosition(position);
        line.rotate(static_cast<float>(rotation));
        line.setFillColor(colour);

        std::lock_guard<std::mutex> guard(linesMutex);

        lines.push_back(line);
    }

    void Turtle::move(double lineLength)
    {
        position += { static_cast<float>(lineLength * std::cos(degToRad(rotation))),
                      static_cast<float>(lineLength * std::sin(degToRad(rotation)))};
    }

    Turtle &Turtle::moveForward(double lineLength)
    {
        if (penDown) { drawLine(-lineLength); }
        move(-lineLength);

        return *this;
    }

    Turtle &Turtle::moveBackwards(double lineLength)
    {
        if (penDown) { drawLine(lineLength); }
        move(lineLength);

        return *this;
    }

    Turtle &Turtle::rotateLeft(double degrees)
    {
        rotation -= degrees;

        return *this;
    }

    Turtle &Turtle::rotateRight(double degrees)
    {
        rotation += degrees;

        return *this;
    }

    Turtle &Turtle::penOn()
    {
        penDown = true;

        return *this;
    }

    Turtle &Turtle::penOff()
    {
        penDown = false;

        return *this;
    }

    Turtle &Turtle::penColour(std::string newColour)
    {
        if (newColour == "red") {
            colour = sf::Color::Red;
        }
        else if (newColour == "green") {
            colour = sf::Color::Green;
        }
        else if (newColour == "blue") {
            colour = sf::Color::Blue;
        }
        else {
            throw std::runtime_error("Colour not supported: " + newColour);
        }

        return *this;
    }

    Turtle &Turtle::penSize(double pixelSize)
    {
        lineWidth = pixelSize;

        return *this;
    }

    std::list<sf::RectangleShape> &Turtle::getLines()
    {
        std::lock_guard<std::mutex> guard(linesMutex);

        return lines;
    }

    void Turtle::withLines(std::function<void(std::list<sf::RectangleShape> &)> fun)
    {
        std::lock_guard<std::mutex> guard(linesMutex);

        fun(lines);
    }
}